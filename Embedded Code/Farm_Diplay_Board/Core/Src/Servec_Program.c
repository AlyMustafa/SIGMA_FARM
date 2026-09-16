/*
 ******************************************************************************
 * @file           : Servec_Program.c
 * @brief          : [Enter a brief description of the file here]
 ******************************************************************************
 * @attention
 *
 * @author   : Mohamed Ahmed
 * @date     : Jul 14, 2026
 * @copyright:
 *   © 2026 M0hamed Ahmed . All rights reserved.
 *
 *   This file is part of the [Project or System Name] project.
 *   Unauthorized copying, modification, or distribution of this file,
 *   via any medium, is strictly prohibited.
 *
 ******************************************************************************
 */

#include "string.h"

#include "main.h"
#include "stdint.h"
#include "Service_Interface.h"
#include "DS1308_Interface.h"
#include "EEPROM_Interface.h"


#include "Bloutoth_Interface.h"


extern TIM_HandleTypeDef htim17;

extern UART_HandleTypeDef huart1;

uint8_t g = 0 ;

uint8_t KL_u8Flag = 0;

uint8_t G_u8ComingFlag = 0;

uint8_t G_u8FlagState =  0;


extern uint8_t Global_u8UserNumber ;

extern uint16_t	Data_Clock[10];

extern TIM_HandleTypeDef htim6;

Time_t NewTime,LastTime,ElapsedTime;

Time_t G_TimeElapsedForSpecficUser ;
/**
 * @brief Calculates the elapsed time between two time points.
 * @param Copy_pStartTime Pointer to the start time structure.
 * @param Coy_pCurrentTime Pointer to the current time structure.
 * @return ElapsedTime_t structure containing the elapsed hours, minutes, and seconds.
 */
Time_t Time_GetElapsed(Time_t*  Copy_pStartTime, Time_t* Coy_pCurrentTime)
{
	Time_t Elapsed;
	uint32_t Local_u32Diff;

	uint32_t StartSeconds =
			(Copy_pStartTime->Hours * 3600UL) +
			(Copy_pStartTime->Minutes * 60UL)
			;

	uint32_t CurrentSeconds =
			(Coy_pCurrentTime->Hours * 3600UL) +
			(Coy_pCurrentTime->Minutes * 60UL) ;

	Local_u32Diff = CurrentSeconds - StartSeconds;

	Elapsed.Hours   = Local_u32Diff / 3600UL;
	Local_u32Diff %= 3600UL;

	Elapsed.Minutes = Local_u32Diff / 60UL;


	return Elapsed;
}



Time_t Time_Add(Time_t* Copy_pTime1, Time_t* Copy_pTime2)
{
	//	Time_t Result;
	//
	//	Result.Minutes = Copy_pTime1->Minutes + Copy_pTime2->Minutes;
	//	Result.Hours   = Copy_pTime1->Hours   + Copy_pTime2->Hours;
	//
	//	/* Carry Minutes */
	//	if(Result.Minutes >= 60)
	//	{
	//		Result.Minutes -= 60;
	//		Result.Hours++;
	//	}
	//
	//
	//
	//	return Result;


	Time_t Elapsed;
	uint32_t Local_u32Diff;

	uint32_t StartSeconds =
			(Copy_pTime1->Hours * 3600UL) +
			(Copy_pTime1->Minutes * 60UL)
			;

	uint32_t CurrentSeconds =
			(Copy_pTime2->Hours * 3600UL) +
			(Copy_pTime2->Minutes * 60UL) ;

	Local_u32Diff = CurrentSeconds + StartSeconds;

	Elapsed.Hours   = Local_u32Diff / 3600UL;
	Local_u32Diff %= 3600UL;

	Elapsed.Minutes = Local_u32Diff / 60UL;


	return Elapsed;
}



#include <stdint.h>

static const uint16_t DaysBeforeMonth[] =
{
		0,    // January
		31,   // February
		59,   // March
		90,   // April
		120,  // May
		151,  // June
		181,  // July
		212,  // August
		243,  // September
		273,  // October
		304,  // November
		334   // December
};

/*
 * ترجع عدد الأيام بين تاريخين في نفس السنة
 * StartDay   : يوم البداية
 * StartMonth : شهر البداية (1-12)
 * CurrDay    : اليوم الحالي
 * CurrMonth  : الشهر الحالي (1-12)
 */
uint16_t Date_GetElapsedDays(uint8_t StartDay,
		uint8_t StartMonth,
		uint8_t CurrDay,
		uint8_t CurrMonth)
{
	uint16_t StartDOY;
	uint16_t CurrDOY;

	StartDOY = DaysBeforeMonth[StartMonth - 1] + StartDay;
	CurrDOY  = DaysBeforeMonth[CurrMonth - 1] + CurrDay;

	return (CurrDOY - StartDOY);
}


