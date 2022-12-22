/**
  ******************************************************************************
  * @file    stm32f1xx_it.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11-February-2014
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F1XX_IT_H
#define __STM32F1XX_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI9_5_IRQHandler(void);
uint32_t millis();
void time_start_set();
void time_get();
uint8_t seg7(uint8_t bcd);
extern uint32_t time, min1, seg1, seg2, decseg;
extern uint32_t min1USART, seg1USART, seg2USART, decsegUSART;
uint32_t get_estado_crono();
void assemblePayload(uint8_t min1_p, uint8_t seg1_p, uint8_t seg2_p,
		uint8_t decseg_p);
void USART_SendBytes(uint8_t *data, int count);
extern volatile uint32_t estado_reg;
extern volatile uint32_t display_timing;
extern volatile uint8_t usart_counter;
volatile uint8_t usart_buffer[4];
extern uint8_t payload[4];

#define DIGIT_1 0x10
#define DIGIT_2 0x20
#define DIGIT_3 0x40
#define DIGIT_4 0x80

enum
{
	LOCAL = 0,
	EXTERNAL = 1,
	DUAL = 2
};

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1XX_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
