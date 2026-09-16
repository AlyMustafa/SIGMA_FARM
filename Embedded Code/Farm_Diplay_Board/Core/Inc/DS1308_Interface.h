/**
 ******************************************************************************
 * @file           : DS1308_Interface.h
 * @brief          : [Enter a brief description of the header file]
 ******************************************************************************
 * @attention
 *
 * @author   : Mohamed Ahmed 
 * @date     : Oct 28, 2025
 * @copyright:
 *   © 2025 Mohamed Ahmed. All rights reserved.
 *
 *   This file is part of the [Data_Logger] project.
 *   Unauthorized copying, modification, or distribution of this file,
 *   via any medium, is strictly prohibited.
 *
 ******************************************************************************
 */
#ifndef INC_DS1308_INTERFACE_H_
#define INC_DS1308_INTERFACE_H_
// Add your includes, typedefs, function prototypes, etc. here
//uint16_t	Data_Clock[10];

// RTC Functions Prototypes
/* Initialize DS1307 RTC */
#define  Day_rtc          	Data_Clock[0]
#define  Month_rtc        	Data_Clock[1]
#define  Hour12_rtc         Data_Clock[2]
#define  Minute_rtc       	Data_Clock[3]
#define  Second_rtc       	Data_Clock[4]
#define  WeekDays_rtc       Data_Clock[5]
#define  Date_rtc	    	Data_Clock[6]
#define  Year_rtc	  		Data_Clock[7]
#define  AM_PM_rtc	    	Data_Clock[8]
#define  Hour24_rtc         Data_Clock[9]

// Day Definitions
#define  Monday    2
#define  Tuesday   3
#define  Wednesday 4
#define  Thursday  5
#define  Friday    6
#define  Saturday  7
#define  Sunday    1

// AM/PM Definitions
#define am_pm_enable  1
#define am_pm_disable 0
#define AM 0
#define PM 1



// RTC Functions Prototypes
/* Initialize DS1307 RTC
 * This function initializes the DS1307 RTC by configuring the seconds and hours registers.
 *It clears the CH (Clock Halt) bit in the seconds register to start the oscillator and sets
 *the 12-hour format in the hours register.
 *the function uses I2C communication to read and write data to the RTC.
 *the hi2c1 handle is used for I2C communication.
 *the data array is used to store data to be transmitted and received.
 *the bit_clear and bit_set macros are used to manipulate specific bits in the registers.
 *the function does not return any value.
 *the DS1307 RTC is configured to operate in 12-hour format with the oscillator running.
 */
void DS1308_Init(void);


/* Set Date And Time to DS1307 RTC
 * This function sets the date and time on the DS1307 RTC using I2C communication.
 * It prepares the data to be written to the RTC registers, including seconds, minutes,
 * hours (in 12-hour format with AM/PM), weekday, day, month, and year.
 * The function uses the bin2bcd function to convert binary values to BCD format before writing.
 * Finally, it transmits the prepared data to the RTC over I2C.
 */
void DS1308SetDateAndTime(void);


/* Get Current Date from DS1307 RTC
 * This function retrieves the current date from the DS1307 RTC using I2C communication
 * and stores it in global variables.
 * It
 * prepares the address of the date register and transmits it to the RTC.
 * Then, it receives the date data, including weekday, day, month, and year.
 * The function converts the received BCD values to binary format using the bcd2bin function
 * and stores them in the respective global variables.
 * the function does not return any value.
 */
void DS1308_GetCurrentDate(void);

/* Get Current Time from DS1307 RTC
 * This function retrieves the current time from the DS1307 RTC using I2C communication
 * and stores it in global variables.
 * It prepares the address of the time register and transmits it to the RTC.
 * Then, it receives the time data, including seconds, minutes, hours (in 12-hour format with AM/PM).
 * The function converts the received BCD values to binary format using the bcd2bin function
 * and stores them in the respective global variables.
 * the function does not return any value.
 */
void DS1308_GetCurrentTime(void);

#endif /* INC_DS1308_INTERFACE_H_ */
