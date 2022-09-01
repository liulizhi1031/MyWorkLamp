/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EncoderA_Pin GPIO_PIN_0
#define EncoderA_GPIO_Port GPIOA
#define EncoderA_EXTI_IRQn EXTI0_IRQn
#define EncoderB_Pin GPIO_PIN_1
#define EncoderB_GPIO_Port GPIOA
#define EncoderB_EXTI_IRQn EXTI1_IRQn
#define EncoderKey_Pin GPIO_PIN_2
#define EncoderKey_GPIO_Port GPIOA
#define TouchKey_Pin GPIO_PIN_3
#define TouchKey_GPIO_Port GPIOA
#define OledRes_Pin GPIO_PIN_0
#define OledRes_GPIO_Port GPIOB
#define OledDC_Pin GPIO_PIN_1
#define OledDC_GPIO_Port GPIOB
#define U1Tx_Pin GPIO_PIN_9
#define U1Tx_GPIO_Port GPIOA
#define U1Rx_Pin GPIO_PIN_10
#define U1Rx_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
