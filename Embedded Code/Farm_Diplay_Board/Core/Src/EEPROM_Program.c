
/*******************************************************************************
 *                                                                             *
 *   ██████╗ ██████╗ ███╗   ███╗ ██╗ ██████╗                                   *
 *   ██╔══██╗██╔══██╗████╗ ████║███║ ╚════██╗                                  *
 *   ██║  ██║██████╔╝██╔████╔██║╚██║  █████╔╝                                  *
 *   ██║  ██║██╔═══╝ ██║╚██╔╝██║ ██║  ╚═══██╗                                  *
 *   ██████╔╝██║     ██║ ╚═╝ ██║ ██║ ██████╔╝                                  *
 *   ╚═════╝ ╚═╝     ╚═╝     ╚═╝ ╚═╝ ╚═════╝                                   *
 *                                  *                                          *
 *              ⚡ DIGITAL POWER METER  ( D P M 1 3 ) ⚡                       *
 *        Advanced Power Monitoring & Energy Management System                 *
 *                                                                             *
 *                  Developed by: Mohamed Ahmed                                *
 *                  Version: 1.3                                               *
 *                  Date: 2025                                                 *
 *                                                                             *
 ******************************************************************************/

/*******************************************************************************
 *                               SYSTEM OVERVIEW:                              *
 * DPM13 is a high-precision digital power meter designed for accurate         *
 * measurement and monitoring of electrical parameters in various              *
 * applications.                                                               *
 *                                                                             *
 * Key Features:                                                               *
 * - Real-time AC/DC voltage monitoring (0-600V)                               *
 * - Current measurement (0-100A)                                              *
 * - Active, reactive, and apparent power calculation                          *
 * - Energy consumption tracking (kWh)                                         *
 * - Power factor measurement                                                  *
 * - Frequency monitoring (45-65Hz)                                            *
 * - Data logging with timestamp                                               *
 * - Multiple communication interfaces (UART, I2C, Bluetooth ,Spi, ModBuse)    *
 * - OLED/LCD display support                                                  *
 * - Alarm thresholds for over-voltage/current                                 *
 *                                                                             *
 * Applications:                                                               *
 * - Industrial power monitoring                                               *
 * - Home energy management systems                                            *
 * - Renewable energy systems                                                  *
 * - Electrical equipment testing                                              *
 * - Smart grid applications                                                   *
 *                                                                             *
 ******************************************************************************/



#include "main.h"
#include "stdint.h"
#include "string.h"


#include "SERVICE.h"
#include "ErrType.h"
#include "EEPROM_Interface.h"

#include "LOG_FILE.h"

extern I2C_HandleTypeDef hi2c2;
uint8_t FlgErrorReadEEPROM =0;
uint16_t AddrErrorReadEEPROM =0 ;

#define OffsetAddrEpprom 200

/**
 * @brief   Write a half-word (16-bit) data into external EEPROM over I2C.
 *
 * This function writes 2 bytes (16-bit = Half Word) into the EEPROM
 * starting from a given EEPROM byte address. The EEPROM used here follows
 * the 24xx series addressing scheme.
 *
 * @param[in] Copy_u16EepromByteAddress
 *            The logical EEPROM half-word address (not byte).
 *            Max allowed = 0x37F (because address is multiplied by 2).
 *
 * @param[in] Copy_u16Data
 *            The 16-bit data to be written.
 *
 * @return
 * - OK  : Write successful
 * - NOK : Address out of range
 *
 * @note
 * - The function internally multiplies the given address by 2
 *   to convert from half-word index → actual byte address.
 * - Device address calculation:
 *   `Local_u8DeviceAddress = (0xA0 | (Local_u16EepromByteAddress >> 7)) & 0xFE;`
 *   → This selects the correct EEPROM page in case of large memories.
 * - `CLR_WP` and `SET_WP` are macros used to disable/enable write-protect.
 * - A delay of 7ms is added after the write (to wait EEPROM internal cycle).
 * - Uses STM32 HAL function:
 *   `HAL_I2C_Mem_Write(...)`
 */
