/*
 * EEPROM_Interface.h
 *
 *  Created on: Mar 24, 2025
 *      Author: Admin
 */

#ifndef INC_EEPROM_INTERFACE_H_
#define INC_EEPROM_INTERFACE_H_

#define EEPROM_SLAVE_ADDRESS    0xA0


#define EEPROM_FLAG_FIRST_TIME_WRITE     0x0000

#define SET_WP  HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_SET)

#define CLR_WP  HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_RESET)

uint8_t EEPROM_u8WriteHalfWordData(uint16_t Copy_u16EepromByteAddress , uint16_t Copy_u16Data);
uint16_t EEPROM_u16ReadHalfWordData(uint16_t Copy_u16EepromByteAddress);

void EEPROM_voidWriteSigmaLog(void);

void EEPROM_voidReadSigmaLog(uint8_t *Copy_pu8ArrLog);

uint8_t EEPROM_Write(uint16_t Copy_u16EepromByteAddress , uint16_t Copy_u16Data);
uint16_t EEPROM_Read(uint16_t Copy_u16EepromByteAddress);


//____________________________________-

void EEPROM_voidWriteBuffer(uint8_t *Data, uint16_t Address, uint16_t Length);

void EEPROM_voidReadBuffer(uint8_t *Copy_pu8Data,uint16_t Copy_u16Address,uint16_t Copy_u16Length);

void EEPROM_voidErase(void);

//------------------------------
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
void EEPROM_voidWriteByteFram(const char *Copy_u8Data,uint16_t Copy_u16Address);

/* @brief   Read a string from FRAM starting from a given address.
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

void EEPROM_voidReadByteFram(char *Copy_u8Data, uint16_t Copy_u16Address);

/**
 * @brief   Clear all data in FRAM by writing null bytes.
 *
 * @note
 * - The function writes 0x00 to the first 1020 bytes of FRAM.
 * - Each byte is written one at a time with a 5ms delay between writes.
 */

void EEPROM_voidClearAll(void);



#endif /* INC_EEPROM_INTERFACE_H_ */