void void_HandleComingFlag(void) {
	uint8_t Local_u8CurrentUserNumber ;
	//uint8_t Local_u8PriviousUserNumber ;
	uint8_t Local_u8LastPostion ;
	uint8_t Local_u8BuffArr[15];
	uint8_t Local_u8NewBuffArr[15];

	if (G_u8ComingFlag == 1)
	{
		//EEPROM_voidWriteBuffer( (uint8_t*) &Global_u8UserNumber,ACTIVE_USER_NUMBER_ADDRESS, 2);
		if(G_u8FlagState == 1)
		{


			EEPROM_voidReadBuffer((uint8_t *)&Local_u8CurrentUserNumber ,CURRENT_USER_NUMBER_ADDRESS, 1) ;


			// HANDILE CALCOLATING NUMBER AND EEPROM WRITING

			if ((Local_u8CurrentUserNumber == Global_u8UserNumber)&& (Global_u8UserNumber != 0))
			{
				// Same user, do nothing
				EEPROM_voidReadBuffer(&Local_u8LastPostion, LAST_POSITION_WEITING_ADDRESS, 1);
				for(uint8_t Local_u8Iterator = 0 ; Local_u8Iterator <8 ;Local_u8Iterator ++  )
				{
					if((Local_u8LastPostion - Local_u8Iterator) < 0 )
					{
						KL_u8Flag = 1 ;
						break ;
					}
					else
					{
						EEPROM_voidReadBuffer(Local_u8BuffArr, (Local_u8LastPostion - Local_u8Iterator + 1) * 15 , (Local_u8LastPostion - Local_u8Iterator));
						if ((Local_u8BuffArr[ALUSER] == Global_u8UserNumber) && (Local_u8BuffArr[ALYOM] == Day_rtc)  )
						{
							// Found the same user in the last 8 entries, do nothing

							LastTime.Hours = Local_u8BuffArr[ALHOUR_ALAKER];
							LastTime.Minutes = Local_u8BuffArr[ALMINUTE_ALAKER];
							NewTime.Hours = Hour12_rtc;
							NewTime.Minutes = Minute_rtc;

							ElapsedTime = Time_GetElapsed(&LastTime, &NewTime);
							NewTime.Hours = Local_u8BuffArr[ALHOUR];
							NewTime.Minutes = Local_u8BuffArr[ALMINUTE];

							ElapsedTime = Time_Add(&ElapsedTime, &NewTime);

							Local_u8NewBuffArr[ALUSER] = Global_u8UserNumber;
							Local_u8NewBuffArr[ALYOM] = Day_rtc;
							Local_u8NewBuffArr[ALSHAHR] = Month_rtc;
							Local_u8NewBuffArr[ALHOUR] = ElapsedTime.Hours;
							Local_u8NewBuffArr[ALMINUTE] = ElapsedTime.Minutes;
							Local_u8NewBuffArr[ALHOUR_ALAKER] = Hour12_rtc;
							Local_u8NewBuffArr[ALMINUTE_ALAKER] = Minute_rtc;


							EEPROM_voidWriteBuffer(Local_u8NewBuffArr, (Local_u8LastPostion - Local_u8Iterator + 1) * 15, 15);

							EEPROM_voidWriteBuffer(&Local_u8LastPostion, LAST_POSITION_WEITING_ADDRESS, 1); //  هتتعدل

							EEPROM_voidWriteBuffer((uint8_t*)"\0", RUNING_FLAG_ADDRESS, 1);

							EEPROM_voidWriteBuffer(&Global_u8UserNumber, CURRENT_USER_NUMBER_ADDRESS, 1);
							break;




						} else if (Local_u8Iterator == 7)
						{
							// Not found in the last 8 entries, write new entry

							KL_u8Flag = 1 ;
							break ;

							//						Local_u8LastPostion++;
							//						if (Local_u8LastPostion > 7)
							//						{
							//							Local_u8LastPostion = 0;
							//						}
							//						EEPROM_voidWriteBuffer(&Local_u8LastPostion, LAST_POSITION_WEITING_ADDRESS, 1);
							//
							//						Local_u8NewBuffArr[ALUSER] = Global_u8UserNumber;
							//						Local_u8NewBuffArr[ALYOM] = Day_rtc;
							//						Local_u8NewBuffArr[ALSHAHR] = Month_rtc;
							//						Local_u8NewBuffArr[ALHOUR] = Hour12_rtc;
							//						Local_u8NewBuffArr[ALMINUTE] = Minute_rtc;
							//						Local_u8NewBuffArr[ALHOUR_ALAKER] = Hour12_rtc;
							//						Local_u8NewBuffArr[ALMINUTE_ALAKER] = Minute_rtc;
							//
							//						EEPROM_voidWriteBuffer(Local_u8NewBuffArr, Local_u8LastPostion * 15, 15);
						}

					}
				}

				if(KL_u8Flag == 1)
				{
					Local_u8LastPostion++;
					//				if (Local_u8LastPostion > 7)
					//				{
					//					Local_u8LastPostion = 0;
					//				}
					EEPROM_voidWriteBuffer(&Local_u8LastPostion, LAST_POSITION_WEITING_ADDRESS, 1);

					Local_u8NewBuffArr[ALUSER] = Global_u8UserNumber;
					Local_u8NewBuffArr[ALYOM] = Day_rtc;
					Local_u8NewBuffArr[ALSHAHR] = Month_rtc;
					Local_u8NewBuffArr[ALHOUR] = 0;
					Local_u8NewBuffArr[ALMINUTE] = 0;
					Local_u8NewBuffArr[ALHOUR_ALAKER] = Hour12_rtc;
					Local_u8NewBuffArr[ALMINUTE_ALAKER] = Minute_rtc;



					EEPROM_voidWriteBuffer(Local_u8NewBuffArr, (Local_u8LastPostion+2) * 15, 15);}


				KL_u8Flag = 0 ;


			}

			//end of HANDILE CALCOLATING NUMBER AND EEPROM WRITING

			EEPROM_voidWriteBuffer( (uint8_t*) &Global_u8UserNumber,CURRENT_USER_NUMBER_ADDRESS, 1);




			HAL_TIM_Base_Start_IT(&htim6);

		}
		else if (G_u8FlagState == 2)
		{

			//EEPROM_voidReadBuffer(&Local_u8CurrentStat ,RUNING_FLAG_ADDRESS, 1);
			HAL_TIM_Base_Stop_IT(&htim6);
		}



		G_u8ComingFlag = 0 ;
		G_u8FlagState =0;
	}
	else
	{
		G_u8ComingFlag =0 ;

	}
}


