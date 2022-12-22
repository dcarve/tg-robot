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
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

// define a var "ticks"  (contador atualizado a cada 1ms)
volatile uint32_t ticks;

// rotina de tratamento da interrupcao (ISR) to timer SysTick
void SysTick_Handler(void)
{
  ticks++;
}

// fn que qpenas retorna o valor de "ticks" quando desejado
uint32_t millis(void)
{
  return ticks;
}

/******************************************************************************/
/*                 STM32F1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_md.s).                                            */
/******************************************************************************/

// ISR rotina de servico de IRQ linha 1  (Joao Ranhel - 09/2017 )

volatile uint32_t estado_crono = 0;

volatile uint8_t usart_counter = 0;
volatile uint8_t usart_buffer[4] = {0, 0, 0, 0};


#define dt_crono  100
uint32_t uniseg=0, dezseg=0, unimin=0, decseg=0; //variáveis do cronômetro
uint32_t prx_valor = 0;
uint32_t Uuniseg=0, Udezseg=0, Uunimin=0, Udecseg=0;

void crono (void){

	prx_valor = (millis() + dt_crono);
	if (millis()>=prx_valor)
		{
			prx_valor = (millis() + dt_crono);
			decseg++;
			if(decseg==10)
			{
				decseg=0;
				uniseg++;
				if(uniseg==10)
				{
					uniseg=0;
					dezseg++;
					if(dezseg==7)
					{
						dezseg=0;
						unimin++;
						if(unimin==10)
						{
							dezseg=0;
							unimin=0;
						}
					}
				}
			}
		}



	for (int i = 0; i < 4; i++) {
			uint8_t digito = (usart_buffer[i] & 0xF0) >> 4;
			uint8_t valor = usart_buffer[i] & 0x0F;
			switch (digito) {
			case 1:
				Uunimin = valor;
				break;
			case 2:
				Udezseg= valor;
				break;
			case 3:
				Uuniseg = valor;
				break;
			case 4:
				Udecseg = valor;
				break;
			default:
				break;
			}
}
}


#define delta_t 500
volatile uint32_t Modo_Oper = 0;    // define var Modo_Oper
volatile uint32_t prox_t =delta_t;

void EXTI9_5_IRQHandler(void)
{

	if (EXTI_GetITStatus(EXTI_Line6)!=RESET && millis()>=prox_t)
	{
	prox_t=(delta_t + millis());
			Modo_Oper++;          // cada vez que atender ISR inc modo operacao
			if (Modo_Oper > 2)
			{
				Modo_Oper = 0;    // se >2 volta modo operacao 0
			}
	}
	EXTI_ClearITPendingBit(EXTI_Line6);
}

// fn que qpenas retorna o valor de "rdt_A E rdt_B" quando desejado
uint32_t Ler_Modo_Oper(void)
{
  return Modo_Oper;
}


void USART2_IRQHandler(void) {

	// check if the USART2 receive RCC_APB1Periph_USART2interrupt flag was set
	if (USART_GetITStatus(USART2, USART_IT_RXNE)) {
		switch (Ler_Modo_Oper()) {
		case 0:
			break;
		default:
			usart_buffer[usart_counter] = USART2->DR;
			usart_counter = (usart_counter + 1) % 4; // : o buffer não precisa ter tamanho 4 (aliás, é melhor que seja maior)
			break;
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}


