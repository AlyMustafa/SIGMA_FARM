/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "SWCHES_Interface.h"
#include "CLCD_Interface.h"
#include "DS1308_Interface.h"
#include "EEPROM_Interface.h"
#include "Service_Interface.h"
#include "Bloutoth_Interface.h"

//#define "BIT_MATH.h

#include "SERVICE.h"
#include "ErrType.h"



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint16_t	Data_Clock[10];
extern uint32_t watchdogtimer;
uint8_t buffer[3] ;
uint8_t b[32];
extern uint8_t G_u8ComingFlag ;

extern uint8_t G_u8FlagState ;



extern Time_t G_TimeElapsedForSpecficUser ;
extern uint32_t watchdogtimer;


Time_t G_TimeStart;


uint8_t G_u8flagAddress ;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t Global_u8UserNumber = 0;

uint8_t u8_RunAthintication(uint8_t Copy_u8UserNumber );

uint8_t u8_StopPumpOrNo(void);

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim17;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM6_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM17_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
const char *Global_WeekDays[8] =
{
		"",
		"Mon",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat",
		"Sun"
};

const char *UserNames[8] = {
		"Ayman",
		"Sherif",
		"Mahmoud",
		"Basem",
		"Sayed",
		"Safwat",
		"Alaa",
		"Ahmed"
};

//const uint8_t Local_u8UsersPassWrodesArrx[8][4]={
//			"1111","2222","3333","4444","5555","6666","7777","8888"
//	};

