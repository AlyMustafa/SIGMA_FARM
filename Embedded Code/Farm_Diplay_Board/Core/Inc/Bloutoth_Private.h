/**
 ******************************************************************************
 * @file           : Bloutoth_Private.h
 * @brief          : [Enter a brief description of the header file]
 ******************************************************************************
 * @attention
 *
 * @author   : Mohamed Ahmed 
 * @date     : Jul 21, 2026
 * @copyright:
 *   © 2026 Mohamed Ahmed. All rights reserved.
 *
 *   This file is part of the [Farm_Diplay_Board] project.
 *   Unauthorized copying, modification, or distribution of this file,
 *   via any medium, is strictly prohibited.
 *
 ******************************************************************************
 */
#ifndef INC_BLOUTOTH_PRIVATE_H_
#define INC_BLOUTOTH_PRIVATE_H_
// Add your includes, typedefs, function prototypes, etc. here
void Send_Show(const char* Text,uint32_t Num);
/**
 ******************************************************************************
 * @file           : SystemComunication_Privat.h
 * @brief          : [Enter a brief description of the header file]
 ******************************************************************************
 * @attention
 *
 * @author   : Mohamed Ahmed
 * @date     : Jun 3, 2025
 * @copyright:
 *   © 2025 Mohamed Ahmed. All rights reserved.
 *
 *   This file is part of the [DPM_13] project.
 *   Unauthorized copying, modification, or distribution of this file,
 *   via any medium, is strictly prohibited.
 *
 ******************************************************************************
 */


/***************************************************/

		/* Define unique frame range values */
#define FRAME_RESET                   0    // Reset atmel or stm or energy
#define FRAME_DATE                    1    // Display set type of measurement
#define FRAME_PASSWORD                2    // Setting password
#define FRAME_TIME                    3    // Time of display or update data
#define FRAME_INIT                    4    // Init atm90e32a
#define GET_PASSWORD                  5    // Brightness of ssd or led
#define FRAME_UPDATE_USER_PASS        6    // Voltage 1 offset
#define FRAME_VOLTAGE_OFFSET_2        7    // Voltage 2 offset
#define FRAME_VOLTAGE_OFFSET_3        8    // Voltage 3 offset
#define FRAME_VOLTAGE_GAIN_3          9    // Voltage 3 Gain
#define FRAME_VOLTAGE_GAIN_TOTAL      10   // Voltage total Gain
#define FRAME_CURRENT_OFFSET_1        11   // Current 1 offset
#define FRAME_CURRENT_OFFSET_2        12   // Current 2 offset
#define FRAME_CURRENT_OFFSET_3        13   // Current 3 offset
#define FRAME_ACTIVE_ENERGY           14   // Current total offset
#define FRAME_CURRENT_GAIN_1          15   // Current 1 Gain
#define FRAME_CURRENT_GAIN_2          16   // Current 2 Gain
#define FRAME_CURRENT_GAIN_3          17   // Current 3 Gain
#define FRAME_ENABLE_MESARMENT_FRAM   18   // Current total Gain
#define FRAME_POWER_OFFSET_1          19   // Power 1 offset
#define FRAME_POWER_OFFSET_2          20   // Power 2 offset
#define FRAME_POWER_OFFSET_3          21   // Power 3 offset
#define FRAME_POWER_OFFSET_TOTAL      22   // Power total offset
#define FRAME_ENERGY_ERROR_1          23   // Energy error phase 1
#define FRAME_ENERGY_ERROR_2          24   // Energy error phase 2
#define FRAME_ENERGY_ERROR_3          25   // Energy error phase 3
#define FRAME_PHASE_ANGLE_1           26   // Phase angle error energy 1
#define FRAME_PHASE_ANGLE_2           27   // Phase angle error energy 2
#define FRAME_PHASE_ANGLE_3           28   // Phase angle error energy 3
#define FRAME_GRID_FREQUENCY          29   // Grid frequency
#define FRAME_READ_ATMEL              30   // Read atmel register
#define FRAME_WRITE_ATMEL             31   // Write atmel register
#define FRAME_TEST                    32   // Test Display or EEPROM
#define FRAME_RELAY                   33   // Relay on or off
#define FRAME_BUZZER                  34   // Buzzer set time
#define FRAME_REACTIVE_POWER          35   // Read address from eeprom
#define FRAME_APPERENT_POWER          36   // Write address from eeprom
#define FRAME_VERSION                 37   // Software and hardware version
#define FRAME_INITIAL_CALIBRATION     38   // Read Line to line voltage
#define FRAME_READ_CALIBRATION        39   // Read Line to Neutral voltage
#define FRAME_ACTIVE_POWER            40   // Read power
#define FRAME_APPERENT_ENERGY         41   // Read Energy
#define FRAME_FREQUENCY               42   // Read Frequency
#define FRAME_POWER_FACTOR            43   // Read Power factor
#define FRAME_GET_DARA                45   // Set Current Transformer Ratio
#define FRAME_REQUEST                     46   // Set Voltage Transformer Ratio
#define FRAME_VOLTAGE_GAIN_1          47    // Voltage 2 Gain
#define FRAME_VOLTAGE_GAIN_2          49    // Voltage 2 Gain
#define FRAME_CURRENT                 50    // Total Voltage  offset
#define FRAME_MODBUS_PARAMETER        51  // Frame ID for configuring Modbus communication parameters
#define FRAME_INVALID                 44   // Invalid command