/*
 *
 */
void void_HandleEEpromData(void)
{
	uint8_t Local_u8UserFlag ;
	uint8_t Local_u8DayNumber ;
	uint16_t Local_u16CurrentPositionToWrite ;
	uint8_t Local_u8BuffArr[10] ;
	uint8_t Local_u8RuningUserNumber ;


	uint8_t G_u8flagAddress ;





	if(G_u8ComingFlag == 1)
	{
		if(G_u8FlagState == 1)
		{
			HAL_TIM_Base_Start_IT(&htim6);



			// handele number of day

			EEPROM_voidReadBuffer(&G_u8flagAddress, DAY_FLAG, 1);

			DS1308_GetCurrentDate();

			if(G_u8flagAddress == 0 )
			{
				uint8_t ll = 1 ;
				EEPROM_voidWriteBuffer((uint8_t*)&ll, DAY_FLAG, 1);
				EEPROM_voidWriteBuffer((uint8_t*)&Day_rtc ,START_LOGING_DAY_ADDRESS, 1);
				EEPROM_voidWriteBuffer((uint8_t*)&Month_rtc ,START_LOGING_MONTH_ADDRESS, 1);


			}
			else if(G_u8flagAddress == 1)
			{
				uint8_t Local_u8StartDay ;
				uint8_t Local_u8StartMonth ;
				uint8_t Local_DayRunning ;

				EEPROM_voidReadBuffer(&Local_u8StartDay  , START_LOGING_DAY_ADDRESS  , 1);
				EEPROM_voidReadBuffer(&Local_u8StartMonth, START_LOGING_MONTH_ADDRESS, 1);

				Local_DayRunning= (uint8_t)Date_GetElapsedDays(Local_u8StartDay, Local_u8StartMonth, (uint8_t)Day_rtc, Month_rtc);

				//				if(Local_DayRunning > 45 )
				//				{
				//					Local_DayRunning -= 46;
				//				}
				(Local_DayRunning > 45) ? (Local_DayRunning -= 46) : 0;

				EEPROM_voidWriteBuffer(&Local_DayRunning, 9, 1);





			}

			//end handling

			EEPROM_voidReadBuffer((uint8_t *)&Local_u8UserFlag ,(Global_u8UserNumber-1), 1) ;
			EEPROM_voidReadBuffer((uint8_t *)&Local_u8DayNumber ,9, 1) ; //day number of runing
			//Local_u16CurrentPositionTOWrite = (Global_u8UserNumber+1)*10 + Local_u8DayNumber ;
			EEPROM_voidReadBuffer((uint8_t *)&Local_u8RuningUserNumber ,RUNINING_USER_ADDRESS, 1) ; //read runing user number
			Local_u16CurrentPositionToWrite = (48u* Local_u8DayNumber) + ((Global_u8UserNumber-1) * 6u)+ 10u ; //current position to write in eeprom calcolated by date and user running

			EEPROM_voidReadBuffer(Local_u8BuffArr, Local_u16CurrentPositionToWrite, 6) ; //read last data of current user and day

			if ((Local_u8UserFlag == 1) && (Local_u8RuningUserNumber == Global_u8UserNumber)) //if user flag is 1 and runing user number is equal to current user number)
			{



				LastTime.Hours   = Local_u8BuffArr[LAST_HOUR_ADDRESS  ];
				LastTime.Minutes = Local_u8BuffArr[LAST_MINUTE_ADDRESS];
				NewTime.Hours   = Hour12_rtc;
				NewTime.Minutes = Minute_rtc;

				ElapsedTime = Time_GetElapsed(&LastTime, &NewTime);

				NewTime.Hours   = Local_u8BuffArr[HOURS_ELAPSED_ADDRESS  ];
				NewTime.Minutes = Local_u8BuffArr[MINUTES_ELAPSED_ADDRESS];

				ElapsedTime = Time_Add(&ElapsedTime, &NewTime);


				Local_u8BuffArr[LAST_HOUR_ADDRESS      ] = Hour12_rtc;
				Local_u8BuffArr[LAST_MINUTE_ADDRESS    ] = Minute_rtc;
				Local_u8BuffArr[HOURS_ELAPSED_ADDRESS  ] = ElapsedTime.Hours;
				Local_u8BuffArr[MINUTES_ELAPSED_ADDRESS] = ElapsedTime.Minutes;
				Local_u8BuffArr[CURRENT_MONTH_ADDRESS  ] = Month_rtc;
				Local_u8BuffArr[CURRENT_DAY_ADDRESS    ] = Day_rtc;

				EEPROM_voidWriteBuffer(Local_u8BuffArr, Local_u16CurrentPositionToWrite, 6) ; //write last data of current user and day


				G_TimeElapsedForSpecficUser.Hours   = ElapsedTime.Hours;
				G_TimeElapsedForSpecficUser.Minutes = ElapsedTime.Minutes ;


			}
			else
			{
				if (memcmp(Local_u8BuffArr, (uint8_t*) "\0\0\0\0\0\0", 6)== 0) //if current user data is empty
				{
					Local_u8BuffArr[LAST_HOUR_ADDRESS] = Hour12_rtc;
					Local_u8BuffArr[LAST_MINUTE_ADDRESS ] = Minute_rtc;
					Local_u8BuffArr[HOURS_ELAPSED_ADDRESS ] = 0;
					Local_u8BuffArr[MINUTES_ELAPSED_ADDRESS] = 0;
					Local_u8BuffArr[CURRENT_MONTH_ADDRESS ] = Month_rtc;
					Local_u8BuffArr[CURRENT_DAY_ADDRESS ] = Day_rtc;

					G_TimeElapsedForSpecficUser.Hours   = 0;
					G_TimeElapsedForSpecficUser.Minutes = 0 ;

					//EEPROM_voidWriteBuffer(Local_u8BuffArr,Local_u16CurrentPositionToWrite, 6) ; //write last data of current user and day
				}
				else
				{
					Local_u8BuffArr[LAST_HOUR_ADDRESS] = Hour12_rtc;
					Local_u8BuffArr[LAST_MINUTE_ADDRESS ] = Minute_rtc;
					Local_u8BuffArr[CURRENT_MONTH_ADDRESS ] = Month_rtc;
					Local_u8BuffArr[CURRENT_DAY_ADDRESS ] = Day_rtc;

					G_TimeElapsedForSpecficUser.Hours   = Local_u8BuffArr[HOURS_ELAPSED_ADDRESS];
					G_TimeElapsedForSpecficUser.Minutes = Local_u8BuffArr[MINUTES_ELAPSED_ADDRESS] ;

					// EEPROM_voidWriteBuffer(Local_u8BuffArr,Local_u16CurrentPositionToWrite, 6) ; //write last data of current user and day
				}
				uint8_t Local_u8Flag = 1 ;
				EEPROM_voidWriteBuffer((uint8_t *)"\0\0\0\0\0\0",0, 6) ;
				EEPROM_voidWriteBuffer(&Local_u8Flag, (Global_u8UserNumber - 1), 1);
				EEPROM_voidWriteBuffer(Local_u8BuffArr,Local_u16CurrentPositionToWrite, 6) ; //write last data of current user and day
				EEPROM_voidWriteBuffer(&Global_u8UserNumber,RUNINING_USER_ADDRESS, 1); //write runing user number

			}

		}



	}
	else if (G_u8FlagState == 2)
	{
		EEPROM_voidWriteBuffer((uint8_t *)"\0\0\0\0\0\0\0",0, 7) ;
		HAL_TIM_Base_Stop_IT(&htim6);

	}

	G_u8ComingFlag = 0 ;
	G_u8FlagState =0;
}

