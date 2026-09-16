/*
 ******************************************************************************
 * @file           : Bloutoth_Program.c
 * @brief          : [Enter a brief description of the file here]
 ******************************************************************************
 * @attention
 *
 * @author   : Mohamed Ahmed
 * @date     : Jul 21, 2026
 * @copyright:
 *   © 2026 M0hamed Ahmed . All rights reserved.
 *
 *   This file is part of the [Project or System Name] project.
 *   Unauthorized copying, modification, or distribution of this file,
 *   via any medium, is strictly prohibited.
 *
 ******************************************************************************
 */
#include "main.h"
#include "stdint.h"
#include "string.h"
#include "ErrType.h"
#include "stdio.h"

#include "EEPROM_Interface.h"
#include "DS1308_Interface.h"
#include "Service_Interface.h"

#include "Bloutoth_Interface.h"
#include "Bloutoth_Private.h"



extern uint16_t	Data_Clock[10];
extern UART_HandleTypeDef huart2;

uint8_t Global_u8ReciveArrBuff[UART_RX_SIZE_BUFFER];



void SytemComunication_voidBluetoothCommProcessFrame(void)
{
	uint8_t Local_u8BufferReciveLenth ;
	uint8_t Local_u8Iterator , Local_u8BufferLenth;
	uint8_t Local_u8Flag = FLAG_IS_LOW ;
	uint8_t Local_u8StartFramPostion,Local_u8EndFramPostion;
	uint8_t Local_u8ArrBuffer[UART_RX_SIZE_BUFFER]={0};
	uint8_t Local_u8IteratorBuff,Local_u8DataLenghToCheck;
	uint16_t Local_u16RecivedChekSum ,Local_u16CalculatedChekSum ;
	uint8_t Local_u8FramRange;

	if( huart2.RxXferCount < 10u )			{Void_ClearArrUart();}
	Local_u8BufferReciveLenth = UART_RX_SIZE_BUFFER - huart2.RxXferCount ;
	if(Local_u8BufferReciveLenth==0)  return;

	for(Local_u8Iterator = 0 ; Local_u8Iterator < Local_u8BufferReciveLenth ; Local_u8Iterator++)
	{
		if((Global_u8ReciveArrBuff[Local_u8Iterator] == ':') && (Local_u8Flag == FLAG_IS_LOW ))
		{
			Local_u8Flag = FLAG_IS_HIGH ;
			Local_u8StartFramPostion = Local_u8Iterator ;
		}
		else if((Global_u8ReciveArrBuff[Local_u8Iterator] == '*') && (Local_u8Flag == FLAG_IS_HIGH ))
		{			//:vlt123456#* start[:]=0 , End(*)=11 , Len=11-0+1=12 , (#)=10
			//time_uart_reset1=0;
			Local_u8EndFramPostion =   Local_u8Iterator ;
			Local_u8BufferLenth = Local_u8EndFramPostion - (Local_u8StartFramPostion + 1u); // to get buffer lenth without start and end

			/*************************************/
			Local_u8Flag = FLAG_IS_HANDLE;
			Global_u8ReciveArrBuff[Local_u8StartFramPostion] = BUFFER_CLEAR ;
			Global_u8ReciveArrBuff[Local_u8EndFramPostion] =   BUFFER_CLEAR ;

			for(Local_u8IteratorBuff = (Local_u8StartFramPostion + 1) ; Local_u8IteratorBuff < Local_u8EndFramPostion ; Local_u8IteratorBuff++)
			{
				Local_u8ArrBuffer[Local_u8IteratorBuff - Local_u8StartFramPostion - 1u] = Global_u8ReciveArrBuff[Local_u8IteratorBuff];
			}
			// Local_u8BufferReciveLenth-3 becam Local_u8BufferLenth- 1
			if(Local_u8ArrBuffer[Local_u8BufferReciveLenth-3] =='#')//(Local_u8ArrBuffer[Local_u8BufferLenth - 1] =='#')  //Check_Sum is important
			{
				Local_u16RecivedChekSum = u32_ConvertStringToIntiger(Local_u8ArrBuffer + Local_u8BufferLenth - 4 , 3);
				Local_u16CalculatedChekSum = 0 ;
				Local_u8DataLenghToCheck = (( Local_u8BufferLenth - 7u ) % 2u) == 1? ((  Local_u8BufferLenth - 7u ) / 2u)+1 :(  Local_u8BufferLenth - 7u ) / 2u ;

				for(Local_u8IteratorBuff = 0 ; Local_u8IteratorBuff < Local_u8DataLenghToCheck ; Local_u8IteratorBuff ++ )
				{
					if((Local_u8IteratorBuff < (Local_u8DataLenghToCheck - 1))  || ((( Local_u8BufferLenth - 7u ) % 2u) == 0))
					{
						Local_u16CalculatedChekSum += u32_ConvertStringToIntiger(((Local_u8ArrBuffer + 3) + (Local_u8IteratorBuff *2)) , 2) ; //Check_Sum_1 += strTOint(array + 3 + x*2 ,2);
					}
					else
					{
						Local_u16CalculatedChekSum += u32_ConvertStringToIntiger(((Local_u8ArrBuffer + 3) + (Local_u8IteratorBuff *2)) , 1) ; //Check_Sum_1 += strTOint(array + 3 + x*2 ,2);
					}
				}

				if(Local_u16RecivedChekSum != Local_u16CalculatedChekSum)
				{
					Void_ClearArrUart() ;
					//BUZZER_voidHornTimeIT(100);
					void_SendTextToTerminal("ChkSumErr");
					return;
				}
			}

			if (Local_u8Iterator > (int8_t)(Local_u8BufferReciveLenth - 2))
			{
				Void_ClearArrUart() ;
			}
			break;
		}
	}
	/*------------------------------------------------*/
	if (Local_u8Flag == FLAG_IS_HANDLE)
	{
		//time_uart_reset1=0;
		BUZZER_voidHornTimeIT(20);
		//void_validateComunicationFram();
		Local_u8IteratorBuff = 0;
		Local_u8Iterator++;

		if      (u8_CompareTwoStringes((Local_u8ArrBuffer),"eep"))         Local_u8FramRange = 0;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"dat"))         Local_u8FramRange = FRAME_DATE;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"Gda"))         Local_u8FramRange = FRAME_GET_DARA;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer), "req"))        Local_u8FramRange = FRAME_REQUEST;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"Pas"))         Local_u8FramRange = FRAME_PASSWORD;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"tim"))         Local_u8FramRange = FRAME_TIME;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"Int"))         Local_u8FramRange = FRAME_INIT;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"pas"))         Local_u8FramRange = GET_PASSWORD;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"nps"))         Local_u8FramRange = FRAME_UPDATE_USER_PASS;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"VF2"))         Local_u8FramRange = FRAME_VOLTAGE_OFFSET_2;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"VF3"))         Local_u8FramRange = FRAME_VOLTAGE_OFFSET_3;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"Crt"))         Local_u8FramRange = FRAME_CURRENT;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"VG1"))         Local_u8FramRange = FRAME_VOLTAGE_GAIN_1;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"VG2"))         Local_u8FramRange = FRAME_VOLTAGE_GAIN_2;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"VG3"))         Local_u8FramRange = FRAME_VOLTAGE_GAIN_3;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"VGT"))         Local_u8FramRange = FRAME_VOLTAGE_GAIN_TOTAL;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"IF1"))         Local_u8FramRange = FRAME_CURRENT_OFFSET_1;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"IF2"))         Local_u8FramRange = FRAME_CURRENT_OFFSET_2;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"IF3"))         Local_u8FramRange = FRAME_CURRENT_OFFSET_3;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"EnA"))         Local_u8FramRange = FRAME_ACTIVE_ENERGY;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"IG1"))         Local_u8FramRange = FRAME_CURRENT_GAIN_1;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"IG2"))         Local_u8FramRange = FRAME_CURRENT_GAIN_2;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"IG3"))         Local_u8FramRange = FRAME_CURRENT_GAIN_3;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"MBE"))         Local_u8FramRange = FRAME_ENABLE_MESARMENT_FRAM;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"PF1"))         Local_u8FramRange = FRAME_POWER_OFFSET_1;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"PF2"))         Local_u8FramRange = FRAME_POWER_OFFSET_2;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"PF3"))         Local_u8FramRange = FRAME_POWER_OFFSET_3;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"PFT"))         Local_u8FramRange = FRAME_POWER_OFFSET_TOTAL;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"EG1"))         Local_u8FramRange = FRAME_ENERGY_ERROR_1;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"EG2"))         Local_u8FramRange = FRAME_ENERGY_ERROR_2;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"EG3"))         Local_u8FramRange = FRAME_ENERGY_ERROR_3;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"EP1"))         Local_u8FramRange = FRAME_PHASE_ANGLE_1;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"EP3"))         Local_u8FramRange = FRAME_PHASE_ANGLE_3;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"GFr"))         Local_u8FramRange = FRAME_GRID_FREQUENCY;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"RMm"))         Local_u8FramRange = FRAME_READ_ATMEL;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"Wat"))         Local_u8FramRange = FRAME_WRITE_ATMEL;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"Tst"))         Local_u8FramRange = FRAME_TEST;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"EP2"))         Local_u8FramRange = FRAME_PHASE_ANGLE_2;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"Rel"))         Local_u8FramRange = FRAME_RELAY;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"Buz"))         Local_u8FramRange = FRAME_BUZZER;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"PwR"))         Local_u8FramRange = FRAME_REACTIVE_POWER;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"PwP"))         Local_u8FramRange = FRAME_APPERENT_POWER;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"Ver"))         Local_u8FramRange = FRAME_VERSION;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"ICL"))         Local_u8FramRange = FRAME_INITIAL_CALIBRATION;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"RCL"))         Local_u8FramRange = FRAME_READ_CALIBRATION;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"PwA"))         Local_u8FramRange = FRAME_ACTIVE_POWER;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"EnP"))         Local_u8FramRange = FRAME_APPERENT_ENERGY;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"Frq"))         Local_u8FramRange = FRAME_FREQUENCY;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"PFa"))         Local_u8FramRange = FRAME_POWER_FACTOR;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"Mod"))         Local_u8FramRange = FRAME_MODBUS_PARAMETER;
		else if (u8_CompareTwoStringes((Local_u8ArrBuffer),"REL"))         Local_u8FramRange = 20;
		else                                                               Local_u8FramRange = FRAME_INVALID;





		//uint32_t Local_u32DataOrder;
		//		uint32_t Local_u32SecondOrder;
		//
		//		float Local_fData;
		//		uint8_t Local_u8ReturnStatuse;

		void_SendTextToTerminal("[");void_SendNumberToTerminal(Local_u8FramRange);void_SendTextToTerminal("]");

		switch(Local_u8FramRange)
		{
		case 0:
			// Handle reset frame
			EEPROM_voidErase();
			void_SendTextToTerminal("EEPROM is cleard");
			break;
		case FRAME_TIME:
			DS1308_GetCurrentDate();
			//			uint8_t Local_u8Year  = u32_ConvertStringToIntiger(&Local_u8ArrBuffer[3], 2);
			//			uint8_t Local_u8Month = u32_ConvertStringToIntiger(&Local_u8ArrBuffer[5], 2);
			//			uint8_t Local_u8Day   = u32_ConvertStringToIntiger(&Local_u8ArrBuffer[7], 2);

			Hour12_rtc =Local_u8ArrBuffer[3] ; // u32_ConvertStringToIntiger(&Local_u8ArrBuffer[3], 2);
			Minute_rtc =Local_u8ArrBuffer[4];// u32_ConvertStringToIntiger(&Local_u8ArrBuffer[5], 2);
			Second_rtc = Local_u8ArrBuffer[5];//u32_ConvertStringToIntiger(&Local_u8ArrBuffer[7], 2);


			DS1308SetDateAndTime();


			break;

		case FRAME_DATE :
			DS1308_GetCurrentTime();

			Day_rtc       = Local_u8ArrBuffer[3] ;//u32_ConvertStringToIntiger(&Local_u8ArrBuffer[3], 2);
			Month_rtc     = Local_u8ArrBuffer[4]; //u32_ConvertStringToIntiger(&Local_u8ArrBuffer[5], 2);
			Year_rtc      = Local_u8ArrBuffer[5]; // u32_ConvertStringToIntiger(&Local_u8ArrBuffer[7], 2);
			//WeekDays_rtc  = u32_ConvertStringToIntiger(&Local_u8ArrBuffer[8], 1);

			DS1308SetDateAndTime();

			break ;

		case FRAME_GET_DARA :
			void_SendDataCaptioer();

			break ;
		case FRAME_REQUEST :
			//uint8_t Local_u8UserNumber = u32_ConvertStringToIntiger(&Local_u8ArrBuffer[3], 1);
			uint16_t Local_u8DataIndex = (Local_u8ArrBuffer[3] << 8) | Local_u8ArrBuffer[4] ;
            uint8_t Local_u8LenthOfData = Local_u8ArrBuffer[5] ;

			void_SendDataCaptioerForSpesficUser(Local_u8DataIndex,Local_u8LenthOfData);
			BUZZER_voidHornTimeIT(20);
			break;
		case GET_PASSWORD:

			void_SendPasswordsOverBluetooth();
			BUZZER_voidHornTimeIT(20);
			break ;
		case FRAME_UPDATE_USER_PASS:
			uint8_t Local_u8UserNumber2 = Local_u8ArrBuffer[3];
			for(uint8_t i = 0; i < 4; i++)
			{
				Local_u8ArrBuffer[4 + i] += '0' ; // Copy password bytes to the buffer
			}
			void_SaveUserPasswordInEEPROM(Local_u8UserNumber2,&Local_u8ArrBuffer[4]);
			break ;


		}
	}
}


