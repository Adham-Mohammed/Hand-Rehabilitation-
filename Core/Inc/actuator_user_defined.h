/*
 *actuator_user_defined.h
 *
 *  Created on: Mar 19, 2024
 *      Author: Adham
 */

#ifndef INC_ACTUATOR_USER_DEFINED_H_
#define INC_ACTUATOR_USER_DEFINED_H_

/*includes*/
#include "stm32f4xx_hal.h" /*needed to use the PWM functions, timer pointer type*/
/***********************************************************************************************/

/*timer pointers for actuator in distance mode */
//#define ACTUATOR_htim_TIMER_ONE		htim9
/***********************************************************************************************/

/*actuator port and pins*/
#define ACTUATOR_PORT  		GPIOB
#define ACTUATOR_LOWER_PIN  GPIO_PIN_4
#define ACTUATOR_HIGER_PIN  GPIO_PIN_9
/***********************************************************************************************/

/*macro to choose actuator*/
/* note that each actuator index is it's lower pin and in the function will shift to get the higher pin*/
#define ACTUATOR_ONE 		GPIO_PIN_4 /*pin_B4,pin_B5*/
#define ACTUATOR_TWO 		GPIO_PIN_6 /*pin_B6,pin_B7*/
#define ACTUATOR_THREE 		GPIO_PIN_8 /*pin_B8,pin_B9*/
/***********************************************************************************************/

/*macro to Set move direction*/
#define ACTUATOR_MOVE_IN 		0
#define ACTUATOR_MOVE_OUT 		1
/***********************************************************************************************/

/*travel speed: 4mm-10mm-30mm/sec, webSite say: 30mm/sec*/
#define ACTUATOR_TRAVEL_SPEED		30
/***********************************************************************************************/

/*PWM mode*/
/* this mode use to control the speed of actuator*/

/* parameters:
 * 		- both this parameter is define and initial automatically by stm32Cube in the main file
 * 				i)  htim: is pointer to the address of PWM timer
 * 				ii) tim_channel: is the time channel for PWM
 *
 * 		- actuator_speed: set the speed of actuator as percentage form it's max speed eg:"10=> 10% of 30mm/sec"
 * 		- actutor_index: choose which actuator from the 3
 * 		- actuator_move_direction: choose if actuator Extension => "MOVE_OUT" or Retraction => "MOVE_IN"
 * */

void ACTUATOR_PWM_MODE_INIT(TIM_HandleTypeDef *htim_pwm_timer_one, uint32_t tim_channel_one,
		TIM_HandleTypeDef *htim_pwm_timer_two, uint32_t tim_channel_two, TIM_HandleTypeDef *htim_pwm_timer_three,uint32_t tim_channel_three);
void ACTUATOR_PWM_MODE_START(uint16_t actuator_index, uint16_t actuator_speed, uint8_t actuator_move_direction);
void ACTUATOR_PWM_MODE_STOP(uint16_t actuator_index);
/***********************************************************************************************/

/*distance mode*/
/* this mode use to control actuator by move certain distance*/

/* parameters:
 * 		- actutor_index: choose which actuator from 1 to 3
 * 		- actuator_move_direction: choose if actuator Extension => "MOVE_OUT" or Retraction => "MOVE_IN"
 * 		- actuator_distance: the distance actuator move before stop in mm
 * 		- htim_travel_timer: pointer to address of the timer used to count the time need to travel the required distance
 * */
/*Initiate the distance mode by pass the timers pointers that will be used for each actuator*/
void ACTUATOR_DISTANCE_MODE_INIT(TIM_HandleTypeDef *htim_travel_timer_one,TIM_HandleTypeDef *htim_travel_timer_two,
		TIM_HandleTypeDef *htim_travel_timer_three);
void ACTUATOR_DISTANCE_MODE(uint16_t actuator_index, uint32_t actuator_distance,
		uint8_t actuator_move_direction);
/***********************************************************************************************/

/*on and off mode*/
/*this mode to start and stop actuator instantly */

/* parameters:
 * 		- actutor_index: choose which actuator from 1 to 3
 * 		- actuator_move_direction: choose if actuator Extension => "MOVE_OUT" or Retraction => "MOVE_IN"
 * */
void ACTUATOR_START(uint16_t actuator_index, uint8_t actuator_move_direction);
void ACTUATOR_STOP(uint16_t actuator_index);
void ACTUATOR_STOP_ALL(void);
/***********************************************************************************************/

#endif /* INC_ACTUATOR_USER_DEFINED_H_ */
