/* ----------------------------------------------------------------------------
 UFABC - Disciplina Sistemas Microprocessados - SuP - 2017.3
 Programa:  Prat_01
 Autor:     João Ranhel
 Descrição: Introdução ao curso e às funções da placa e piscar LED da placa
 Usa:
 a) PC13 - pino 13 da GPIOC   (iniciar bus APB2 + GPIO (com estrutura dados)
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
  int d = 3600000;
  // 2) habilitar o clock no barramento da GPIO
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  // 3) setup dos valores dos parâmetros da estrutura de dados da GPIO
  GPIO_LED.GPIO_Pin = GPIO_Pin_13;
  GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_2MHz;
  // 4) chamar função que inicializa a GPIO (configura registradores da GPIO)
  GPIO_Init(GPIOC, &GPIO_LED);

  /* ---- aqui começa o loop infinito ---- */
  while (1)
  {
    // 5) chamar a função que acende o LED  (led PC13 tem ânodo ligado a +3V3)
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // essa fn ACENDE o LED
    // 6) chamar função que causa um DELAY
    Delay(d);                                     // chama fn delay
    // 7) chamar a função que apaga o LED
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);   // essa fn APAGA o LED
    // 8 ) chamar função DELAY
    Delay(d);                                     // chama fn delay
  }
}
