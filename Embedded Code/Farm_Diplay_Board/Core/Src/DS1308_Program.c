/*
 ******************************************************************************
 * @file           : DS1308_Program.c
 * @brief          : [Enter a brief description of the file here]
 ******************************************************************************
 * @attention
 *
 * @author   : Mohamed Ahmed
 * @date     : Oct 28, 2025
 * @copyright:
 *   © 2025 M0hamed Ahmed . All rights reserved.
 *
 *   This file is part of the [Project or System Name] project.
 *   Unauthorized copying, modification, or distribution of this file,
 *   via any medium, is strictly prohibited.
 *
 ******************************************************************************
 */
#include "main.h"

#include "BIT_MATH.h"

uint16_t	Data_Clock[10];

#include "DS1308_Interface.h"
#include "DS1308_Private.h"
#include "DS1308_Config.h"

//------------------------------Start_RTC
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

uint8_t data[10];

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
void DS1308_Init(void)
{

   uint8_t sec_ = 0;// seconds register
   uint8_t hrs_= 0;// hours register
	 data[0]=0x00;// seconds register address
	 HAL_I2C_Master_Transmit (&hi2c2,0xD0,data,1,100);// transmit address
	 HAL_I2C_Master_Receive  (&hi2c2,0xD0,&sec_,1,100);// receive seconds register

   bit_clear(sec_,7);// clear CH bit to start oscillator


	 data[0]=0;data[1]=	sec_;// prepare data to write back seconds register
   HAL_I2C_Master_Transmit (&hi2c2,0xD0,data,2,100);// write back seconds register

   data[0]=2;// hours register address
	 HAL_I2C_Master_Transmit (&hi2c2,0xD0,data,1,100);// transmit address
	 HAL_I2C_Master_Receive  (&hi2c2,0xD0,&hrs_,1,100);// receive hours register
//	 bit_set(hrs_,6);// set 12-hour format
	 bit_clear(hrs_,6);// clear 12-hour format for 24-hour mode

	 data[0]=2;// hours register address
	 data[1]=hrs_;// prepare data to write back hours register
   HAL_I2C_Master_Transmit (&hi2c2,0xD0,data,2,100);// write back hours register
}

/* Set Date And Time to DS1307 RTC
 * This function sets the date and time on the DS1307 RTC using I2C communication.
 * It prepares the data to be written to the RTC registers, including seconds, minutes,
 * hours (in 12-hour format with AM/PM), weekday, day, month, and year.
 * The function uses the bin2bcd function to convert binary values to BCD format before writing.
 * Finally, it transmits the prepared data to the RTC over I2C.
 */
void DS1308SetDateAndTime(void)
{
	uint8_t  _hr;    // temporary variable for hours
  Second_rtc &= 0x7F;// ensure CH bit is cleared
	_hr = Hour12_rtc;// copy hour value
  _hr &= 0x3F; // mask to keep only hour bits
  _hr=bin2bcd(_hr);  // convert to BCD


  //bit_set(_hr,6);    // set 12-hour format bit
  bit_clear(_hr,6);  // clear 12-hour format bit for 24-hour mode

  /*MY LINE*/
  //AM_PM_rtc = Hour12_rtc /12; // determine AM/PM
  /*_____________*/
  //PM=1 , AM=0
//  if(AM_PM_rtc)	   bit_set(_hr,5);     // set PM bit if needed
//  else		       bit_clear(_hr,5);   // clear PM bit for AM

  /* Prepare data to write to RTC registers */
	data[0]=0x00;data[1]=bin2bcd(Second_rtc);data[2]=bin2bcd(Minute_rtc); data[3] = _hr ;data[4]=bin2bcd(WeekDays_rtc);
	data[5]=bin2bcd(Day_rtc);data[6]=bin2bcd(Month_rtc);data[7]=bin2bcd(Year_rtc);data[8]=0x80;

	/* Transmit data to RTC */
	HAL_I2C_Master_Transmit (&hi2c2,0xD0,data,9,10);
}


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
void DS1308_GetCurrentDate(void)
{
	 data[0]=0x03;    // Date register address
	 /* Transmit address and receive date data */
	 HAL_I2C_Master_Transmit (&hi2c2,0xD0,data,1,10); // transmit address
	 HAL_I2C_Master_Receive  (&hi2c2,0xD0,data,4,10); // receive date data

	 /* Convert BCD to binary and store in respective variables */
   WeekDays_rtc      = bcd2bin(data[0] & 0x7f);    // Weekday
   Day_rtc   		 = bcd2bin(data[1] & 0x3f);    // Day
   Month_rtc 		 = bcd2bin(data[2] & 0x1f);    // Month
   Year_rtc  		 = bcd2bin(data[3]);           // Year
}
void DS1308_GetCurrentTime(void)
{
	data[0]=0x00; // Time register address
	// Transmit address and receive time data
	HAL_I2C_Master_Transmit (&hi2c2,0xD0,data,1,100);  // transmit address
	HAL_I2C_Master_Receive  (&hi2c2,0xD0,data,3,100);  // receive time data

	/* Convert BCD to binary and store in respective variables */
  Second_rtc  = bcd2bin(data[0] & 0x7f);     // Seconds
  Minute_rtc  = bcd2bin(data[1] & 0x7f);     // Minutes
  Hour12_rtc  = data[2];                     // Hours in 12-hour format
 // AM_PM_rtc   = bit_test(data[2],5);      // AM/PM bit

  Hour12_rtc  = bcd2bin(data[2] & 0x3F);  // Convert to binary
  //Hour12_rtc  = bcd2bin(data[2] & 0x0F);  // Convert to binary


}


