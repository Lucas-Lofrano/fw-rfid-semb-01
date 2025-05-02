/**
  ******************************************************************************
  * @file    hw.h
  * @author  Luiza Mariel Leal Caetano de Souza
  * 		 Pedro Aires Ferreira da Silva Arantes
  * @brief   Header file of hardware HAL module.
  ******************************************************************************
**/

#ifndef HW_H_
#define HW_H_

#ifdef __cplusplus
extern "C"{
#endif

// Definitions -----------------------------------------------------------------
#define SERVO_MIN      1000	// Valor PWM para 0 graus
#define SERVO_MAX      2000 // Valor PWM para 180 graus
#define SERVO_CLOSED   0	// Ângulo quando fechado
#define SERVO_OPEN     90 	// Ângulo quando aberto

// Functions -------------------------------------------------------------------
void hw_delay_ms(uint32_t time_ms);
uint32_t hw_tick_ms_get(void);
void hw_timer_start(void);
void hw_timer_stop(void);

void hw_servo_init(void); // Alt
void hw_servo_set_position(uint8_t angle); // Alt
void hw_servo_unlock(void); // Alt

void hw_led_toggle(uint16_t led);
void hw_led_write(uint16_t led, uint8_t value);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void hw_cpu_stop(void);


#ifdef __cplusplus
}
#endif

#endif /* HW_H_ */