extern uint8_t Global_u8ReciveArrBuff[UART_RX_SIZE_BUFFER];
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C2_Init();
	MX_USART1_UART_Init();
	MX_TIM6_Init();
	MX_USART2_UART_Init();
	MX_TIM17_Init();
	/* USER CODE BEGIN 2 */

	//  MX_USART1_UART_Init();
	//  MX_USART1_UART_Init();

	//HAL_NVIC_SystemReset();

	HAL_UART_Receive_IT(&huart1, (uint8_t *)buffer, 3);

	HAL_UART_Receive_IT(&huart2, Global_u8ReciveArrBuff, UART_RX_SIZE_BUFFER) ;// Enable UART reception with interrupt
	CLCD_voidInit();
	DS1308_Init();                               /* Initialize DS1307 RTC */
	CLCD_voidSendCmd(1);                         // Clear LCD
	CLCD_voidGoToXY(0,0);                        // Set cursor to first row, first column
	CLCD_u8SendString(" Sigma Engineering");     // Display on LCD
	CLCD_voidGoToXY(0,1);                        // Set cursor to second row, first column
	CLCD_u8SendString("Smart Irrigation Sys");   // Display on LCD
	CLCD_voidGoToXY(0,2);                        // Set cursor to third row, first column
	CLCD_u8SendString("Designed by: ");          // Display on LCD
	CLCD_voidGoToXY(0,3);                        // Set cursor to fourth row, first column
	CLCD_u8SendString("   Sigma R&D Team");      // Display on LCD
	HAL_Delay(5000);
	// Wait for 60 seconds
	CLCD_voidSendCmd(1);                         // Clear LCD
	//CLCD_voidSendCmd(1);                         // Clear LCD
	//EEPROM_voidErase();

	uint8_t Local_u8CurrentStat[50];
	EEPROM_voidReadBuffer((uint8_t*)Local_u8CurrentStat , 0, 50);
	//	 	Day_rtc = 14 ;
	//	 	Month_rtc = 6 ;
	//	 	Hour12_rtc = 10 ;
	//		Minute_rtc = 25 ;((uint8_t*)&G_u8flagAddress , DAY_FLAG, 1);
	//	 	WeekDays_rtc = 2 ;
	//		Second_rtc = 20 ;
	//
	//   Date_rtc = 14;
	//	  	Year_rtc = 26  ;
	//	 	AM_PM_rtc = 0 ;
	//EEPROM_voidErase();

	//EEPROM_voidReadBuffer(b, 0, 32);

	SW_State_t Local_u8SWState = NO_SW_PRESSED;

	//uint8_t lbuff[32]={0};
	uint64_t Local_u64CurrentTime = 0;

	//uint8_t Local_u8DayNumber ;

	DS1308_GetCurrentDate();
	DS1308_GetCurrentTime();



	//EEPROM_voidWriteBuffer((uint8_t*)&Day_rtc, DAY_ADDRESS, 1);
	//Local_u8DayNumber = Day_rtc ;



	//Time_t Local_CurrentTime;
	//Time_t Local_CurrentElapsedTime ;

	//Time_t t ;
	HAL_UART_Transmit(&huart1,(uint8_t*) "NOK", 3, HAL_MAX_DELAY);
	//DS1308SetDateAndTime();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		watchdogtimer=0;
		if(HAL_GetTick() - Local_u64CurrentTime >= 1000)
		{
			DS1308_GetCurrentDate();
			DS1308_GetCurrentTime();
			CLCD_voidGoToXY(0, 0);
			CLCD_u8SendString("User NO ");
			CLCD_voidGoToXY(0, 3);
			CLCD_u8SendString("Run Time :");

			if ((Global_u8UserNumber > 0) && (Global_u8UserNumber < 9)) //User X Works
			{
//				CLCD_voidGoToXY(8, 0);
//				CLCD_voidSendNumber(Global_u8UserNumber);
//				CLCD_voidGoToXY(10, 0);
//				CLCD_u8SendString(UserNames[Global_u8UserNumber - 1]);



				CLCD_voidGoToXY(8, 0);
				CLCD_u8SendString("( )"); //8 9 10
				CLCD_voidGoToXY(9, 0);
				CLCD_voidSendNumber(Global_u8UserNumber);
				CLCD_voidGoToXY(12, 0);
				CLCD_u8SendString(UserNames[Global_u8UserNumber - 1]);

				//===========================


				CLCD_voidGoToXY(10, 3);
				CLCD_u8SendString("          ");
				CLCD_voidGoToXY(10, 3);

				CLCD_voidSendNumber(G_TimeElapsedForSpecficUser.Hours);
				CLCD_u8SendString(":");
				CLCD_voidSendNumber(G_TimeElapsedForSpecficUser.Minutes);
				CLCD_u8SendString(":");
				CLCD_voidSendNumber(Second_rtc);

			}
			else
			{
				CLCD_voidGoToXY(9, 0);
				CLCD_u8SendString("N/A");
				//================================

				CLCD_voidGoToXY(10, 3);
				CLCD_u8SendString("00:00:00");

			}

			CLCD_voidGoToXY(4, 1);
			CLCD_u8SendString("Time:         ");

			CLCD_voidGoToXY(10, 1);
			CLCD_voidSendNumber(Hour12_rtc);
			CLCD_voidSendData(':');
			// CLCD_voidGoToXY(12, 1);
			CLCD_voidSendNumber(Minute_rtc);
			CLCD_voidSendData(':');
			// CLCD_voidGoToXY(15, 1);
			//CLCD_voidSendData('  ');
			//CLCD_u8SendString("  ");   // Clear previous seconds value
			//CLCD_voidGoToXY(15, 1);// Move cursor back to seconds position
			CLCD_voidSendNumber(Second_rtc);



			CLCD_voidGoToXY(0, 2);
			CLCD_u8SendString("Date: ");
			//CLCD_voidGoToXY(6, 2);
			CLCD_voidSendNumber(Day_rtc);
			CLCD_voidSendData('/');
			//CLCD_voidGoToXY(9, 2);
			CLCD_voidSendNumber(Month_rtc);
			CLCD_voidSendData('/');
			// CLCD_voidGoToXY(11, 2);
			CLCD_voidSendNumber(Year_rtc + 2000);
			//			CLCD_voidGoToXY(17, 2);
			//
			//			CLCD_u8SendString(Global_WeekDays[WeekDays_rtc]);










			Local_u64CurrentTime = HAL_GetTick();
		}

		Local_u8SWState = SWCHES_u8GetSwitchState();

		if((Local_u8SWState > SW_1_PRESSED) && (Local_u8SWState < SW_10_PRESSED))
		{
			if (Global_u8UserNumber == 0)
			{
				if(u8_RunAthintication((uint8_t)Local_u8SWState) == OK)
				{
					Global_u8UserNumber =(uint8_t)Local_u8SWState ;
					Local_u64CurrentTime +=1000;
					G_TimeStart.Hours = Hour12_rtc ;
					G_TimeStart.Minutes =  Minute_rtc ;
					HAL_UART_Transmit(&huart1,(uint8_t*) "OK_", 3, HAL_MAX_DELAY);
					//				HAL_UART_Transmit_IT(&huart1, (uint8_t*) "OK_", 3);
					//				HAL_UART_Transmit(&huart1,(uint8_t*) "OK_", 3, HAL_MAX_DELAY);
				}


			}

		}
		else if(Local_u8SWState == SW_1_PRESSED)
		{
			if (Global_u8UserNumber != 0)
			{
				if (u8_StopPumpOrNo() == (uint8_t)OK)
				{
					Global_u8UserNumber = 0;
					Local_u64CurrentTime += 1000;
					HAL_UART_Transmit(&huart1,(uint8_t*) "NOK", 3, HAL_MAX_DELAY);
				}
			}
			//HAL_UART_Transmit_IT(&huart1, (uint8_t*) "NOK", 3);
		}

		//void_HandleEEpromData();
		void_HandleDataFarmUsers();

		SytemComunication_voidBluetoothCommProcessFrame();
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void)
{

	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x2000090E;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */

}