/*
 * Convert binary to BCD
 * This function converts an 8-bit binary value to its Binary-Coded Decimal (BCD) representation.
 * It repeatedly subtracts 10 from the binary value and increments the tens place in the BCD result
 * until the binary value is less than 10. The remaining value is added to the units place.
 * The function returns the BCD representation of the input binary value.
 * the function takes an 8-bit binary value as input.
 * the function returns the BCD representation as an 8-bit value.
 * the function does not modify the input value.
 * the function uses a loop to perform the conversion.
 * the function handles values from 0 to 99.
 * the function is static, indicating it is only accessible within this file.
 * the function uses basic arithmetic operations for conversion.
 * the function is efficient for small values.
 * the function does not use any external libraries.
 * the function is suitable for embedded systems.
 * the function is simple and easy to understand.
 */

static uint8_t bin2bcd(uint8_t binary_value)
{
  uint8_t temp;
  uint8_t retval;
  temp = binary_value;
  retval = 0;
  while(1)
  {
    if(temp >= 10)
    {
      temp -= 10;
      retval += 0x10;
    }
    else
    {
      retval += temp;
      break;
    }
  }
  return(retval);
}

/*
 * Convert BCD to binary
 * This function converts an 8-bit Binary-Coded Decimal (BCD) value to its binary representation.
 * It shifts the BCD value to isolate the tens place, multiplies it by 10, and adds the units place.
 * The function returns the binary representation of the input BCD value.
 * the function takes an 8-bit BCD value as input.
 * the function returns the binary representation as an 8-bit value.
 * the function does not modify the input value.
 * the function uses bitwise operations for conversion.
 * the function handles values from 0 to 99 in BCD format.
 * the function is static, indicating it is only accessible within this file.
 * the function uses basic arithmetic operations for conversion.
 * the function is efficient for small values.
 * the function does not use any external libraries.
 * the function is suitable for embedded systems.
 * the function is simple and easy to understand.
 */
static uint8_t bcd2bin(uint8_t bcd_value)
{
  uint8_t temp;
  temp = bcd_value;
  temp >>= 1;
  temp &= 0x78;
  return(temp + (temp >> 2) + (bcd_value & 0x0f));
}
//void get_RTC_External()
//{
//              ds1307_get_date();
//              ds1307_get_time();
//              if(AM_PM_rtc ==AM)
//              {
//                     if(Hour12_rtc ==12)
//                              Hour12_rtc= 0;
//                     else
//                              Hour=Hour12_rtc;
//              }
//              else
//              {
//                     if(Hour12_rtc==12)
//                              Hour12_rtc= 12;
//                     else
//                              Hour24_rtc=Hour12_rtc+12;
//              }
//}
//--------------------------------------------------------------------------End_RTC



