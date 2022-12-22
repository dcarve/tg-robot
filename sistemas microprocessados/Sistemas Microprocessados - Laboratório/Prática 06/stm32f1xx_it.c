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
volatile uint8_t usart_buffer[6] = {0, 0, 0, 0, 0, 0};
volatile uint8_t usart_counter = 0;
#define dt_crono 100
uint32_t prx_valor= 0;
uint32_t Cuniseg=0, Cdezseg=0, Cunimin=0, Cdecseg=0; //variáveis do cronômetr
uint32_t UsartUniMin = 0, UsartDezSeg = 0, UsartUniSeg = 0, UsartDecSeg = 0;
uint8_t payload[4];
uint8_t request=0;
uint32_t prx_request=0;

#define dt_request 2000
void cronometro(void){

	if (millis()>=prx_valor)
			{
				prx_valor = (millis() + dt_crono);
				Cdecseg++;
				if(Cdecseg==10)
				{
					Cdecseg=0;
					Cuniseg++;
					if(Cuniseg==10)
					{
						Cuniseg=0;
						Cdezseg++;
						if(Cdezseg==6)
						{
							Cdezseg=0;
							Cunimin++;
							if(Cunimin==10)
							{
								Cdezseg=0;
								Cunimin=0;
							}
						}
					}
				}
				if (request==1){
				USART_SendBytes(payload, 4); // mandando sempre ,
				}
			}
	for (int i = 0; i < 5; i++) {
			uint8_t digito = (usart_buffer[i] & 0xF0) >> 4;
			uint8_t valor = usart_buffer[i] & 0x0F;

			for (int i = 0; i < 5; i++){
					if(usart_buffer[i]==0xFF){
						usart_buffer[i]=0x00;
						request=1;
						prx_request = (millis() + dt_request);
					}
			}
			if(millis()>=prx_request){
			request = 0;
			}

			switch (digito) {
			case 1:
				UsartDecSeg = valor;
				break;
			case 2:
				UsartUniSeg = valor;
				break;
			case 3:
				UsartDezSeg = valor;
				break;
			case 4:
				UsartUniMin = valor;
				break;
			default:
				break;
			}
		}
}

void USART_SendBytes(uint8_t *data, int count) {
	int i;
	for (i = 0; i < count; i++) {
		USART_SendData(USART2, *(data + i));
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
}

void assemblePayload(uint8_t min1_p, uint8_t seg1_p, uint8_t seg2_p, uint8_t decseg_p) {
	payload[0x0] = (0x1 << 4) + (decseg_p & 0x0F);
	payload[0x1] = (0x2 << 4) + (seg2_p & 0x0F);
	payload[0x2] = (0x3 << 4) + (seg1_p & 0x0F);
	payload[0x3] = (0x4 << 4) + (min1_p & 0x0F);
}


void USART2_IRQHandler(void) {

	// check if the USART2 receive RCC_APB1Periph_USART2interrupt flag was set
	if (USART_GetITStatus(USART2, USART_IT_RXNE)) {

			usart_buffer[usart_counter] = USART2->DR;
			usart_counter = (usart_counter + 1) % 5;

		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}


