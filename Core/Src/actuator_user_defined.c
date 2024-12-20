/*
 * actuator_user_defined.c
 *
 *  Created on: Mar 19, 2024
 *      Author: Adham
 */

#include "actuator_user_defined.h"

/***********************************************************************************************/

/*macros*/
#define FREE		0
#define BUSY		1
/***********************************************************************************************/

/*global declarations*/

/*PWM mode*/
/*global timer pointer*/
TIM_HandleTypeDef *htim_pwm_timer_global_one;
TIM_HandleTypeDef *htim_pwm_timer_global_two;
TIM_HandleTypeDef *htim_pwm_timer_global_three;
/*global timer channels*/
uint32_t tim_channel_global_one;
uint32_t tim_channel_global_two;
uint32_t tim_channel_global_three;

/*distance mode*/
/*global timer pointer*/
TIM_HandleTypeDef *htim_travel_timer_global_one;
TIM_HandleTypeDef *htim_travel_timer_global_two;
TIM_HandleTypeDef *htim_travel_timer_global_three;
/*global flags*/
uint8_t	actuator_timer_busy_one=FREE;
uint8_t	actuator_timer_busy_two=FREE;
uint8_t	actuator_timer_busy_three=FREE;

/***********************************************************************************************/

/*PWM mode*/
/*select the timer and channels used in PWM mode*/
void ACTUATOR_PWM_MODE_INIT(TIM_HandleTypeDef *htim_pwm_timer_one, uint32_t tim_channel_one,
		TIM_HandleTypeDef *htim_pwm_timer_two, uint32_t tim_channel_two, TIM_HandleTypeDef *htim_pwm_timer_three,uint32_t tim_channel_three){
	/*set timer pointer to global pointer*/
	htim_pwm_timer_global_one=htim_pwm_timer_one;
	htim_pwm_timer_global_two=htim_pwm_timer_two;
	htim_pwm_timer_global_three=htim_pwm_timer_three;
	/*set timer channel to global channel*/
	tim_channel_global_one=tim_channel_one;
	tim_channel_global_two=tim_channel_two;
	tim_channel_global_three=tim_channel_three;
}

/*start actuator with required speed*/
void ACTUATOR_PWM_MODE_START(uint16_t actuator_index, uint16_t actuator_speed, uint8_t actuator_move_direction){
	if(actuator_index==ACTUATOR_ONE){
		/*start PWM signal generation*/
		HAL_TIM_PWM_Start(htim_pwm_timer_global_one, tim_channel_global_one);
		/*set speed of actuator by set the compare register to set the duty cycle*/
		__HAL_TIM_SET_COMPARE(htim_pwm_timer_global_one, tim_channel_global_one,actuator_speed);
		/*start actuator*/
		ACTUATOR_START(ACTUATOR_ONE, actuator_move_direction);
	}
	else if(actuator_index==ACTUATOR_TWO){
		/*start PWM signal generation*/
		HAL_TIM_PWM_Start(htim_pwm_timer_global_two, tim_channel_global_two);
		/*set speed of actuator by set the compare register to set the duty cycle*/
		__HAL_TIM_SET_COMPARE(htim_pwm_timer_global_two, tim_channel_global_two,actuator_speed);
		/*start actuator*/
		ACTUATOR_START(ACTUATOR_TWO, actuator_move_direction);
	}
	else if(actuator_index==ACTUATOR_THREE){
		/*start PWM signal generation*/
		HAL_TIM_PWM_Start(htim_pwm_timer_global_three, tim_channel_global_three);
		/*set speed of actuator by set the compare register to set the duty cycle*/
		__HAL_TIM_SET_COMPARE(htim_pwm_timer_global_three, tim_channel_global_three,actuator_speed);
		/*start actuator*/
		ACTUATOR_START(ACTUATOR_THREE, actuator_move_direction);
	}
}

/*stop actuator*/
void ACTUATOR_PWM_MODE_STOP(uint16_t actuator_index){
	if(actuator_index==ACTUATOR_ONE){
		/*Stops the PWM signal generation*/
		HAL_TIM_PWM_Stop(htim_pwm_timer_global_one, tim_channel_global_one);
		/*stop the actuator*/
		ACTUATOR_STOP(ACTUATOR_ONE);
	}
	else if(actuator_index==ACTUATOR_TWO){
		/*Stops the PWM signal generation*/
		HAL_TIM_PWM_Stop(htim_pwm_timer_global_two, tim_channel_global_two);
		/*stop the actuator*/
		ACTUATOR_STOP(ACTUATOR_TWO);
	}
	else if(actuator_index==ACTUATOR_THREE){
		/*Stops the PWM signal generation*/
		HAL_TIM_PWM_Stop(htim_pwm_timer_global_three, tim_channel_global_three);
		/*stop the actuator*/
		ACTUATOR_STOP(ACTUATOR_THREE);
	}
}
/***********************************************************************************************/

/*distance mode*/
void ACTUATOR_DISTANCE_MODE_INIT(TIM_HandleTypeDef *htim_travel_timer_one,TIM_HandleTypeDef *htim_travel_timer_two,
		TIM_HandleTypeDef *htim_travel_timer_three){
	htim_travel_timer_global_one = htim_travel_timer_one;
	htim_travel_timer_global_two = htim_travel_timer_two;
	htim_travel_timer_global_three = htim_travel_timer_three;

}

void ACTUATOR_DISTANCE_MODE(uint16_t actuator_index, uint32_t actuator_distance,
		uint8_t actuator_move_direction){
	/*Initiate time*/
	uint32_t time =0;
	while(actuator_timer_busy_one == BUSY);
	if(actuator_index == ACTUATOR_ONE && actuator_timer_busy_one != BUSY ){
		/*set timer to busy*/
		actuator_timer_busy_one=BUSY;
		/*calculate the time  needed to travel distance in milliMeter*/
		time = ((actuator_distance *10000)/ ACTUATOR_TRAVEL_SPEED); /*every 10000 tick is 1 second and the multiplication due to division rule*/
		/*set the autoReload value: the value where timer overflow*/
		__HAL_TIM_SET_AUTORELOAD(htim_travel_timer_global_one,time);
		/*start timer*/
		HAL_TIM_Base_Start_IT(htim_travel_timer_global_one); /*it for interrupt*/
		/*start actuator*/
		ACTUATOR_START(ACTUATOR_ONE, actuator_move_direction);
	}
	else if(actuator_index == ACTUATOR_TWO && actuator_timer_busy_two != BUSY ){
		/*set timer to busy*/
		actuator_timer_busy_two=BUSY;
		/*calculate the time  needed to travel distance in milliMeter*/
		time = ((actuator_distance *10000)/ ACTUATOR_TRAVEL_SPEED); /*every 10000 tick is 1 second and the multiplication due to division rule*/
		/*set the autoReload value: the value where timer overflow*/
		__HAL_TIM_SET_AUTORELOAD(htim_travel_timer_global_two,time);
		/*start timer*/
		HAL_TIM_Base_Start_IT(htim_travel_timer_global_two); /*it for interrupt*/
		/*start actuator*/
		ACTUATOR_START(ACTUATOR_TWO, actuator_move_direction);
	}
	else if(actuator_index == ACTUATOR_THREE && actuator_timer_busy_three != BUSY ){
		/*set timer to busy*/
		actuator_timer_busy_three=BUSY;
		/*calculate the time  needed to travel distance in milliMeter*/
		time = ((actuator_distance *10000)/ ACTUATOR_TRAVEL_SPEED); /*every 10000 tick is 1 second and the multiplication due to division rule*/
		/*set the autoReload value: the value where timer overflow*/
		__HAL_TIM_SET_AUTORELOAD(htim_travel_timer_global_three,time);
		/*start timer*/
		HAL_TIM_Base_Start_IT(htim_travel_timer_global_three); /*it for interrupt*/
		/*start actuator*/
		ACTUATOR_START(ACTUATOR_THREE, actuator_move_direction);
	}
}
/***********************************************************************************************/

/*on and off mode*/
void ACTUATOR_START(uint16_t actuator_index, uint8_t actuator_move_direction){
	/*check that actuator_index is in the range of actuators' pins need to change it if pins
	 * is not put sequentially "لو مش ورا بعض"*/
	if(actuator_index>=ACTUATOR_LOWER_PIN && actuator_index<=ACTUATOR_HIGER_PIN){
		/*select the direction of motion either forward or backward */
		if(actuator_move_direction==ACTUATOR_MOVE_OUT){
			HAL_GPIO_WritePin(ACTUATOR_PORT, actuator_index, 0);
			/*shift right to get the higher pin
			 * eg: if the actuator_index is actuator one which is GPIO_pin4
			 * actuator_index <<1 = GPIO_pin4 <<1 = GPIO_pin5*/
			HAL_GPIO_WritePin(ACTUATOR_PORT, actuator_index << 1, 1);
		}
		else if(actuator_move_direction==ACTUATOR_MOVE_IN){
			HAL_GPIO_WritePin(ACTUATOR_PORT, actuator_index, 1);
			HAL_GPIO_WritePin(ACTUATOR_PORT, actuator_index << 1, 0);
		}

	}
}

void ACTUATOR_STOP(uint16_t actuator_index){
	/*check that actuator_index is in the range of actuators' pins need to change it if pins
	 * is not put sequentially "لو مش ورا بعض"*/
	if(actuator_index>=ACTUATOR_LOWER_PIN && actuator_index<=ACTUATOR_HIGER_PIN){
		/*set both pins of the selected actuator to zeros*/
		HAL_GPIO_WritePin(ACTUATOR_PORT, actuator_index, 0);
		/*shift right to get the higher pin
		 * eg: if the actuator_index is actuator one which is GPIO_pin4
		 * actuator_index <<1 = GPIO_pin4 <<1 = GPIO_pin5*/
		HAL_GPIO_WritePin(ACTUATOR_PORT, actuator_index << 1, 0);
	}
}

void ACTUATOR_STOP_ALL(void){
	/*stop all actuators by set all to zero*/
	HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_ONE, 0);
	HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_ONE << 1, 0);

	HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_TWO, 0);
	HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_TWO << 1, 0);

	HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_THREE, 0);
	HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_THREE << 1, 0);
}
/***********************************************************************************************/

/*interrupt function*/
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim){
	if (htim == htim_travel_timer_global_one){
		HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_ONE, 0);
		HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_ONE << 1, 0);
		/*set timer as free*/
		actuator_timer_busy_one=FREE;
	}
	if (htim == htim_travel_timer_global_two){
		HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_TWO, 0);
		HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_TWO << 1, 0);
		/*set timer as free*/
		actuator_timer_busy_two=FREE;
	}
	if (htim == htim_travel_timer_global_three){
		HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_THREE, 0);
		HAL_GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_THREE << 1, 0);
		/*set timer as free*/
		actuator_timer_busy_three=FREE;
	}
}
/***********************************************************************************************/