/*****************************************************************************/

void void_HandleDataFarmUsers(void)
{

	uint8_t Local_u8UserFlag ;
	//uint8_t Local_u8DayNumber ;

	uint16_t Local_u16CurrentPositionToWrite ;
	uint8_t Local_u8BuffArr[10] ;
	//uint8_t Local_u8RuningUserNumber ;

	uint16_t Local_u16CurrentPositionToRead ;


	//uint8_t G_u8flagAddress ;

	if(G_u8ComingFlag == 1)
	{
		if(G_u8FlagState == 1)
		{
			HAL_TIM_Base_Start_IT(&htim6);

			EEPROM_voidReadBuffer((uint8_t *)&Local_u8UserFlag ,CURRENT_USER_NUMBER_ADDRESS, 1) ;// read current user flag from EEPROM

			EEPROM_voidReadBuffer(Local_u8BuffArr ,CURRENT_TO_WRITE_ADDRESS, 2) ;// read current user number and day number from EEPROM

			Local_u16CurrentPositionToRead = (Local_u8BuffArr[0] << 8 )| Local_u8BuffArr[1] ; //current position to write in eeprom calcolated by date and user running
			DS1308_GetCurrentDate();
			Local_u16CurrentPositionToWrite =  ( Local_u16CurrentPositionToRead* 7u)+ 10u ;

			if((Local_u8UserFlag == Global_u8UserNumber) && (Global_u8UserNumber != 0) &&(g == Global_u8UserNumber)/*(Local_u8BuffArr[USER_NUMBER_ADDRESS] == Global_u8UserNumber)*/) //if user flag is 1 and runing user number is equal to current user number )
			{
				EEPROM_voidReadBuffer(Local_u8BuffArr, Local_u16CurrentPositionToWrite, 7) ; //read last data of current user and day) ;



				LastTime.Hours   = Local_u8BuffArr[LAST_HOUR_ADDRESS  ];
				LastTime.Minutes = Local_u8BuffArr[LAST_MINUTE_ADDRESS];
				NewTime.Hours   = Hour12_rtc;
				NewTime.Minutes = Minute_rtc;

				ElapsedTime = Time_GetElapsed(&LastTime, &NewTime);
				//
				//				NewTime.Hours   = Local_u8BuffArr[HOURS_ELAPSED_ADDRESS  ];
				//				NewTime.Minutes = Local_u8BuffArr[MINUTES_ELAPSED_ADDRESS];
				//
				//				ElapsedTime = Time_Add(&ElapsedTime, &NewTime);


				Local_u8BuffArr[HOURS_ELAPSED_ADDRESS  ] = ElapsedTime.Hours;
				Local_u8BuffArr[MINUTES_ELAPSED_ADDRESS] = ElapsedTime.Minutes;
				//				Local_u8BuffArr[CURRENT_MONTH_ADDRESS  ] = Month_rtc;
				//				Local_u8BuffArr[CURRENT_DAY_ADDRESS    ] = Day_rtc;
				//Local_u8BuffArr[USER_NUMBER_ADDRESS    ] = Global_u8UserNumber ;


				EEPROM_voidWriteBuffer(&(Local_u8BuffArr[HOURS_ELAPSED_ADDRESS]), Local_u16CurrentPositionToWrite + 4, 2) ; //write last data of current user and day


				G_TimeElapsedForSpecficUser.Hours   = ElapsedTime.Hours;
				G_TimeElapsedForSpecficUser.Minutes = ElapsedTime.Minutes ;

			}
			else
			{
				//uint8_t Local_u8Flag = 1;
				//EEPROM_voidWriteBuffer((uint8_t*) "\0\0\0\0\0\0", 0, 7);
				EEPROM_voidWriteBuffer(&Global_u8UserNumber, CURRENT_USER_NUMBER_ADDRESS, 1);


				++Local_u16CurrentPositionToRead ; //increment the current position to read for the next user entry

				Local_u8BuffArr[0] = Local_u16CurrentPositionToRead >> 8;// Store the high byte of the current position to read
				Local_u8BuffArr[1] = Local_u16CurrentPositionToRead & 0xFF;// Store the low byte of the current position to read

				EEPROM_voidWriteBuffer(Local_u8BuffArr, CURRENT_TO_WRITE_ADDRESS, 2); // Store the updated current position to read in EEPROM


				Local_u8BuffArr[LAST_HOUR_ADDRESS] = Hour12_rtc;
				Local_u8BuffArr[LAST_MINUTE_ADDRESS] = Minute_rtc;
				Local_u8BuffArr[HOURS_ELAPSED_ADDRESS] = 0;
				Local_u8BuffArr[MINUTES_ELAPSED_ADDRESS] = 0;
				Local_u8BuffArr[CURRENT_MONTH_ADDRESS] = Month_rtc;
				Local_u8BuffArr[CURRENT_DAY_ADDRESS] = Day_rtc;
				Local_u8BuffArr[USER_NUMBER_ADDRESS] = Global_u8UserNumber;
				g = Global_u8UserNumber ;
				G_TimeElapsedForSpecficUser.Hours = 0;
				G_TimeElapsedForSpecficUser.Minutes = 0;

				Local_u16CurrentPositionToWrite = (Local_u16CurrentPositionToRead * 7u) + 10u; // Calculate the position to write in EEPROM based on the updated current position to read

				EEPROM_voidWriteBuffer(Local_u8BuffArr,Local_u16CurrentPositionToWrite, 7); //write last data of current user and day
			}



		}
		else if (G_u8FlagState == 2) {
			EEPROM_voidReadBuffer((uint8_t *)&Local_u8UserFlag ,CURRENT_USER_NUMBER_ADDRESS, 1) ;// read current user flag from EEPROM

			EEPROM_voidReadBuffer(Local_u8BuffArr ,CURRENT_TO_WRITE_ADDRESS, 2) ;// read current user number and day number from EEPROM

			Local_u16CurrentPositionToRead = (Local_u8BuffArr[0] << 8 )| Local_u8BuffArr[1] ; //current position to write in eeprom calcolated by date and user running
			DS1308_GetCurrentDate();
			Local_u16CurrentPositionToWrite =  ( Local_u16CurrentPositionToRead* 7u)+ 10u ;




			EEPROM_voidReadBuffer(Local_u8BuffArr, Local_u16CurrentPositionToWrite, 7) ; //read last data of current user and day) ;



			LastTime.Hours   = Local_u8BuffArr[LAST_HOUR_ADDRESS  ];
			LastTime.Minutes = Local_u8BuffArr[LAST_MINUTE_ADDRESS];
			NewTime.Hours   = Hour12_rtc;
			NewTime.Minutes = Minute_rtc;

			ElapsedTime = Time_GetElapsed(&LastTime, &NewTime);
			//
			//							NewTime.Hours   = Local_u8BuffArr[HOURS_ELAPSED_ADDRESS  ];
			//							NewTime.Minutes = Local_u8BuffArr[MINUTES_ELAPSED_ADDRESS];

			//ElapsedTime = Time_Add(&ElapsedTime, &NewTime);


			//				Local_u8BuffArr[LAST_HOUR_ADDRESS      ] = Hour12_rtc;
			//				Local_u8BuffArr[LAST_MINUTE_ADDRESS    ] = Minute_rtc;
			Local_u8BuffArr[HOURS_ELAPSED_ADDRESS  ] = ElapsedTime.Hours;
			Local_u8BuffArr[MINUTES_ELAPSED_ADDRESS] = ElapsedTime.Minutes;
			//				Local_u8BuffArr[CURRENT_MONTH_ADDRESS  ] = Month_rtc;
			//				Local_u8BuffArr[CURRENT_DAY_ADDRESS    ] = Day_rtc;
			//Local_u8BuffArr[USER_NUMBER_ADDRESS    ] = Global_u8UserNumber ;
			g =0 ;

			EEPROM_voidWriteBuffer(&(Local_u8BuffArr[HOURS_ELAPSED_ADDRESS]), Local_u16CurrentPositionToWrite + 4, 2) ; //write last data of current user and day



			EEPROM_voidWriteBuffer(&(uint8_t){0}, CURRENT_USER_NUMBER_ADDRESS, 1) ;







			HAL_TIM_Base_Stop_IT(&htim6);

		}
	}

	G_u8ComingFlag = 0 ;
	G_u8FlagState =0;

}