//
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(Hold_TX2==0)		SendUartBufX2();
//}
//void WriteBufText2(const char *s,uint16_t len)
//{
//	Hold_TX2=1;
//	if(len==0) len=strlen((char*)s);
//	for(uint16_t x=0;x<len;x++)
//	{
//		Buf2[CurstorBuf2]=s[x];
//		CurstorBuf2++;
//		if(CurstorBuf2>(Len_Buf-1))	CurstorBuf2=0;
//	}
//	Hold_TX2=0;
//}
//void WriteBufNum2(uint32_t num , uint8_t type)
//{
//	Hold_TX2=1;
//	if(type==0)
//	{
//		Send_Num_Without_Send(num);
//		for(uint16_t x=0;x<flag;x++)
//		{
//			Buf2[CurstorBuf2]=aString[x];
//			CurstorBuf2++;
//			if(CurstorBuf2>(Len_Buf-1))	CurstorBuf2=0;
//		}
//	}
//	else
//	{
//		Buf2[CurstorBuf2]=num;
//		CurstorBuf2++;
//		if(CurstorBuf2>(Len_Buf-1))	CurstorBuf2=0;
//	}
//	Hold_TX2=0;
//}
//void SendUartBuf2()
//{
//	uint16_t len;
//	uint16_t fix,fix2;
//	Hold_TX2=1;
//	if (huart2.gState==HAL_UART_STATE_BUSY_TX)			{}
//	else
//	{
//		if(CurstorBuf2>lastCurstor2)
//		{
//			len=CurstorBuf2-lastCurstor2;
//		}
//		else
//		{
//			fix = Len_Buf-lastCurstor2;
//			fix2 = fix + CurstorBuf2 + 5;
//			for(uint16_t x=0;x<CurstorBuf2;x++)			Buf2[fix2+x] = Buf2[x];
//			for(uint16_t x=0;x<CurstorBuf2;x++)			Buf2[x+fix]  = Buf2[fix2+x];
//			for(uint16_t x=0;x<fix;x++)							{Buf2[x]     = Buf2[x+lastCurstor2];}
//			lastCurstor2=0;
//			len = CurstorBuf2+fix;
//			CurstorBuf2+=fix;
//		}
//		HAL_UART_Transmit_IT(&huart2,Buf2+lastCurstor2,len);
//		lastCurstor2=CurstorBuf2;
//	}
//	Hold_TX2=0;
//}
//void SendUartBufX2()
//{
//	uint16_t len;
//	uint16_t fix,fix2;
//	if(CurstorBuf2==lastCurstor2)			{return;}
//	if (huart2.gState==HAL_UART_STATE_BUSY_TX)			{}
//	else
//	{
//		if(CurstorBuf2>lastCurstor2)
//		{
//			len=CurstorBuf2-lastCurstor2;
//		}
//		else
//		{
//			fix = Len_Buf-lastCurstor2;
//			fix2 = fix + CurstorBuf2 + 5;
//			for(uint16_t x=0;x<CurstorBuf2;x++)			Buf2[fix2+x] = Buf2[x];
//			for(uint16_t x=0;x<CurstorBuf2;x++)			Buf2[x+fix]  = Buf2[fix2+x];
//			for(uint16_t x=0;x<fix;x++)					{Buf2[x]     = Buf2[x+lastCurstor2];}
//			lastCurstor2=0;
//			len = CurstorBuf2+fix;
//			CurstorBuf2+=fix;
//		}
//		HAL_UART_Transmit_IT(&huart2,Buf2+lastCurstor2,len);
//		lastCurstor2=CurstorBuf2;
//	}
//}

