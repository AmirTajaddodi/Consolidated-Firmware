/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Io_SharedErrorHandlerOverride.h"
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
#define LSI_FREQUENCY 32000
#define IWDG_PRESCALER 4
#define IWDG_RESET_FREQUENCY 5
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define IDWG_RESET_FREQUENCY 5
#define TASK_1HZ_STACK_SIZE 512
#define TASK_100HZ_STACK_SIZE 512
#define TASK_1KHZ_STACK_SIZE 512
#define TASK_CANRX_STACK_SIZE 512
#define TASK_CANTX_STACK_SIZE 512
#define TIM8_AUTO_RELOAD_REG 0xFFFF
#define TIMx_FREQUENCY 96000000
#define TIM8_PWM_MINIMUM_FREQUENCY 1
#define ADC_FREQUENCY 1000
#define TIM8_PRESCALER (TIMx_FREQUENCY / TIM8_AUTO_RELOAD_REG / TIM8_PWM_MINIMUM_FREQUENCY)
#define TIM12_AUTO_RELOAD_REG 0xFFFF
#define TIM12_PRESCALER (TIMx_FREQUENCY / TIM12_AUTO_RELOAD_REG / TIM12_PWM_MINIMUM_FREQUENCY)
#define TIM12_PWM_MINIMUM_FREQUENCY 1
#define TIM3_PRESCALER 8
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define COOLANT_PRESSURE1_3V3_Pin GPIO_PIN_0
#define COOLANT_PRESSURE1_3V3_GPIO_Port GPIOA
#define APPS2_3V3_Pin GPIO_PIN_1
#define APPS2_3V3_GPIO_Port GPIOA
#define APPS1_3V3_Pin GPIO_PIN_2
#define APPS1_3V3_GPIO_Port GPIOA
#define COOLANT_PRESSURE2_3V3_Pin GPIO_PIN_3
#define COOLANT_PRESSURE2_3V3_GPIO_Port GPIOA
#define COOLANT_TEMP103V_Pin GPIO_PIN_4
#define COOLANT_TEMP103V_GPIO_Port GPIOA
#define COOLANT_TEMP2_3V3_Pin GPIO_PIN_5
#define COOLANT_TEMP2_3V3_GPIO_Port GPIOA
#define STEERING_ANGLE_3V3_Pin GPIO_PIN_6
#define STEERING_ANGLE_3V3_GPIO_Port GPIOA
#define BPS_F_3V3_Pin GPIO_PIN_7
#define BPS_F_3V3_GPIO_Port GPIOA
#define BPS_B_3V3_Pin GPIO_PIN_0
#define BPS_B_3V3_GPIO_Port GPIOB
#define BPPS_3V3_Pin GPIO_PIN_1
#define BPPS_3V3_GPIO_Port GPIOB
#define WHEELSPEED_R_5V_Pin GPIO_PIN_14
#define WHEELSPEED_R_5V_GPIO_Port GPIOB
#define WHEELSPEED_L_5V_Pin GPIO_PIN_15
#define WHEELSPEED_L_5V_GPIO_Port GPIOB
#define FLOWMETER_5V_Pin GPIO_PIN_6
#define FLOWMETER_5V_GPIO_Port GPIOC
#define BSPD_BRAKE_PRESSED_5V_Pin GPIO_PIN_7
#define BSPD_BRAKE_PRESSED_5V_GPIO_Port GPIOC
#define BRAKE_OCSC_OK_5V_Pin GPIO_PIN_8
#define BRAKE_OCSC_OK_5V_GPIO_Port GPIOC

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */