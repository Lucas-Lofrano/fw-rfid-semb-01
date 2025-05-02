/**
  ******************************************************************************
  * @file    hw.c
  * @author  Luiza Mariel Leal Caetano de Souza
  * 		 Pedro Aires Ferreira da Silva Arantes
  * @brief   Hardware HAL module.
  ******************************************************************************
**/

// Includes --------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include "hw.h"
#include "main.h"
#include "app.h"

// External variables ----------------------------------------------------------
extern TIM_HandleTypeDef htim2;

// Functions -------------------------------------------------------------------

/*
 ===============================================================================
                        ##### Time Functions #####
 ===============================================================================
*/

// Adjusts delay time (in milliseconds)
void hw_delay_ms(uint32_t time_ms)
{
	HAL_Delay(time_ms);
}

// Gets the tick value (in milliseconds)
uint32_t hw_tick_ms_get(void)
{
	return HAL_GetTick();
}

// Start the timer (2 values for autoreload are possible)
uint32_t tim_values[] = {199, 999};
uint32_t pos;
void hw_timer_start(void)
{
	pos = 0;
	__HAL_TIM_SET_AUTORELOAD(&htim2, tim_values[0]);
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	HAL_TIM_Base_Start_IT(&htim2);
}

// Stop the timer
void hw_timer_stop(void)
{
	HAL_TIM_Base_Stop_IT(&htim2);
}

// Timer's callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim2)
	{
		hw_led_toggle(LED_BLUE_Pin);
	}
}


/*
 ===============================================================================
                        ##### Servo Motor Functions #####
 ===============================================================================
*/

// Alt

/**
  * @brief  Inicializa o timer PWM para controle do servo
  * @retval Nenhum
  */
void hw_servo_init(void)
{
  // Iniciar o timer em modo PWM
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

/**
  * @brief  Define a posição do servo em graus
  * @param  angle: Ângulo desejado (0-180)
  * @retval Nenhum
  */
void hw_servo_set_position(uint8_t angle)
{
  // Limitar o ângulo entre 0-180
  if (angle > 180)
  {
	  angle = 180;
  }

  // Converter ângulo para valor PWM
  uint32_t pulse = (angle * (SERVO_MAX - SERVO_MIN) / 180) + SERVO_MIN;

  // Definir valor do PWM
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);
}

/**
  * @brief  Abre a fechadura e depois fecha
  * @retval Nenhum
  */
void hw_servo_unlock(void)
{
  // Posição de desbloqueio
  hw_servo_set_position(SERVO_OPEN);

  hw_delay_ms(1500);

  // Voltar para posição de bloqueio
  hw_servo_set_position(SERVO_CLOSED);
}

/*
 ===============================================================================
                        ##### LED Functions #####
 ===============================================================================
*/

// Toggles the specified led
void hw_led_toggle(uint16_t led)
{
	switch (led)
	{
		case LED_BLUE_Pin:
			HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
			break;
		case LED_GREEN_Pin:
			HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
			break;
		default:
			break;
	}
}

// Sets or clears the specified led
void hw_led_write(uint16_t led, uint8_t value)
{
	switch (led)
	{
		case LED_BLUE_Pin:
			HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, value);
			break;
		case LED_GREEN_Pin:
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, value);
			break;
		default:
			break;
	}
}

/*
 ===============================================================================
                          ##### CPU Functions #####
 ===============================================================================
*/

// Alt

void hw_cpu_stop(void) {

    HAL_PWR_EnableWakeUpPin(IRQ_Pin);

    // Entrar em STOP mode
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

    SystemClock_Config();  // Função gerada pelo CubeMX
}

/*
 ===============================================================================
                    ##### Interruption Functions #####
 ===============================================================================
*/
// Handles interrupts
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == IRQ_Pin)
	{
		app_rfid_checks_tag(); // Checks if irq sends a signal
	}

	if(GPIO_Pin == CONFIG_MODE_Pin)
	{
		app_config_mode(); // Enter config mode if the switch sends a signal
	}
}