//void Send_Text(const char * str)
//{
//	WriteBufText2(str,0);
//	SendUartBuf2();
//}

static void Void_ClearArrUart(void)
{
	huart2.RxXferCount = UART_RX_SIZE_BUFFER;
	huart2.RxXferSize  = UART_RX_SIZE_BUFFER;
	huart2.pRxBuffPtr = &Global_u8ReciveArrBuff[0];
	//Count_UART = UART_RX_SIZE_BUFFER;
	//Send_Text_Show("u1");
}

static uint32_t u32_ConvertStringToIntiger(uint8_t * Copy_pu8StartBuffer,uint8_t Copy_u8Lenght)
{
	uint32_t Local_u32Value = 0;

	// Switch-case ladder to handle conversion for up to 10 digits (max for uint32_t)
	switch(Copy_u8Lenght)
	{
	case 10: Local_u32Value += (uint32_t)((*(Copy_pu8StartBuffer + Copy_u8Lenght - 10)) - 48) * 1000000000;
	case 9:  Local_u32Value += (uint32_t)((*(Copy_pu8StartBuffer + Copy_u8Lenght - 9))  - 48) * 100000000;
	case 8:  Local_u32Value += (uint32_t)((*(Copy_pu8StartBuffer + Copy_u8Lenght - 8))  - 48) * 10000000;
	case 7:  Local_u32Value += (uint32_t)((*(Copy_pu8StartBuffer + Copy_u8Lenght - 7))  - 48) * 1000000;
	case 6:  Local_u32Value += (uint32_t)((*(Copy_pu8StartBuffer + Copy_u8Lenght - 6))  - 48) * 100000;
	case 5:  Local_u32Value += (uint32_t)((*(Copy_pu8StartBuffer + Copy_u8Lenght - 5))  - 48) * 10000;
	case 4:  Local_u32Value += (uint32_t)((*(Copy_pu8StartBuffer + Copy_u8Lenght - 4))  - 48) * 1000;
	case 3:  Local_u32Value += (uint32_t)((*(Copy_pu8StartBuffer + Copy_u8Lenght - 3))  - 48) * 100;
	case 2:  Local_u32Value += (uint32_t)((*(Copy_pu8StartBuffer + Copy_u8Lenght - 2))  - 48) * 10;
	case 1:  Local_u32Value += *(Copy_pu8StartBuffer + Copy_u8Lenght - 1) - 48;
	// No default: if length is 0, returns 0
	}
	return Local_u32Value;
}

