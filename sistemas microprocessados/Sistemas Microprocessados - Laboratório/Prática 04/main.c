/* ----------------------------------------------------------------------------
 UFABC - Disciplina Sistemas Microprocessados - SuP - 2017.3
 Programa:  Prat_04
 Autor:     Joao Ranhel
 Descricao: Interrupcao Externa (input port) e "debouncing"
 Funcionamento:
No projeto da placa Display UFABCrev1 ha 1 botao no pino B6 (ver projeto):
Quando precionado, gerar uma INTERRUPCAO externa (criar ISR p/ isso
(observar a necessidade de debouning).
Quando precionado, trocar o padrao de acionamento dos LEDs
 Usa:
 a) PC13 - pino 13 da GPIOC   (iniciar bus APB2_C + GPIO + com estrutura dados)
 b) PC14 - pino 14 da GPIOC   (led na placa DUr1, = acima)
 c) PB7  - pino 7 da GPIOB    (controle de intensidade via PWM)
 d) PB6  - pino 6 da GPIOB    (input - pedido de interrupcao)
 OBS.1 : na pasta /src/ modificamos "stm32f1xx_it.c"
 OBS.2 : na bib. /Libraries/STM32F10X_StdPeriph_Driver vamos modificar os arqs:
 stm32f10x_exti.h ; stm32f10x_exti.h  para atender interrupcoes.
/  --------------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "stm32f1xx_it.h"      // prots serv interrupt (ISR, em stm32f1xx_it.c)

#define FREQ_TICK 1000
#define DT_PC13 500
#define DT_PC14 250
#define DT_PB7  50

// dec ESTRUTURAS dados: GPIO, EXTI (external int) e NVIC (contrl vet interrpt)
EXTI_InitTypeDef IntExt_PtB;           // declara estrut IntExt_PtB
NVIC_InitTypeDef NVIC_PtB;             // estrutura NVIC port B
GPIO_InitTypeDef GPIO_PtC;             // estrutura dados config GPIO_C
GPIO_InitTypeDef GPIO_PtB;             // estrutura dados config GPIO_B

uint32_t millis(void);         // prot fn millis (em stm32f1xx_it.c)
uint32_t Ler_Modo_Oper(void);  // prot fn ModoOper (em stm32f1xx_it.c)

// -- esta secao e nova nesta pratica --
void setup_INT_externa(void)
{
  // configurar dados estrutura interrupcao
  IntExt_PtB.EXTI_Line = EXTI_Line6;           // qual linha pede interrupcao
  IntExt_PtB.EXTI_Mode = EXTI_Mode_Interrupt;  // modo interrupcao
  IntExt_PtB.EXTI_Trigger = EXTI_Trigger_Rising;  // dispara no falling_edge
  IntExt_PtB.EXTI_LineCmd = ENABLE;           // habilita ext_int
  EXTI_Init(&IntExt_PtB);  // chama função que inicializa interrupcao

  // configurar o NVIC (estrutura e funcao no misc.h e misc.c)
  NVIC_PtB.NVIC_IRQChannel = EXTI9_5_IRQn;       // IRQ_ext linha EXTI9_5_IRQn
  NVIC_PtB.NVIC_IRQChannelPreemptionPriority = 1;// prioridade preempt
  NVIC_PtB.NVIC_IRQChannelSubPriority = 1;       // prioridade 1
  NVIC_PtB.NVIC_IRQChannelCmd = ENABLE;		     // habilitada
  NVIC_Init(&NVIC_PtB);    // chama fn que inicializa NVIC
}

// esta funcao cuida do SETUP das GPIOs C e B
void setup_GPIOs_LEDs(void)
{
  // habilitar o clock no barramento das GPIOs B e C
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  // habilitar o clock do periferico de interrupcao
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  // setup dos vals pars da estrutura de dados da GPIO_C e iniciar
  GPIO_PtC.GPIO_Pin = GPIO_Pin_13 + GPIO_Pin_14 ;
  GPIO_PtC.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_PtC.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_PtC);

  // setup dos vals pars da estrutura de dados da GPIO_B e iniciar
  GPIO_PtB.GPIO_Pin = GPIO_Pin_7;
  GPIO_PtB.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_PtB.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_PtB);

  // setup dos vals p/ estrutura de GPIO_B e iniciar pino 6 INPUT
  GPIO_PtB.GPIO_Pin = GPIO_Pin_6;
  GPIO_PtB.GPIO_Mode = GPIO_Mode_IN_FLOATING; // modo input pino B6
  GPIO_PtB.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_PtB);

  // config que GPIOB pino 6 sera usado para gerar EXT INT
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
  EXTI_ClearITPendingBit(EXTI_Line6);

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
  setup_INT_externa();                 // setup Interrupcao externa
  setup_systick(FREQ_TICK);            // set timers p/ 1 ms  (1000 Hz)

  // vars e flags de controle do programa para 3 tarefas no superloop...
  int prx_PC13 = (millis() + DT_PC13); // calc prox toggle de PC13
  int prx_PC14 = (millis() + DT_PC14); // calc prox toogle de PC14
  int prx_PB7  = (millis() + DT_PB7);  // calc prox toogle de PB7
  int16_t f13=0, f14=0, f7=0;

  // entra no loop infinito
  while (1)
  {
	// TAREFA #1 toda vez que "ticks" > prx_PC13 faz toogle no LED PC13
	if (millis()>=prx_PC13)
	{
	  // calc proximo tempo para PC13...
	  prx_PC13 = (millis() + DT_PC13);
      if(f13==0)   // faz toogle LED PC13 entre ligado e desligado
      {
	    f13 = 1;
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);  // apaga o LED PC13
	  } else {
		f13 = 0;
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // acende o LED PC13
	  }
	  switch (Ler_Modo_Oper()) // dependendo do estado da var Modo_Oper
	  {
		case 1:                // no modo 1 inc com DT_PC14
			prx_PC13 = (millis() + DT_PC14); // calc prox tempo para PC14...
			break;
		case 2:                // no modo 2 inc com DT_PB7
			prx_PC13 = (millis() + DT_PB7);  // calc prox tempo para PC14...
			break;
		default:               // no modo 0 inc com DT_PC13
			prx_PC13 = (millis() + DT_PC13); // calc prox tempo para PC13...
			break;
	  }
	}

	//  TAREFA #2 toda vez que "ticks" > prx_PC14 faz toogle no LED PC14
	if (millis()>=prx_PC14)
	{
	  // calc prox tempo para PC14...
      prx_PC14 = (millis() + DT_PC14);
      if(f14==0)   // faz toogle LED PC14 entre ligado e desligado
      {
	    f14 = 1;
		GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);  // apaga o LED PC14
	  } else {
		f14 = 0;
		GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET); // acende o LED PC14
	  }
	  switch (Ler_Modo_Oper()) // dependendo do estado da var Modo_Oper
	  {
		case 1:                // no modo 1
			prx_PC14 = (millis() + DT_PB7); // calc prox tempo usando PB7...
			break;
		case 2:                // no modo 2
			prx_PC14 = (millis() + DT_PC13);  // calc prox tempo usando PC13...
			break;
		default:               // no modo 0
			prx_PC14 = (millis() + DT_PC14); // calc prox tempo usando PC14...
			break;
	  }
	}

/*  -- deixei este trecho para debug - para ver se o pino na PB6 funciona!
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)==0)
    {
    	GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);  // apaga o LED PC14
    }else{
    	GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);  // liga o LED PC14
    }
*/

	//  TAREFA #3 toda vez que "ticks" > prx_PB7 faz toogle no LED PB7
	if (millis()>=prx_PB7)
	{
	  // calc prox valor para PB7...
	  prx_PB7 = (millis() + DT_PB7);
      if(f7==0)    // faz toogle LED PB7 entre ligado e desligado
      {
	    f7 = 1;
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);  // apaga o LED PB7
	  } else {
		f7 = 0;
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET); // acende o LED PB7
	  }
	  switch (Ler_Modo_Oper()) // dependendo do estado da var Modo_Oper
	  {
		case 1:                // no modo 1
			prx_PB7 = (millis() + DT_PC13); // calc prox tempo usando PC13...
			break;
		case 2:                // no modo 2
			prx_PB7 = (millis() + DT_PC14);  // calc prox tempo usando PC14...
			break;
		default:               // no modo 0
			prx_PB7 = (millis() + DT_PB7); // calc prox tempo usando PB7...
			break;
	  }
	}

  // a CPU pode assumir outras tarefas se rodar sem delay no loop infinito

  }
}
