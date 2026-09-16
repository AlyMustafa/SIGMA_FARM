/**
 ******************************************************************************
 * @file           : DS1308_Private.h
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
#ifndef INC_DS1308_PRIVATE_H_
#define INC_DS1308_PRIVATE_H_
// Add your includes, typedefs, function prototypes, etc. here

// Convert BCD to Binary


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
static uint8_t bin2bcd(uint8_t binary_value);

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
static uint8_t bcd2bin(uint8_t bcd_value);


#endif /* INC_DS1308_PRIVATE_H_ */