/**
 * @brief Compares two strings for equality.
 *
 * Compares the first n characters of s11 and s22, where n is the length of s22.
 * Returns 1 if all characters match, 0 otherwise.
 *
 * @param s11 Pointer to the first string (uint8_t array, not necessarily null-terminated).
 * @param s22 Pointer to the second string (char array, must be null-terminated).
 * @return uint8_t 1 if strings are equal for n characters, 0 otherwise.
 *
 * @note Only compares up to the length of s22. Does not check for s11 length.
 */
static uint8_t u8_CompareTwoStringes(uint8_t * s11, char * s22)
{
	uint8_t x = 0;
	uint8_t n = strlen(s22); // Get length of s22

	// Compare each character up to n
	while (x < n)
	{
		if (s11[x] != (uint8_t)s22[x])
		{
			// Mismatch found, strings are not equal
			return 0;
		}
		else
		{
			x++;
		}
	}
	// All characters matched
	return 1;
}

/**
 * @brief Sends a null-terminated string to the terminal via UART.
 *
 * Uses HAL_UART_Transmit to send the string over UART2.
 *
 * @param Copy_pcString Pointer to the null-terminated string to send.
 *
 * @note The function blocks until the transmission is complete or timeout (500ms).
 * @note Uncomment the IT version for non-blocking transmission if needed.
 */