uint8_t EEPROM_u8WriteHalfWordData(uint16_t Copy_u16EepromByteAddress , uint16_t Copy_u16Data)
{
	uint8_t Local_u8ErrorStatuse = OK ;
	if(Copy_u16EepromByteAddress < 0x380)    // Check max range
	{
		// Disable write protection
		CLR_WP;delay_us(300);

		// Convert word index to byte address
		uint16_t Local_u16EepromByteAddress = Copy_u16EepromByteAddress * 2 ;

		// Build device I2C address (24LCxx addressing scheme)
		uint8_t Local_u8DeviceAddress = (0xA0 | (Local_u16EepromByteAddress >> 7)) & 0xFE;

		// Try writing using HAL
		HAL_I2C_Mem_Write(&hi2c2, Local_u8DeviceAddress, (uint8_t)Local_u16EepromByteAddress, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&Copy_u16Data, 2, HAL_MAX_DELAY);

		// Wait for EEPROM write cycle (5-10ms typical)
		delay_ms(7);

		// Re-enable write protection
		SET_WP;
	}
	else
	{
		Local_u8ErrorStatuse = NOK ;
	}
	return Local_u8ErrorStatuse ;
}

/**
 * @brief   Write a 16-bit data redundantly into EEPROM (main + backup).
 *
 * @param[in] Copy_u16EepromByteAddress
 *            Base address for the data (half-word index).
 *
 * @param[in] Copy_u16Data
 *            16-bit data to store.
 *
 * @return
 * - OK   : Write success (both copies match input)
 * - NOK  : Data mismatch after verification
 * - ERR  : Underlying I2C/HAL error
 */
uint8_t EEPROM_Write(uint16_t Copy_u16EepromByteAddress , uint16_t Copy_u16Data)
{
	uint16_t data1,data2;

	// Write main copy
	EEPROM_u8WriteHalfWordData(Copy_u16EepromByteAddress ,Copy_u16Data);

	// Write backup copy
	EEPROM_u8WriteHalfWordData(Copy_u16EepromByteAddress + OffsetAddrEpprom ,Copy_u16Data);

	// Read back both copies
	data1 = EEPROM_u16ReadHalfWordData(Copy_u16EepromByteAddress);
	data2 = EEPROM_u16ReadHalfWordData(Copy_u16EepromByteAddress + OffsetAddrEpprom );
	if ((data1==data2)&&(data1==Copy_u16Data))		{return OK;}   // ✅ All good
	else											{return NOK;}  // ❌ Data corruption detected
}

/**
 * @brief   Read a 16-bit data with redundancy check.
 *
 * @param[in] Copy_u16EepromByteAddress
 *            Base address for the data (half-word index).
 *
 * @return
 * - Data from EEPROM (main or backup)
 * - Sets error flag if mismatch found
 */
uint16_t EEPROM_Read(uint16_t Copy_u16EepromByteAddress)
{
	uint16_t data1,data2;
	data1 = EEPROM_u16ReadHalfWordData(Copy_u16EepromByteAddress);
	data2 = EEPROM_u16ReadHalfWordData(Copy_u16EepromByteAddress + OffsetAddrEpprom );
	if (data1!=data2)		{FlgErrorReadEEPROM=1;AddrErrorReadEEPROM=Copy_u16EepromByteAddress;}
	return data1;
}

uint16_t EEPROM_u16ReadHalfWordData(uint16_t Copy_u16EepromByteAddress)
{
	uint16_t Local_u16ReturnedValue =0;
	if(Copy_u16EepromByteAddress < 0x380)
	{
		uint16_t Local_u16EepromByteAddress = Copy_u16EepromByteAddress * 2 ;
		uint8_t Local_u8DeviceAddress = (0xA0 | (Local_u16EepromByteAddress >> 7)) & 0xFE;
		HAL_I2C_Mem_Read(&hi2c2, Local_u8DeviceAddress, Local_u16EepromByteAddress, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&Local_u16ReturnedValue, 2, HAL_MAX_DELAY);
	}
	else
	{
		return ERROR_NULL ;
	}
	return Local_u16ReturnedValue ;
}


void EEPROM_voidWriteSigmaLog(void)
{
	uint8_t Local_u8Iterator =0 ;
	while( Sigma_LogArr[Local_u8Iterator] != '\0')
	{
		HAL_I2C_Mem_Write(&hi2c2, 0xAE, Local_u8Iterator, I2C_MEMADD_SIZE_8BIT, &Sigma_LogArr[Local_u8Iterator], 1, HAL_MAX_DELAY);
		Local_u8Iterator ++ ;
		HAL_Delay(5);
	}
}

