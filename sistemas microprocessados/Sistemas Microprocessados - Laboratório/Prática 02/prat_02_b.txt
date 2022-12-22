/* ----------------------------------------------------------------------------
 UFABC - Disciplina Sistemas Microprocessados - SuP - 2017.3
 Programa:  Prat_02
 Autor:     Joao Ranhel
 Descricao: Geracao de uma base de tempo de 1ms para o STM32F103C8
 Usa:
 PC13 - pino 13 da GPIOC   (iniciar bus APB2_C + GPIO + com estrutura dados)
  OBS: na pasta /src/ modificamos "stm32f1xx_it.c"
/  --------------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "stm32f1xx_it.h"    // prots serv interrupt (ISR, em stm32f1xx_it.c)

#define FREQ_TICK 1000
#define TEMPO_PC13 250
#define TEMPO_PC14 400
#define TEMPO_PB7 600
uint32_t millis(void);       // prot fn millis (em stm32f1xx_it.c)

// rotina que inicializa GPIOx p/ interface LEDs (GPIOC)
void setup_GPIOs_LEDs(void)
{
  // habilitar o clock do da GPIOC
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure; // declarar estrutura antes de usar
  // inicia com estrutura default da biblioteca CMSIS
  GPIO_StructInit(&GPIO_InitStructure);
  // reiniciar a GPIO mudando a pinagem como queremos
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 + GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
// fn que configura o SysTick c/ 1ms  (freq_systema/frequency
void setup_systick(uint16_t f_tick)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  (void) SysTick_Config(RCC_Clocks.HCLK_Frequency / f_tick);
}

// funcao principal do programa
int main(void)
{
  setup_GPIOs_LEDs();                  // setup GPIOs interface LEDs
  setup_systick(FREQ_TICK);            // set timers p/ 1 ms  (1000 Hz)
  // entra no loop infinito

  uint16_t st13, st14, st7 = 0;
  int countPC13 = (millis() + TEMPO_PC13);
  int countPC14 = (millis() + TEMPO_PC14);
  int countPB7  = (millis() + TEMPO_PB7);

  while (1) {
		if (millis() >= countPC13) {
			if (st13 == 0) {
				st13 = 1;
				GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
			} else {
				st13 = 0;
				GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
			}
			countPC13 = (millis() + TEMPO_PC13);
		}

		if (millis() >= countPC14) {
			if (st14 == 0) {
				st14 = 1;
				GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);
			} else {
				st14 = 0;
				GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);
			}
			countPC14 = (millis() + TEMPO_PC14);
		}

		if (millis() >= countPB7) {
					if (st7 == 0) {
						st7 = 1;
						GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
					} else {
						st7 = 0;
						GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
					}
					countPB7 = (millis() + TEMPO_PB7);
				}
	}}