static void void_SendTextToTerminal(const char *Copy_pcString)
{
	// Non-blocking version (interrupt-based) - uncomment if needed
	// HAL_UART_Transmit_IT(&huart2, (uint8_t*)Copy_pcString, strlen(Copy_pcString));

	// Blocking version (waits up to 500ms)
	HAL_UART_Transmit(&huart2, (uint8_t*)Copy_pcString, strlen(Copy_pcString), 500);
}

void void_SendDataOverBlutothWithLength(uint8_t *Copy_pcString,uint8_t Copy_u8Length)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)Copy_pcString,Copy_u8Length, 500);
}

void Send_Show(const char* Text,uint32_t Num)
{
	void_SendTextToTerminal("[");void_SendTextToTerminal(Text);void_SendTextToTerminal("]=");Send_Num(Num);
}

/**
 * @brief Executes various reset operations on the device and energy metering IC.
 *
 * This function handles different types of reset commands based on the input parameter:
 *   - STM32 MCU software reset
 *   - ATM90E32 (energy metering IC) software reset
 *   - ATM90E32 hardware reset
 *   - Resetting all accumulated energy data
 *
 * It provides user feedback via the terminal and buzzer for error or unrecognized commands.
 *
 * @param Copy_u8ResetParameter The reset command type. Must be one of:
 *        - STM_REST:         Triggers a software reset of the STM32 MCU.
 *        - ATMEL_SOFT_REST:  Triggers a software reset of the ATM90E32 IC.
 *        - ATMEL_HARD_REST:  Triggers a hardware reset of the ATM90E32 IC.
 *        - ALL_ENERGY_REST:  Clears all accumulated energy data.
 *        - Any other value:  Triggers an error response.
 */

