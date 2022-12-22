/* ----------------------------------------------------------------------------
 UFABC - Disciplina Sistemas Microprocessados - SuP - 2017.3
 Programa:  Prat_01
 Autor:     João Ranhel
 Descrição: Introdução ao curso e às funções da placa e piscar LED da placa
 Usa:
 c) PC13 - pino 13 da GPIOC   (iniciar bus APB2 + GPIO (com estrutura dados)
 	PC14 - pino 14 da GPIOC   (iniciar bus APB2 + GPIO (com estrutura dados)
/  --------------------------------------------------------------------------*/

/* ---- inclui a(s) biblioteca(s) ---- */
#include <stddef.h>
#include "stm32f10x.h"

  // 1) declarar a estrutura de dados para configurar GPIO
GPIO_InitTypeDef GPIO_LED;             // declara estrutura dados config GPIO

// esta função causa delay - atraso - no programa (conta ate que nCount==0)
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {}
}

/*  -------- aqui começa o programa principal ---- */
int main(void)
{
  // declara variável p/ tempo do delay  (a ser passada para nCount)
  int d1 = 4000000;
  int d2 = 1000000;
  // 2) habilitar o clock no barramento da GPIOB e GPIOC
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  // 3) setup dos valores dos parâmetros da estrutura de dados da GPIOC_pin_13
  GPIO_LED.GPIO_Pin = GPIO_Pin_13;
  GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_2MHz;
  // 4) chamar função que inicializa a GPIO (configura registradores da GPIO)
  GPIO_Init(GPIOC, &GPIO_LED);

  //define e chama o GPIC_pin_14
  GPIO_LED.GPIO_Pin = GPIO_Pin_14;
  GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_2MHz;
  // 4) chamar função que inicializa a GPIO (configura registradores da GPIO)
  GPIO_Init(GPIOC, &GPIO_LED);

  //define e chama o GPIOBpin_7
  GPIO_LED.GPIO_Pin = GPIO_Pin_7;
  GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_2MHz;
  // 4) chamar função que inicializa a GPIO (configura registradores da GPIO)
  GPIO_Init(GPIOB, &GPIO_LED);

  /* ---- aqui começa o loop infinito ---- */
  while (1)
  {
	//led pc13 pisca 3 vezes
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // essa fn ACENDE o LED da placa
	Delay(d2);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);   // essa fn APAGA o LED da placa
	Delay(d2);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // essa fn ACENDE o LED da placa
	Delay(d2);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);   // essa fn APAGA o LED da placa
	Delay(d2);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // essa fn ACENDE o LED da placa
	Delay(d2);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);   // essa fn APAGA o LED da placa
	Delay(d2);

    GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET); // essa fn ACENDE o LED1
    Delay(d1);
    GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET); //desliga o L1
    Delay(d1);

    GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET); // essa fn LIGA o L2
    Delay(d1);
    GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET); // essa fn DESLIGA o L2
    Delay(d1);                                     // chama fn delay1
  }
}