/**
 * @brief TIM6 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM6_Init(void)
{

	/* USER CODE BEGIN TIM6_Init 0 */

	/* USER CODE END TIM6_Init 0 */

	/* USER CODE BEGIN TIM6_Init 1 */

	/* USER CODE END TIM6_Init 1 */
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 64000-1;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 7500;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM6_Init 2 */

	/* USER CODE END TIM6_Init 2 */

}

/**
 * @brief TIM17 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM17_Init(void)
{

	/* USER CODE BEGIN TIM17_Init 0 */

	/* USER CODE END TIM17_Init 0 */

	/* USER CODE BEGIN TIM17_Init 1 */

	/* USER CODE END TIM17_Init 1 */
	htim17.Instance = TIM17;
	htim17.Init.Prescaler = 8000-1;
	htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim17.Init.Period = 65535;
	htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim17.Init.RepetitionCounter = 0;
	htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM17_Init 2 */

	/* USER CODE END TIM17_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */
	HAL_UART_Init(&huart1);
	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

	/*Configure GPIO pins : PC15 PC4 PC5 */
	GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB2 EEPROM_WP_Pin
                           PB3 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|EEPROM_WP_Pin
			|GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : SW_5_Pin SW_10_Pin SW_4_Pin */
	GPIO_InitStruct.Pin = SW_5_Pin|SW_10_Pin|SW_4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : SW_9_Pin SW_3_Pin SW_8_Pin SW_2_Pin */
	GPIO_InitStruct.Pin = SW_9_Pin|SW_3_Pin|SW_8_Pin|SW_2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : SW_7_Pin SW_6_Pin SW_1_Pin */
	GPIO_InitStruct.Pin = SW_7_Pin|SW_6_Pin|SW_1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PD2 */
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//uint8_t Local_u8CurrentStat ;
	if(huart == &huart1)
	{
		G_u8ComingFlag =1;
		//		BUZZER_voidHornTimeIT(20);
		if ((memcmp(buffer, "OK_", 3) == 0) &&  (Global_u8UserNumber != 0 )) {
			G_u8FlagState = 1 ;

			//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET); // Set PA12 high
			//Local_u8CurrentStat = 1 ;
			//EEPROM_voidWriteBuffer( (uint8_t*) &Global_u8UserNumber,ACTIVE_USER_NUMBER_ADDRESS, 2);
			//EEPROM_voidReadBuffer(&Local_u8CurrentStat ,RUNING_FLAG_ADDRESS, 1);
			//HAL_TIM_Base_Start_IT(&htim6);

		} else {
			G_u8FlagState = 2 ;
			//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET); // Set PA12 low

			//Local_u8CurrentStat  = 0 ;
			//EEPROM_voidWriteBuffer( &Local_u8CurrentStat,ACTIVE_USER_NUMBER_ADDRESS, 1);
			//EEPROM_voidReadBuffer(&Local_u8CurrentStat ,RUNING_FLAG_ADDRESS, 1);
			//HAL_TIM_Base_Stop_IT(&htim6);
		}
	}
	HAL_UART_Receive_IT(&huart1, (uint8_t *)buffer, 3);
}