/**
 * @brief Switches the display to a specified screen number, with validation and feedback.
 *
 * This function validates the requested display ID and updates the display accordingly.
 * It also updates button press counters and turns off all LEDs as part of the screen switch.
 * If the display ID is invalid (>= 25), it signals an error via buzzer and terminal.
 *
 * @param Copy_u8DisplayID The requested display screen number (valid range: 0-24).
 */

/**
 * @brief  Updates the current transfer ratio based on the given value.
 * @param  Copy_u16CTR: The new transfer ratio value (typically 0 - 65535).
 * @retval None
 */

/**
 * @brief  Updates the potential transfer ratio (PTR) and stores it in EEPROM.
 * @note   The function validates the input PTR, writes it to EEPROM, and verifies the write operation.
 *         It provides feedback via the terminal and a buzzer for error conditions.
 * @param  Copy_u16PTR: The new potential transfer ratio value. Valid range: 1 to 9998.
 * @retval None




 * @brief  Updates the system password and stores it in EEPROM.
 * @note   This function validates the new password, writes it to EEPROM, and verifies the write.
 *         It provides feedback via the terminal and a buzzer for error conditions.
 * @param  Copy_u16passwordValue: The new password value. Valid range: 0 to 9999.
 * @retval None
 */

/**
 * @brief Updates the last action timestamps for display or data update events and stores them in EEPROM.
 *
 * This function updates the global timestamp variables for either display refresh or data update events,
 * writes the new value to EEPROM, and verifies the write operation. It provides user feedback via the terminal
 * and triggers a buzzer alarm in case of errors.
 *
 * @param Copy_u8TypeTimeUpdate   Type of time update to perform:
 *                                - DISPLAY_DATA_TIME: Update display refresh timestamp.
 *                                - UPDATE_DATA_TIME:  Update data update timestamp.
 * @param Copy_u16CurrentTimeMs   The new timestamp value in milliseconds (must be >0 and <0xFFFF).
 *
 * @note If the time value is invalid or the type is unknown, an error is signaled.
 * @note EEPROM write is verified by reading back the value.
 * @retval None
 */