/*****************************************************************************/











void void_SendDataCaptioer(void)
{
	uint8_t Local_u8DayNumber ;
	uint16_t Local_u16StartPostion ;
	uint8_t Local_u8BuffArr[8]= {0};

	//EEPROM_voidReadBuffer((uint8_t *)&Local_u8UserFlag ,(Global_u8UserNumber-1), 1) ;
	EEPROM_voidReadBuffer((uint8_t *)&Local_u8DayNumber ,9, 1) ; //day number of runing
	//Local_u16CurrentPositionTOWrite = (Global_u8UserNumber+1)*10 + Local_u8DayNumber ;
	//EEPROM_voidReadBuffer((uint8_t *)&Local_u8RuningUserNumber ,RUNINING_USER_ADDRESS, 1) ; //read runing user number
	//Local_u16CurrentPositionToWrite = (48u* (Local_u8DayNumber+1)) +  9u ;
	for (uint8_t Local_u8DayIterator = 0 ; Local_u8DayIterator <= Local_u8DayNumber ; Local_u8DayIterator ++ )
	{
		for (uint8_t Local_u8UserIterator =0 ; Local_u8UserIterator < 8 ; Local_u8UserIterator ++ )
		{
			Local_u8BuffArr[0] = Local_u8UserIterator + 1 ;
			Local_u16StartPostion = (48 * Local_u8DayIterator) + (Local_u8UserIterator * 6 ) + 10 ;
			EEPROM_voidReadBuffer((uint8_t*)&Local_u8BuffArr[1], Local_u16StartPostion, 6);

			void_SendDataOverBlutothWithLength((uint8_t*)Local_u8BuffArr, 7);

		}
	}

}

