/**
 ******************************************************************************
 * @file    stm32f1xx_it.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    11-February-2014
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_it.h"
#include "stdlib.h"

/** @addtogroup IO_Toggle
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M Processor Exceptions Handlers                          */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void) {
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void) {
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void) {
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void) {
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void) {
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void) {
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void) {
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */

uint32_t ticks = 0;

void SysTick_Handler(void) {
	ticks++;
}

uint32_t millis() {
	return ticks;
}

void time_start_set() {
	time = millis();
}

uint32_t min1 = 0, seg1 = 0, seg2 = 0, decseg = 0;
uint32_t min1USART = 0, seg1USART = 0, seg2USART = 0, decsegUSART = 0;
uint32_t time = 0;

void time_get() {
	uint32_t now;
	now = millis() - time;
	min1 = (now / (1000 * 60)) % 10;
	seg1 = ((now % (1000 * 60)) / 1000) / 10;
	seg2 = ((now % (1000 * 60)) / 1000) % 10;
	decseg = ((now % (1000 * 60)) % 1000) / 100;

	for (int i = 0; i < 4; i++) {
		uint8_t digito = (usart_buffer[i] & 0xF0) >> 4;
		uint8_t valor = usart_buffer[i] & 0x0F;
		switch (digito) {
		case 1:
			min1USART = valor;
			break;
		case 2:
			seg1USART = valor;
			break;
		case 3:
			seg2USART = valor;
			break;
		case 4:
			decsegUSART = valor;
			break;
		default:
			break;
		}
	}
}

uint8_t seg7_diplay[10] = { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0,
		0xFE, 0xE6 };

uint8_t seg7(uint8_t bcd) {
	if (bcd >= 0 && bcd <= 9)
		return ~seg7_diplay[bcd];
	else
		return ~0x2; // Traço no meio
}

/******************************************************************************/
/*                 STM32F1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_md.s).                                            */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
 {
 }*/

/**
 * @}
 */

volatile uint32_t estado_crono = LOCAL;
volatile uint32_t estado_reg = 0;
volatile uint32_t EXI9_5_Before = 0;
volatile uint32_t display_timing = 0;
volatile uint8_t usart_counter = 0;
volatile uint8_t usart_buffer[4] = {0, 0, 0, 0};
uint8_t payload[4];

void EXTI9_5_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line6) != RESET
			&& (millis() - EXI9_5_Before > 150)) {
		estado_crono = (estado_crono + 1) % 3;
		EXI9_5_Before = millis();
	}
	EXTI_ClearITPendingBit(EXTI_Line6);
}

uint32_t get_estado_crono(void) {
	return estado_crono;
}

void USART_SendBytes(uint8_t *data, int count) {
	int i;
	for (i = 0; i < count; i++) {
		//while( !(USART2->SR & 0x00000040));
		USART_SendData(USART2, *(data + i));
		//USART_ClearFlag(USART2, USART_FLAG_RXNE + USART_FLAG_TC);
		//while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		// while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
			;
	}
}

void assemblePayload(uint8_t min1_p, uint8_t seg1_p, uint8_t seg2_p,
		uint8_t decseg_p) {
	payload[0x0] = (0x1 << 4) + (min1_p & 0x0F);
	payload[0x1] = (0x2 << 4) + (seg1_p & 0x0F);
	payload[0x2] = (0x3 << 4) + (seg2_p & 0x0F);
	payload[0x3] = (0x4 << 4) + (decseg_p & 0x0F);
}

void USART2_IRQHandler(void) {

	// check if the USART2 receive RCC_APB1Periph_USART2interrupt flag was set
	if (USART_GetITStatus(USART2, USART_IT_RXNE)) {
		switch (estado_crono) {
		case LOCAL:
			break;
		default:
			usart_buffer[usart_counter] = USART2->DR;
			usart_counter = (usart_counter + 1) % 4; // TODO : o buffer não precisa ter tamanho 4 (aliás, é melhor que seja maior)
			break;
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