/**
 * @brief Adjusts the brightness of specified display components and saves settings to EEPROM
 *
 * This function handles brightness adjustment for different display types:
 * - LED array brightness control
 * - 7-segment display brightness control (SSD1, SSD2, SSD3)
 *
 * For each display type, the function:
 * 1. Applies the brightness setting to the hardware
 * 2. Saves the setting to EEPROM for persistence
 * 3. Verifies the EEPROM write operation
 * 4. Provides user feedback via terminal messages
 * 5. Triggers error indication (buzzer) if EEPROM operations fail
 *
 * @param Copy_u8DisplayNumber Target display identifier:
 *                            - LEDS: LED array
 *                            - SIVEN_SIGMANT_DISPLAY_1: First 7-segment display
 *                            - SIVEN_SIGMANT_DISPLAY_2: Second 7-segment display
 *                            - SIVEN_SIGMANT_DISPLAY_3: Third 7-segment display
 * @param Copy_u8BrightnessValue Brightness level to apply (0-255 range)
 *
 * @note EEPROM addresses used:
 *       - LEDS_BRIGHTNESS_LEVEL: LED brightness storage
 *       - SSD1_BRIGHTNESS_LEVEL: SSD1 brightness storage
 *       - SSD2_BRIGHTNESS_LEVEL: SSD2 brightness storage
 *       - SSD3_BRIGHTNESS_LEVEL: SSD3 brightness storage
 *
 * @note Function triggers buzzer alarm (100ms) on EEPROM write failures
 * @note All operations are logged to terminal for debugging and user feedback
 *Safe conversion with proper format specifier matching the data type
 */
void void_SendNumberToTerminal(uint32_t Copy_u32Number)
{
	/* Buffer to hold the converted number as a string (20 bytes sufficient for uint32_t max) */
	char buffer[20];

	/* Convert number to string using snprintf with proper format specifier */
	snprintf(buffer, sizeof(buffer), "%lu", (unsigned long)Copy_u32Number);

	/* Send the converted string to terminal */
	void_SendTextToTerminal(buffer);
}
/*

static void void_SendFloatToTerminal(float Copy_f32Number)
{
	char buffer[20];
	int32_t intPart;
	uint32_t fracPart;
	uint32_t multiplier = 1;
	uint8_t i;

	// Handle negative numbers
	if (Copy_f32Number < 0.0f)
	{
		void_SendTextToTerminal("-");
		Copy_f32Number = -Copy_f32Number;
	}

	// Extract integer part
	intPart = (int32_t)Copy_f32Number;

	// Send integer part
	snprintf(buffer, sizeof(buffer), "%ld", (long)intPart);
	void_SendTextToTerminal(buffer);

	// Send dot
	void_SendTextToTerminal(".");

	// Prepare multiplier for fractional part
	for (i = 0; i < 3; i++)
		multiplier *= 10;

	// Extract fractional part
	fracPart = (uint32_t)((Copy_f32Number - intPart) * multiplier + 0.5f);  // +0.5f for rounding

	// Send fractional part
	snprintf(buffer, sizeof(buffer), "%0*u", 3, (unsigned int)fracPart);  // Add leading zeros if needed
	void_SendTextToTerminal(buffer);
}
 */

/**
 * @brief Performs a comprehensive display test for LEDs and SSDs
 *
 * This function tests the display system by:
 * 1. Cycling through all 25 LEDs one by one
 * 2. Clearing all SSDs between each LED test
 * 3. Displaying different test patterns on SSDs based on LED position
 * 4. Adding a delay between each test cycle for visual verification
 *
 * @note The test runs for 25 iterations (one for each LED)
 * @note SSD patterns cycle through: "1234", "5678", "9PNL"
 * @note Each test cycle has a 500ms delay for visual observation
 * @note All LEDs are turned off and SSDs are cleared at the start of each cycle
 */



/**
 * @brief Performs a comprehensive EEPROM integrity test
 *
 * This function tests the EEPROM memory by:
 * 1. Reading the current data at each address
 * 2. Writing a test pattern (0xAAAA) to each address
 * 3. Reading back the data to verify it was written correctly
 * 4. Restoring the original data
 * 5. Reporting any errors found during the test
 *
 * @note This test covers addresses 0 to 895 (896 half-words total)
 * @note The test pattern 0xAAAA is used to detect stuck bits and data corruption
 * @note Original data is preserved by restoring it after each test
 */

//static void void_sendEepromError(void)
//{
//	void_SendTextToTerminal("EEPROM_Err");
//}
//static void void_sendFramError(void)
//{
//	void_SendTextToTerminal("Fram_Err");
//}
//
//static void void_SendErrorMessage(void)
//{
//	void_SendTextToTerminal("ERR");
//}