void void_SendDataCaptioerForSpesficUser( uint16_t Copy_u16StartPostion , uint8_t Copy_u8DataLength)
{
	uint8_t Local_u8DayNumber[2] ;
	uint16_t Local_u16StartPostion ;
	uint8_t Local_u8BuffArr[24]= ":usr";
	//Local_u8BuffArr[4] = Copy_u8UserNumber + '0' ;
	Local_u8BuffArr[23] = '*';
	uint16_t Local_u16CheckSum = 0 ;
	//static  uint16_t x =  0;


	uint8_t Local_u8BuffArrRead[7];

	/*
	 *
	 * :usr 1 01 06 23 43 00 00 05 4
	 *
	User1 --> :usr1090823231010999*
	bound_start -> ":usr1"

	start Date Month -> 09
	start Date Day ->	08
	start Time Hour -> 	23
	start Time Min ->	23
	Duration Hour-> 	10
	Duration Min-> 		10
	ChkSum -> 			999

	bound_End -> "*"
	 *
	 *
	 */

	uint8_t flag = 0 ;

	//uint16_t Local_u16NumberOfLogData ;

	EEPROM_voidReadBuffer((uint8_t *)Local_u8DayNumber ,CURRENT_TO_WRITE_ADDRESS, 2) ; //day number of runing


	uint16_t Local_u16CurrentPositionToRead = (Local_u8DayNumber[0] << 8 )| Local_u8DayNumber[1] ; //current position to write in eeprom calcolated by date and user running

	/*
	 * Calculate the number of log data entries to read based on the current position and the specified start position.
	 * This will determine how many entries will be sent over Bluetooth.
	 */
	//Local_u16NumberOfLogData =  Local_u16CurrentPositionToRead ;
	Uint16_To_3DigitASCII(Local_u16CurrentPositionToRead, &Local_u8BuffArr[20]); // Convert the number of log data entries to ASCII and store it in the buffer


	if (Copy_u16StartPostion >= Local_u16CurrentPositionToRead)
	{
		// If the specified start position is greater than or equal to the current position, there's no data to read.
		// Send a message indicating that there are no log data entries available.
		memcpy(Local_u8BuffArr, ":usr", 4);
		memcpy(&(Local_u8BuffArr[5]), "######*", 7); // Reset the rest of the buffer to default values
		Local_u8BuffArr[4] = '#'; // Set the user number to indicate no data
		void_SendDataOverBlutothWithLength((uint8_t*) Local_u8BuffArr, 12); // Send the final message
		return; // Exit the function as there's no data to send
	}

	Local_u16CurrentPositionToRead -= Copy_u16StartPostion ;
	//uint16_t Local_u16CurrentPositionToWrite ; //=  ( Local_u16CurrentPositionToRead* 7u)+ 10u ;


	for (uint8_t Local_u8DayIterator = 0 ; Local_u8DayIterator < Copy_u8DataLength ; Local_u8DayIterator ++ )
	{

		Local_u16StartPostion =  ( Local_u16CurrentPositionToRead* 7u)+ 10u ;

		//Local_u8BuffArr[4] = Copy_u8UserNumber + '0' ;
		//Local_u16StartPostion = (48 * Local_u8DayIterator) + ((Copy_u8UserNumber - 1) * 6 ) + 10 ;
		EEPROM_voidReadBuffer((uint8_t*)Local_u8BuffArrRead, Local_u16StartPostion, 7);
		Local_u8BuffArr[4] = Local_u8BuffArrRead[6] + '0' ;
		Local_u16CurrentPositionToRead -- ;

		if (flag == 1) {
			break;   // Exit the loop if the flag is set to 1, indicating that there are no more entries to read
		}

		if(Local_u16CurrentPositionToRead == 0 )
		{
			flag = 1 ;
		}

		for (uint8_t i = 0; i < 6; i++)
		{
			Uint8_To_2DigitASCII(Local_u8BuffArrRead[i], &Local_u8BuffArr[5 + (i * 2)]);
			Local_u16CheckSum += Local_u8BuffArrRead[i] ;
		}

//		if (x - Local_u16CurrentPositionToRead == 32)
//		{
//		Local_u16CheckSum = 5 ;
//
//		}

		Uint16_To_3DigitASCII(Local_u16CheckSum, &Local_u8BuffArr[17]);

		Local_u16CheckSum = 0 ;
		//Uint16_To_3DigitASCII(Local_u16CheckSum, &Local_u8BuffArr[17]);
		if (memcmp(Local_u8BuffArrRead, (uint8_t*) "\0\0\0\0\0\0", 6) == 0) {
			continue; // Skip sending if the data is empty
		}

		void_SendDataOverBlutothWithLength((uint8_t*)Local_u8BuffArr, 24);


	}
	memcpy(Local_u8BuffArr, ":usr", 4);
	memcpy(&(Local_u8BuffArr[5]), "######*", 7); // Reset the rest of the buffer to default values);
	Local_u8BuffArr[4] = '#'; // Set the user number
	void_SendDataOverBlutothWithLength((uint8_t*)Local_u8BuffArr, 12); // Send the final message

}

