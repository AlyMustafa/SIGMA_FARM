/**
 ******************************************************************************
 * @file           : Service_Interface.h
 * @brief          : [Enter a brief description of the header file]
 ******************************************************************************
 * @attention
 *
 * @author   : Mohamed Ahmed 
 * @date     : Jul 14, 2026
 * @copyright:
 *   © 2026 Mohamed Ahmed. All rights reserved.
 *
 *   This file is part of the [Farm_Diplay_Board] project.
 *   Unauthorized copying, modification, or distribution of this file,
 *   via any medium, is strictly prohibited.
 *
 ******************************************************************************
 */
#ifndef INC_SERVICE_INTERFACE_H_
#define INC_SERVICE_INTERFACE_H_
// Add your includes, typedefs, function prototypes, etc. here

/**
 * @brief Structure to represent elapsed time in hours, minutes, and seconds.
 */
typedef struct
{
    uint8_t Hours;
    uint8_t Minutes;

} ElapsedTime_t;


/**
 * @brief Structure to represent time in hours, minutes, and seconds.
 */
typedef struct
{
    uint8_t Hours;
    uint8_t Minutes;
    //uint8_t Seconds;
} Time_t;


void void_HandleEEpromData(void);


void void_HandleDataFarmUsers(void) ;

Time_t Time_Add(Time_t* Copy_pTime1, Time_t* Copy_pTime2);


Time_t Time_GetElapsed(Time_t*  Copy_pStartTime, Time_t* Coy_pCurrentTime);


void void_SendDataCaptioer(void);

void BUZZER_voidHornTimeIT(uint32_t Copy_u32MsBuzzerTimMS);

void Uint8_To_2DigitASCII(uint8_t value, uint8_t *array);

void Uint16_To_3DigitASCII(uint16_t value, uint8_t *array);
void void_SendDataCaptioerForSpesficUser( uint16_t Copy_u16StartPostion , uint8_t Copy_u8DataLength) ;
//void void_SendDataCaptioerForSpesficUser(uint8_t Copy_u8UserNumber);


/*
 * @brief Saves a user's password in EEPROM.
 * @param Copy_u8UserNumber The user number (1-8).
 * @param Copy_u8Password Pointer to the password array (4 bytes).
 */
void void_SaveUserPasswordInEEPROM(uint8_t Copy_u8UserNumber,uint8_t *Copy_u8Password) ;

/*
 * @brief Reads a user's password from EEPROM.
 * @param Copy_u8UserNumber The user number (1-8).
 * @param Copy_u8Password Pointer to the password array (4 bytes) to store the read password.
 */
void void_ReadUserPasswordFromEEPROM(uint8_t Copy_u8UserNumber,uint8_t *Copy_u8Password);



void void_SendPasswordsOverBluetooth(void);


#define BUZZER_ON     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET)

#define BUZZER_OFF     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET)



#define CURRENT_USER_NUMBER_ADDRESS      0u
#define CURRENT_TO_WRITE_ADDRESS            2u

#define PRIVUSE_USER_NUMBER_ADDRESS              3
#define RUNING_FLAG_ADDRESS             1
#define LAST_POSITION_WEITING_ADDRESS           4



#define  RUNINING_USER_ADDRESS              8U



#define ALUSER              0
#define ALYOM               1
#define ALSHAHR             2
#define ALHOUR              3
#define ALMINUTE            4
#define ALHOUR_ALAKER            5
#define ALMINUTE_ALAKER            6




#define CURRENT_MONTH_ADDRESS           0u
#define CURRENT_DAY_ADDRESS             1u
#define LAST_HOUR_ADDRESS               2u
#define LAST_MINUTE_ADDRESS             3u
#define MINUTES_ELAPSED_ADDRESS         5u
#define HOURS_ELAPSED_ADDRESS           4u
#define USER_NUMBER_ADDRESS              6u




#define USERS_PASSWORD_ADDRESS           4050u


#define DAY_ADDRESS                  4090u
#define DAY_FLAG                     4091u
#define START_LOGING_DAY_ADDRESS                 4092u
#define START_LOGING_MONTH_ADDRESS               4093

#endif /* INC_SERVICE_INTERFACE_H_ */
