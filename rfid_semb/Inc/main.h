/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI2_SCK_FLASH_Pin GPIO_PIN_1
#define SPI2_SCK_FLASH_GPIO_Port GPIOA
#define NRSTPD_RFID_Pin GPIO_PIN_3
#define NRSTPD_RFID_GPIO_Port GPIOA
#define SPI1_NSS_RFID_Pin GPIO_PIN_4
#define SPI1_NSS_RFID_GPIO_Port GPIOA
#define SPI1_SCK_RFID_Pin GPIO_PIN_5
#define SPI1_SCK_RFID_GPIO_Port GPIOA
#define SPI1_MISO_RFID_Pin GPIO_PIN_6
#define SPI1_MISO_RFID_GPIO_Port GPIOA
#define SPI1_MOSI_RFID_Pin GPIO_PIN_7
#define SPI1_MOSI_RFID_GPIO_Port GPIOA
#define nRESET_FLASH_Pin GPIO_PIN_8
#define nRESET_FLASH_GPIO_Port GPIOA
#define nWP_FLASH_Pin GPIO_PIN_9
#define nWP_FLASH_GPIO_Port GPIOA
#define nCS_FLASH_Pin GPIO_PIN_10
#define nCS_FLASH_GPIO_Port GPIOA
#define SPI2_MISO_FLASH_Pin GPIO_PIN_11
#define SPI2_MISO_FLASH_GPIO_Port GPIOA
#define SPI2_MOSI_FLASH_Pin GPIO_PIN_12
#define SPI2_MOSI_FLASH_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define TIM2_CH1_PWM_Pin GPIO_PIN_15
#define TIM2_CH1_PWM_GPIO_Port GPIOA
#define LED_01_Pin GPIO_PIN_3
#define LED_01_GPIO_Port GPIOB
#define LED_02_Pin GPIO_PIN_4
#define LED_02_GPIO_Port GPIOB
#define PUSH_BUTTON_01_Pin GPIO_PIN_5
#define PUSH_BUTTON_01_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