//
//void void_SendDataCaptioerForSpesficUser( uint8_t Copy_u8DayNumber)
//{
//	uint8_t Local_u8DayNumber[2] ;
//	uint16_t Local_u16StartPostion ;
//	uint8_t Local_u8BuffArr[21]= ":usr";
//	//Local_u8BuffArr[4] = Copy_u8UserNumber + '0' ;
//	Local_u8BuffArr[17] = '*';
//	uint16_t Local_u16CheckSum = 0 ;
//
//
//	uint8_t Local_u8BuffArrRead[7];
//
//	/*
//	 *
//User1 --> :usr1090823231010999*
//bound_start -> ":usr1"
//
//start Date Month -> 09
//start Date Day ->	08
//start Time Hour -> 	23
//start Time Min ->	23
//Duration Hour-> 	10
//Duration Min-> 		10
//ChkSum -> 			999
//
//bound_End -> "*"
//	 *
//	 *
//	 */
//
//	 uint8_t flag = 0 ;
//
//	EEPROM_voidReadBuffer((uint8_t *)Local_u8DayNumber ,CURRENT_TO_WRITE_ADDRESS, 2) ; //day number of runing
//
//
//	uint16_t Local_u16CurrentPositionToRead = (Local_u8DayNumber[0] << 8 )| Local_u8DayNumber[1] ; //current position to write in eeprom calcolated by date and user running
//
//	//uint16_t Local_u16CurrentPositionToWrite ; //=  ( Local_u16CurrentPositionToRead* 7u)+ 10u ;
//
//
//	for (uint8_t Local_u8DayIterator = 0 ; Local_u8DayIterator < Copy_u8DayNumber ; Local_u8DayIterator ++ )
//	{
//		Local_u16StartPostion =  ( Local_u16CurrentPositionToRead* 7u)+ 10u ;
//
//		//Local_u8BuffArr[4] = Copy_u8UserNumber + '0' ;
//		//Local_u16StartPostion = (48 * Local_u8DayIterator) + ((Copy_u8UserNumber - 1) * 6 ) + 10 ;
//		EEPROM_voidReadBuffer((uint8_t*)Local_u8BuffArrRead, Local_u16StartPostion, 7);
//		Local_u8BuffArr[4] = Local_u8BuffArrRead[6] + '0' ;
//		Local_u16CurrentPositionToRead -- ;
//
//		if (flag == 1) {
//			break;   // Exit the loop if the flag is set to 1, indicating that there are no more entries to read
//		}
//
//		if(Local_u16CurrentPositionToRead == 0 )
//		{
//			flag = 1 ;
//		}
//
//		for (uint8_t i = 0; i < 6; i++)
//		{
//			Uint8_To_2DigitASCII(Local_u8BuffArrRead[i], &Local_u8BuffArr[5 + (i * 2)]);
//			Local_u16CheckSum += Local_u8BuffArrRead[i];
//		}
//
//		//Uint16_To_3DigitASCII(Local_u16CheckSum, &Local_u8BuffArr[17]);
//		if (memcmp(Local_u8BuffArrRead, (uint8_t*) "\0\0\0\0\0\0", 6) == 0) {
//			continue; // Skip sending if the data is empty
//		}
//
//		void_SendDataOverBlutothWithLength((uint8_t*)Local_u8BuffArr, 18);
//
//
//	}
//	memcpy(Local_u8BuffArr, ":usr", 4);
//	memcpy(&(Local_u8BuffArr[5]), "######*", 7); // Reset the rest of the buffer to default values);
//	Local_u8BuffArr[4] = '#'; // Set the user number
//	void_SendDataOverBlutothWithLength((uint8_t*)Local_u8BuffArr, 12); // Send the final message
//
//}


/*
 * @brief Saves a user's password in EEPROM.
 * @param Copy_u8UserNumber The user number (1-8).
 * @param Copy_u8Password Pointer to the password array (4 bytes).
 */
void void_SaveUserPasswordInEEPROM(uint8_t Copy_u8UserNumber,uint8_t *Copy_u8Password)
{
	// Calculate the starting address for the user's password in EEPROM
	uint16_t Local_u16StartAddress = USERS_PASSWORD_ADDRESS + ((Copy_u8UserNumber - 1) * 4);
	// Write the password to EEPROM
	EEPROM_voidWriteBuffer((uint8_t*)Copy_u8Password, Local_u16StartAddress, 4);

}


/*
 * @brief Reads a user's password from EEPROM.
 * @param Copy_u8UserNumber The user number (1-8).
 * @param Copy_u8Password Pointer to the password array (4 bytes) to store the read password.
 */
void void_ReadUserPasswordFromEEPROM(uint8_t Copy_u8UserNumber,uint8_t *Copy_u8Password)
{
	// Calculate the starting address for the user's password in EEPROM
	uint16_t Local_u16StartAddress = USERS_PASSWORD_ADDRESS + ((Copy_u8UserNumber - 1) * 4);
	// Read the password from EEPROM
	EEPROM_voidReadBuffer((uint8_t*)Copy_u8Password, Local_u16StartAddress, 4);
}

void void_SendPasswordsOverBluetooth(void)
{
	uint8_t Local_u8Password[4] = ":pas";
	uint8_t Local_u8UserNumber;
	void_SendDataOverBlutothWithLength(Local_u8Password, 4);
	for (Local_u8UserNumber = 1; Local_u8UserNumber <= 8;
			Local_u8UserNumber++) {
		void_ReadUserPasswordFromEEPROM(Local_u8UserNumber, Local_u8Password);
		// Send the user number and password over Bluetooth
		//void_SendDataOverBlutothWithLength((uint8_t*) &Local_u8UserNumber, 1);
		void_SendDataOverBlutothWithLength(Local_u8Password, 4);
	}
	uint8_t terminationChar = '*';
	void_SendDataOverBlutothWithLength(&terminationChar, 1); // Send a termination character to indicate the end of the password list
}

void Uint8_To_2DigitASCII(uint8_t value, uint8_t *array)
{
	array[0] = (value / 10) + '0';
	array[1] = (value % 10) + '0';
}

void Uint16_To_3DigitASCII(uint16_t value, uint8_t *array)
{
	array[0] = (value / 100) + '0';
	array[1] = ((value / 10) % 10) + '0';
	array[2] = (value % 10) + '0';
}




void BUZZER_voidHornTimeIT(uint32_t Copy_u32MsBuzzerTimMS)
{
	BUZZER_ON ;
	__HAL_TIM_SET_AUTORELOAD(&htim17,Copy_u32MsBuzzerTimMS);
	HAL_TIM_Base_Start_IT(&htim17);

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t Local_u8Flage = 0;
	if (htim->Instance == TIM17)
	{
		if(Local_u8Flage == 1)


		{
			BUZZER_OFF;
			HAL_TIM_Base_Stop_IT(&htim17);
			Local_u8Flage =0;
		}
		else if(Local_u8Flage == 0 )
		{
			Local_u8Flage =1;
		}
	}
	else if(htim == &htim6)
	{
		HAL_UART_Transmit(&huart1,(uint8_t*)"Tel" , 3, HAL_MAX_DELAY);

	}

}