uint8_t u8_RunAthintication(uint8_t Copy_u8UserNumber )
{
	//uint8_t Local_u8ErrorStatuse = NOK ;
	uint8_t Local_u8UsersPassWrodesArr[8][4];

	SW_State_t Swich_Stat ;
	uint8_t Local_u8Arr[4]={0};
	uint32_t Local_u32ElabsedTimeMS = HAL_GetTick();
	uint8_t Local_u8StarIterator = 0 ;
	void_ReadUserPasswordFromEEPROM(Copy_u8UserNumber,&(Local_u8UsersPassWrodesArr[Copy_u8UserNumber-1][0]));
	CLCD_voidSendCmd(1);   // Clear LCD
	CLCD_voidGoToXY(1, 0); //
	CLCD_u8SendString("User No ") ;
	CLCD_voidSendNumber(Copy_u8UserNumber);
	CLCD_voidGoToXY(11, 0); //
	CLCD_u8SendString(UserNames[Copy_u8UserNumber-1]) ;
	CLCD_u8SendString(":");


	CLCD_voidGoToXY(0, 1);
	CLCD_u8SendString("Enter Your Password:");

	CLCD_voidGoToXY(4, 2);

	while (HAL_GetTick() - Local_u32ElabsedTimeMS < 60000 )
	{
		Swich_Stat = SWCHES_u8GetSwitchState();

		if (Swich_Stat < NO_SW_PRESSED)
		{

			//		if (Local_u8StarIterator == 0 )
			//		{
			//
			//		}
			Local_u8Arr[Local_u8StarIterator] = (uint8_t)Swich_Stat + '0';
			CLCD_u8SendString("*");
			Local_u8StarIterator++ ;
			Local_u32ElabsedTimeMS = HAL_GetTick();




		}

		if (Local_u8StarIterator >= 4  )
		{
			CLCD_voidSendCmd(1);   // Clear LCD

			if (memcmp(Local_u8Arr , &(Local_u8UsersPassWrodesArr[Copy_u8UserNumber-1][0]),4)== 0)
			{
				return  (uint8_t)OK ;
			}
			else
			{
				return NOK ;
			}

		}


	}

	CLCD_voidSendCmd(1);   // Clear LCD

	return NOK ;




}



uint8_t u8_StopPumpOrNo(void)
{
	SW_State_t Swich_Stat ;

	uint32_t Local_u32ElabsedTimeMS = HAL_GetTick();

	CLCD_voidSendCmd(1);   // Clear LCD
	CLCD_voidGoToXY(2, 0);

	CLCD_u8SendString("Turn Off Pump?");
	CLCD_voidGoToXY(0, 3);

	CLCD_u8SendString("Yes               No");

	while (HAL_GetTick() - Local_u32ElabsedTimeMS < 15000 )
	{
		Swich_Stat = SWCHES_u8GetSwitchState();

		if(Swich_Stat == SW_1_PRESSED)
		{
			CLCD_voidSendCmd(1);   // Clear LCD
			G_u8ComingFlag =1;
			G_u8FlagState = 2 ;
			return (uint8_t)OK ;
		}
		else if ( Swich_Stat <  NO_SW_PRESSED)
		{
			CLCD_voidSendCmd(1);   // Clear LCD
			G_u8ComingFlag =0;
			G_u8FlagState = 0 ;
			return (uint8_t)NOK ;
		}

	}
	CLCD_voidSendCmd(1);   // Clear LCD
	return (uint8_t)NOK ;
}


//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim == &htim6)
//	{
//		HAL_UART_Transmit(&huart1,(uint8_t*)"Tel" , 3, HAL_MAX_DELAY);
//
//	}
//}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