void EEPROM_voidReadSigmaLog(uint8_t *Copy_pu8ArrLog)
{
	HAL_I2C_Mem_Read(&hi2c2, 0xAE, 0x00, I2C_MEMADD_SIZE_8BIT, Copy_pu8ArrLog, 242, HAL_MAX_DELAY);
}

/**
 * @brief   Write a null-terminated string into FRAM starting from a given address.
 *
 * @param[in] Copy_u8Data
 *            Pointer to the null-terminated string to write.
 *
 * @param[in] Copy_u16Address
 *            Starting byte address in FRAM to write the string.
 *
 * @note
 * - The function disables write protection before writing and re-enables it afterward.
 * - Each character is written one byte at a time with a 5ms delay between writes.
 */
//void EEPROM_voidWriteByteFram(const char *Copy_u8Data,uint16_t Copy_u16Address)
//{
//
//	uint8_t Local_u8Iterator = 0;
//	CLR_WP;
//	delay_us(300);
//	while (Copy_u8Data[Local_u8Iterator] != '\0') {
//		HAL_I2C_Mem_Write(&hi2c1, 0xA0, Copy_u16Address + Local_u8Iterator,
//				I2C_MEMADD_SIZE_8BIT, (uint8_t*) &Copy_u8Data[Local_u8Iterator],
//				1, HAL_MAX_DELAY);
//		Local_u8Iterator++;
//		HAL_Delay(5);
//	}
//	SET_WP;
//}


void EEPROM_voidWriteByteFram(const char *Copy_u8Data, uint16_t Copy_u16Address)
{
    uint16_t Local_u16Iterator = 0;

    CLR_WP;
    delay_us(300);

    while (Copy_u8Data[Local_u16Iterator] != '\0')
    {
        HAL_I2C_Mem_Write(
                &hi2c2,
                0xA0,                     // Device Address
                Copy_u16Address + Local_u16Iterator,
                I2C_MEMADD_SIZE_16BIT,    // 16-bit Memory Address
                (uint8_t *)&Copy_u8Data[Local_u16Iterator],
                1,
                HAL_MAX_DELAY);

        HAL_Delay(5);    // Write Cycle Time

        Local_u16Iterator++;
    }

    SET_WP;
}

/**
 * @brief   Read a string from FRAM starting from a given address.
 *
 * @param[out] Copy_u8Data
 *             Pointer to the buffer where the read string will be stored.
 *
 * @param[in] Copy_u16Address
 *             Starting byte address in FRAM to read the string from.
 *
 * @note
 * - The function reads up to 1020 bytes or until a null terminator is encountered.
 * - The read string is null-terminated before returning.
 */

//void EEPROM_voidReadByteFram(char *Copy_u8Data, uint16_t Copy_u16Address)
//{
//	uint16_t Local_u16Iterator = 0;
//	while (Local_u16Iterator < 1020) {
//		HAL_I2C_Mem_Read(&hi2c1, 0xA0, Copy_u16Address + Local_u16Iterator,
//		I2C_MEMADD_SIZE_8BIT, (uint8_t*) &Copy_u8Data[Local_u16Iterator], 1,
//				HAL_MAX_DELAY);
//		Local_u16Iterator++;
//	}
//	Copy_u8Data[Local_u16Iterator] = '\0'; // Null-terminate the string
//}

void EEPROM_voidReadByteFram(char *Copy_u8Data, uint16_t Copy_u16Address)
{
    uint16_t Local_u16Iterator = 0;

    while (Local_u16Iterator < 1020)
    {
        HAL_I2C_Mem_Read(
                &hi2c2,
                0xA0,
                Copy_u16Address + Local_u16Iterator,
                I2C_MEMADD_SIZE_16BIT,
                (uint8_t *)&Copy_u8Data[Local_u16Iterator],
                1,
                HAL_MAX_DELAY);

        Local_u16Iterator++;
    }

    Copy_u8Data[Local_u16Iterator] = '\0';
}

/**
 * @brief   Clear all data in FRAM by writing null bytes.
 *
 * @note
 * - The function writes 0x00 to the first 1020 bytes of FRAM.
 * - Each byte is written one at a time with a 5ms delay between writes.
 */

