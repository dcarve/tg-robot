/* ----------------------------------------------------------------------------
 UFABC - Disciplina Sistemas Microprocessados - SuP - 2017.3
 Programa:  Prat_03
 Autor:     Joao Ranhel
 Descricao: Controle de saida PWM e multitask em "superloop"
 Usa:
 a) PC13 - pino 13 da GPIOC   (iniciar bus APB2_C + GPIO + com estrutura dados)
 b) PC14 - pino 14 da GPIOC   (led na placa DUr1, = acima)
 c) PB7  - pino 7 da GPIOB    (controle de intensidade via PWM)
 OBS: na pasta /src/ modificamos "stm32f1xx_it.c"
/  --------------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "stm32f1xx_it.h"    // prots serv interrupt (ISR, em stm32f1xx_it.c)

#define FREQ_TICK 1000
#define DT_PC13 250
#define DT_PC14 400
#define VAL_INC 7

uint32_t millis(void);       // prot fn millis (em stm32f1xx_it.c)

// duas vars globais para uso com PWM
uint32_t PWM_Freq = 100;     // freq do PWM
uint32_t PWM_Resol = 250;    // resolucao do PWM (qtde passos do PWM)

// fn que obtem o clock do barramento do TIMER
uint32_t get_timer_clk_freq (void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);
  uint32_t mult;
  if (RCC_Clocks.PCLK1_Frequency == RCC_Clocks.SYSCLK_Frequency)
    { mult = 1;}
  else
    { mult = 2; }
  return mult * RCC_Clocks.PCLK1_Frequency;
}

void setup_clk_timer (void)
{
  uint32_t TIMER_Freq = get_timer_clk_freq();    // obter freq bus timer
  uint32_t COUNTER_Freq = PWM_Resol * PWM_Freq;  // calc freq contador
  uint32_t PSC_Val = (TIMER_Freq / COUNTER_Freq) - 1;  // calc val PSC
  uint16_t ARR_Val = PWM_Resol - 1;                    // calc val ARR

  // como nas GPIOs, habilita o clock do TIMER
  RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   // declara estrut dados TIM4
  // reinicializa com valores default
  TIM_TimeBaseStructInit (&TIM_TimeBaseStructure);
  // modifica apenas os valores necessários
  TIM_TimeBaseStructure.TIM_Period = ARR_Val;
  TIM_TimeBaseStructure.TIM_Prescaler = PSC_Val;
  TIM_TimeBaseInit (TIM4, &TIM_TimeBaseStructure); // reinicializa TIM4
}
// fn dispara timer 4
void timer_start (void)
{
  TIM_Cmd (TIM4, ENABLE);
}
// fn para o timer 4
void timer_stop (void)
{
  TIM_Cmd (TIM4, DISABLE);
}
// fn inicializa PWM do timer 4 canal 2 (gera pwm no pino PB7)
void setup_timer_pwm (void)
{
  TIM_OCInitTypeDef  TIM_OCInitStructure;  // declara estrutura
  //sempre reinicializa com valores default
  TIM_OCStructInit (&TIM_OCInitStructure);
  // ajustes
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 0;
  // inicializa o canal 2 do TIMER4 p/ PWM
  TIM_OC2Init (TIM4, &TIM_OCInitStructure);
}

// fn cuida do SETUP das GPIOs C e B
void setup_GPIOs_LEDs(void)
{
  // habilitar o clock no barramento das GPIOs B e C
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitTypeDef GPIO_PtC;             // estrutura dados config GPIO_C
  // setup dos vals pars da estrutura de dados da GPIO_C e iniciar
  GPIO_PtC.GPIO_Pin = GPIO_Pin_13 + GPIO_Pin_14 ;
  GPIO_PtC.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_PtC.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_PtC);

  GPIO_InitTypeDef GPIO_PtB;             // estrutura dados config GPIO_B
  // setup dos vals pars da estrutura de dados da GPIO_B e iniciar
  GPIO_PtB.GPIO_Pin = GPIO_Pin_7;
  GPIO_PtB.GPIO_Mode = GPIO_Mode_AF_PP;  // modo de OUT PWM do pino PB7
  GPIO_PtB.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_PtB);
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
  setup_clk_timer();                   // setup do clk e regs timer
  setup_timer_pwm();                   // setup PWM
  timer_start();                       // dispara timer4 PWM
  setup_GPIOs_LEDs();                  // setup GPIOs interface LEDs
  setup_systick(FREQ_TICK);            // set timers p/ 1 ms  (1000 Hz)


  // vars e flags de controle do programa para 3 tarefas no superloop...
  int16_t f13, f14 = 0;                // flags e brilho
  int brilho = 0;                      // ini brilho com zero
  int dt_pb7 = PWM_Resol/VAL_INC;      // calc valor delta = resPWM/VAL_INC
  int pwm_lim = PWM_Resol;             // lim superior brilho p/ pwm
  int inc = VAL_INC;                   // inicializa incremento
  int prx_PC13 = (millis() + DT_PC13); // calc prox toggle de PC13
  int prx_PC14 = (millis() + DT_PC14); // calc prox toogle de PC14
  int prx_PB7  = (millis() + dt_pb7);  // calc prox toogle de PB7

  // entra no loop infinito
  while (1)
  {
	// TAREFA #1 toda vez que "ticks" > prx_PC13 faz toogle no LED PC13
	if (millis()>=prx_PC13)
	{
		if(f13==0) {
			f13 = 1;
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);  // apaga o LED PC13
		} else {
			f13 = 0;
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // acende o LED PC13
		}
		prx_PC13 = (millis() + DT_PC13); // prox tempo para PC13
     }

	//  TAREFA #2 toda vez que "ticks" > prx_PC14 faz toogle no LED PC14
	if (millis()>=prx_PC14)
	{
		if(f14==0){
			f14 = 1;
			GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);  // apaga o LED PC14
		} else {
			f14 = 0;
			GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET); // acende o LED PC14
		}
		prx_PC14 = (millis() + DT_PC14); // prox tempo para PC14
    }

	//  TAREFA #3 toda vez que "ticks" > prx_PB7 faz toogle no LED PB7
	if (millis()>=prx_PB7)
	{
		brilho = brilho + inc;         // incrementa/decrementa brilho
		if (brilho < 0)                // se brilho < zero - limita a zero
		{
			inc = -inc;                // inverte para decrementar brilho
			brilho = 0;                // coloca brilho no minimo
		} else if (brilho > pwm_lim) { // se brilho > PWM_Resol, limita
			inc = -inc;                // inverte para incrementar brilho
			brilho = pwm_lim;          // coloca brilho no maximo
	    }
		TIM_SetCompare2(TIM4, brilho); // reajusta val brilho deste tempo!
		prx_PB7 = millis() + dt_pb7;   // prox tempo para PB7
    }
  }

  // a CPU pode assumir outras tarefas se rodar sem delay no loop infinito

}