/*_____________________________________________*/

#define  MAX_OVERFLOW                  0x9FF6UL
/*_____________________________________________*/

#define LOW_FLAG               0u

#define HIGH_FLAG               1U
/*_____________________________________________*/


/*____________________________________________*/

#define STM_REST                     0
#define ATMEL_SOFT_REST              1
#define ATMEL_HARD_REST              2
#define ALL_ENERGY_REST              3
/*____________________________________________*/

/*____________________________________________*/

   typedef struct
   {
	   uint8_t fram_length;
	   uint8_t order_data;
	   uint8_t SeartIndex_data;


   }DataToGui_t;

   /*____________________________________________*/


#define DISPLAY_DATA_TIME                    0
#define UPDATE_DATA_TIME                     1

/*____________________________________________*/

#define SIVEN_SIGMANT_DISPLAY_1    1
#define SIVEN_SIGMANT_DISPLAY_2    2
#define SIVEN_SIGMANT_DISPLAY_3    3

#define LEDS                       0
/***************************************************/


#define FLAG_IS_HIGH              1u
#define FLAG_IS_LOW               0U

#define FLAG_IS_HANDLE            2U


#define BUFFER_CLEAR               0U


static void Void_ClearArrUart(void);

//static void Void_ClearArrUart(void);

static void void_SendTextToTerminal(const char *Copy_pcString);

void void_SendNumberToTerminal(uint32_t Copy_u32Number);

//static void void_SendFloatToTerminal(float Copy_f32Number);

static uint8_t u8_CompareTwoStringes(uint8_t * s11 , char * s22);
static uint32_t u32_ConvertStringToIntiger(uint8_t * Copy_pu8StartBuffer,uint8_t Copy_u8Lenght);



//
//static void void_sendEepromError(void);
//static void void_sendFramError(void);
//static void void_SendErrorMessage(void);


void Send_Text(const char * str);
void Send_Text_len(const char * str,uint8_t len);
void Send_Num(uint32_t num);

void void_ReadAndSendCalibrationValuos(void);

void void_SendDataMesseringToVisualBasic(DataToGui_t* p_DataOrder);



void void_validateComunicationFram(void);



void void_DisplayErrorToCalibration(uint8_t Copy_u8ErrorType);


#endif /* INC_SYSTEMCOMUNICATION_PRIVAT_H_ */




