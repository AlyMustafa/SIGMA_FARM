/**
 ******************************************************************************
 * @file           : SWCHES_Interface.h
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
#ifndef INC_SWCHES_INTERFACE_H_
#define INC_SWCHES_INTERFACE_H_
// Add your includes, typedefs, function prototypes, etc. here


typedef enum {
	SW_1_PRESSED = 0,
	SW_2_PRESSED,
	SW_3_PRESSED,
	SW_4_PRESSED,
	SW_5_PRESSED,
	SW_6_PRESSED,
	SW_7_PRESSED,
	SW_8_PRESSED,
	SW_9_PRESSED,
	SW_10_PRESSED,
	NO_SW_PRESSED
}SW_State_t;

/**
 * @brief  Get the state of the switches.
 * @retval SW_State_t: The state of the switches, indicating which switch is pressed or if no switch is pressed.
 */
SW_State_t SWCHES_u8GetSwitchState(void);

#endif /* INC_SWCHES_INTERFACE_H_ */
