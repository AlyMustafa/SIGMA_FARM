/**
 ******************************************************************************
 * @file           : SWCHES_Config.h
 * @brief          : [Enter a brief description of the header file]
 ******************************************************************************
 * @attention
 *
 * @author   : Mohamed Ahmed 
 * @date     : Jul 9, 2026
 * @copyright:
 *   © 2026 Mohamed Ahmed. All rights reserved.
 *
 *   This file is part of the [Farm_Diplay_Board] project.
 *   Unauthorized copying, modification, or distribution of this file,
 *   via any medium, is strictly prohibited.
 *
 ******************************************************************************
 */
#ifndef INC_SWCHES_CONFIG_H_
#define INC_SWCHES_CONFIG_H_
// Add your includes, typedefs, function prototypes, etc. here


// Define the GPIO pins and ports for the switches
uint16_t Global_u16SwchesArr[10] = {SW_1_Pin, SW_2_Pin, SW_3_Pin, SW_4_Pin, SW_5_Pin, SW_6_Pin, SW_7_Pin, SW_8_Pin, SW_9_Pin, SW_10_Pin};

// Define the GPIO ports for the switches
GPIO_TypeDef *Global_GPIOxSwchesArr[10] = {SW_1_GPIO_Port, SW_2_GPIO_Port, SW_3_GPIO_Port, SW_4_GPIO_Port, SW_5_GPIO_Port, SW_6_GPIO_Port, SW_7_GPIO_Port, SW_8_GPIO_Port, SW_9_GPIO_Port, SW_10_GPIO_Port};

#endif /* INC_SWCHES_CONFIG_H_ */