//void EEPROM_voidClearAll(void)
//{
//	uint16_t Local_u16Iterator = 0;
//	CLR_WP;
//		delay_us(300);
//	while (Local_u16Iterator < 1020) {
//		HAL_I2C_Mem_Write(&hi2c1, 0xA0, Local_u16Iterator,
//		I2C_MEMADD_SIZE_8BIT, (uint8_t*) "\0", 1, HAL_MAX_DELAY);
//		Local_u16Iterator++;
//		HAL_Delay(5);
//	}
//
//	SET_WP;
//}


void EEPROM_voidClearAll(void)
{
    uint16_t Local_u16Iterator = 0;
    uint8_t Local_u8Data = 0x00;

    CLR_WP;
    delay_us(300);

    while (Local_u16Iterator < 4096)   // AT24C32 = 4096 Bytes
    {
        HAL_I2C_Mem_Write(
                &hi2c2,
                0xA0,                   // أو (0x50 << 1)
                Local_u16Iterator,
                I2C_MEMADD_SIZE_16BIT,
                &Local_u8Data,
                1,
                HAL_MAX_DELAY);

        HAL_Delay(5);   // EEPROM Write Cycle Time

        Local_u16Iterator++;
    }

    SET_WP;
}

//
//void EEPROM_voidWriteByteFramFromEndOfWriting(const char *Copy_u8Data,
//		uint16_t Copy_u16Address) {
//
//	uint8_t Local_u8Iterator = 0;
//	while (Copy_u8Data[Local_u8Iterator] != '\0') {
//		HAL_I2C_Mem_Write(&hi2c1, 0xA0, Copy_u16Address - Local_u8Iterator,
//		I2C_MEMADD_SIZE_8BIT, (uint8_t*) &Copy_u8Data[Local_u8Iterator], 1,
//				HAL_MAX_DELAY);
//		Local_u8Iterator++;
//		HAL_Delay(5);
//	}
//}




#define EEPROM_PAGE_SIZE    32

void EEPROM_voidWriteBuffer(uint8_t *Data, uint16_t Address, uint16_t Length)
{
    uint16_t BytesToWrite;
    uint16_t Offset = 0;

    CLR_WP;

    while (Length > 0)
    {
        BytesToWrite = EEPROM_PAGE_SIZE - (Address % EEPROM_PAGE_SIZE);

        if (BytesToWrite > Length)
            BytesToWrite = Length;

        HAL_I2C_Mem_Write(
            &hi2c2,
            (0x50 << 1),
            Address,
            I2C_MEMADD_SIZE_16BIT,
            &Data[Offset],
            BytesToWrite,
            HAL_MAX_DELAY);


        HAL_Delay(5); // Ensure EEPROM write cycle is complete

        Address += BytesToWrite;
        Offset  += BytesToWrite;
        Length  -= BytesToWrite;
    }

    SET_WP;
}


void EEPROM_voidReadBuffer(uint8_t *Copy_pu8Data,
                           uint16_t Copy_u16Address,
                           uint16_t Copy_u16Length)
{
    HAL_I2C_Mem_Read(
        &hi2c2,
        (0x50 << 1),              // EEPROM I2C Address = 0xA0
        Copy_u16Address,
        I2C_MEMADD_SIZE_16BIT,
        Copy_pu8Data,
        Copy_u16Length,
        HAL_MAX_DELAY);
}


#define EEPROM_SIZE        4096
//#define EEPROM_PAGE_SIZE   32

void EEPROM_voidErase(void)
{
    uint8_t Local_u8Buffer[EEPROM_PAGE_SIZE];

    /* Fill page with 0xFF */
    memset(Local_u8Buffer, 0x00, EEPROM_PAGE_SIZE);

//    for (uint16_t Address = 0; Address < EEPROM_SIZE; Address += EEPROM_PAGE_SIZE)
//    {
//        EEPROM_voidWriteBuffer(Local_u8Buffer, Address, EEPROM_PAGE_SIZE);
//    }


    for (uint16_t Address = 0; Address < 4000; Address += EEPROM_PAGE_SIZE)
       {
           EEPROM_voidWriteBuffer(Local_u8Buffer, Address, EEPROM_PAGE_SIZE);
      }



}
