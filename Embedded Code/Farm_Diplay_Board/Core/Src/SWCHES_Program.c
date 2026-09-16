/*
 ******************************************************************************
 * @file           : SWCHES_Program.c
 * @brief          : [Enter a brief description of the file here]
 ******************************************************************************
 * @attention
 *
 * @author   : Mohamed Ahmed
 * @date     : Jul 9, 2026
 * @copyright:
 *   © 2026 M0hamed Ahmed . All rights reserved.
 *
 *   This file is part of the [Project or System Name] project.
 *   Unauthorized copying, modification, or distribution of this file,
 *   via any medium, is strictly prohibited.
 *
 ******************************************************************************
 */

#include "stdint.h"
#include "main.h"

#include "Service_Interface.h"

#include "SWCHES_Interface.h"
#include "SWCHES_Private.h"
#include "SWCHES_Config.h"

/**
 * @brief  Get the state of the switches.
 * @retval SW_State_t: The state of the switches, indicating which switch is pressed or if no switch is pressed.
 */

SW_State_t SWCHES_u8GetSwitchState_1(void)
{

	static uint8_t Local_u8SWLastState = SWCHES_RELEASED_STATE;
	static uint8_t Local_u8SWNumber = 50 ;

	static uint64_t Local_u64ElapsedTime = 0;

	/*
	 * Iterate through all the switches to check their state.
	 */
	for (uint8_t i = 0; i < 10; i++)
	{

		/*
		 * If the switch is pressed and the last state was released, update the last state to pressed
		 * and return the switch number. This ensures that the function only returns a switch press once
		 * until it is released again.
		 */
		if ((HAL_GPIO_ReadPin(Global_GPIOxSwchesArr[i], Global_u16SwchesArr[i])== GPIO_PIN_RESET)&& (Local_u8SWLastState == SWCHES_RELEASED_STATE)&& (HAL_GetTick() - Local_u64ElapsedTime >= 1000)) // Check if the switch is pressed and the last state was released, and debounce time has passed)
		{
			HAL_Delay(3); // Debounce delay to avoid false triggering due to switch bounce
			Local_u8SWNumber = i;// Store the switch number that was pressed
			Local_u8SWLastState = SWCHES_PRESSED_STATE; // Update the last state to pressed
			BUZZER_voidHornTimeIT(20);
			Local_u64ElapsedTime = HAL_GetTick(); // Record the time when the switch was pressed
			return (SW_State_t) i;// Return the switch number as the state

		}
		/*
		 * If the switch is released and the last state was pressed, update the last state to released.

		 * This ensures that the function only returns a switch press once until it is released again.
		 */
		if(Local_u8SWNumber != 50)
		{
			if ((HAL_GPIO_ReadPin(Global_GPIOxSwchesArr[Local_u8SWNumber],Global_u16SwchesArr[Local_u8SWNumber]) == GPIO_PIN_SET) )
			{
				// Update the last state to released
				if (Local_u8SWNumber == i)
				{
					Local_u8SWLastState = SWCHES_RELEASED_STATE;
					Local_u8SWNumber = 50; // Reset the switch number to indicate no switch is currently pressed ;
				}


			}
		}

	}
	return NO_SW_PRESSED;
}

SW_State_t SWCHES_u8GetSwitchState(void)
{
	static uint8_t Local_u8SWLastState = SWCHES_RELEASED_STATE;
	static uint8_t Local_u8SWNumber = 50;
	static uint64_t Local_u64ElapsedTime = 0;

	/*
	 * Iterate through all the switches
	 */
	for (uint8_t i = 0; i < 10; i++)
	{
		/*
		 * Check for a new button press.
		 *
		 * The switches are Active-Low:
		 * Released -> GPIO_PIN_SET
		 * Pressed  -> GPIO_PIN_RESET
		 *
		 * 100 ms debounce is used for the power pushbuttons.
		 */
		if ((HAL_GPIO_ReadPin(Global_GPIOxSwchesArr[i],
				Global_u16SwchesArr[i]) == GPIO_PIN_RESET)
				&&
				(Local_u8SWLastState == SWCHES_RELEASED_STATE)
				&&
				((HAL_GetTick() - Local_u64ElapsedTime) >= 100))
		{
			/*
			 * Wait for the mechanical contact to settle.
			 */
			HAL_Delay(100);

			/*
			 * Confirm that the button is still pressed
			 * after the debounce period.
			 */
			if (HAL_GPIO_ReadPin(Global_GPIOxSwchesArr[i],
					Global_u16SwchesArr[i]) == GPIO_PIN_RESET)
			{
				Local_u8SWNumber = i;
				Local_u8SWLastState = SWCHES_PRESSED_STATE;

				BUZZER_voidHornTimeIT(20);

				Local_u64ElapsedTime = HAL_GetTick();

				return (SW_State_t)i;
			}
		}

		/*
		 * Check if the currently pressed switch has been released.
		 */
		if (Local_u8SWNumber != 50)
		{
			if (HAL_GPIO_ReadPin(
					Global_GPIOxSwchesArr[Local_u8SWNumber],
					Global_u16SwchesArr[Local_u8SWNumber]) == GPIO_PIN_SET)
			{
				/*
				 * Button released.
				 */
				Local_u8SWLastState = SWCHES_RELEASED_STATE;
				Local_u8SWNumber = 50;

				/*
				 * Reset debounce timer.
				 */
				Local_u64ElapsedTime = HAL_GetTick();
			}
		}
	}

	return NO_SW_PRESSED;
}
