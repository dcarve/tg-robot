/*
Códico do Grupo composto por Daniel Carvalho, Vanessa Tiname, Robson Leite, Gabriel Gomes e Danilo dos Santos.
*/

#include "stm32f10x.h"
#include "stm32f1xx_it.h"      // prots serv interrupt (ISR, em stm32f1xx_it.c)

#define FREQ_TICK 1000
#define dt_data 5
#define dt_toggle 5000
#define DT_PC13 50
#define DT_PC14 50


// declação de  ESTRUTURAS de dados: GPIO, EXTI (external int) e NVIC (contrl vet interrpt)
EXTI_InitTypeDef IntExt_PtB;           // declara estrut IntExt_PtB
NVIC_InitTypeDef NVIC_PtB;             // estrutura NVIC port B
GPIO_InitTypeDef GPIO_PtC;             // estrutura dados config GPIO_C
GPIO_InitTypeDef GPIO_PtB;             // estrutura dados config GPIO_B

uint32_t millis(void);         // prot fn millis (em stm32f1xx_it.c)
uint32_t Ler_Modo_Oper(void);  // prot fn ModoOper (em stm32f1xx_it.c)

void setup_INT_externa(void) //interrupção externa
{
  // configurar dados estrutura interrupcao
  IntExt_PtB.EXTI_Line = EXTI_Line6;           // qual linha pede interrupcao
  IntExt_PtB.EXTI_Mode = EXTI_Mode_Interrupt;  // modo interrupcao
  IntExt_PtB.EXTI_Trigger = EXTI_Trigger_Rising;  // dispara no falling_edge
  IntExt_PtB.EXTI_LineCmd = ENABLE;           // habilita ext_int
  EXTI_Init(&IntExt_PtB);  // chama funÃ§Ã£o que inicializa interrupcao

  // configurar o NVIC (estrutura e funcao no misc.h e misc.c)
  NVIC_PtB.NVIC_IRQChannel = EXTI9_5_IRQn;       // IRQ_ext linha EXTI9_5_IRQn
  NVIC_PtB.NVIC_IRQChannelPreemptionPriority = 1;// prioridade preempt
  NVIC_PtB.NVIC_IRQChannelSubPriority = 1;       // prioridade 1
  NVIC_PtB.NVIC_IRQChannelCmd = ENABLE;		     // habilitada
  NVIC_Init(&NVIC_PtB);    // chama fn que inicializa NVIC
}

void setup_USART(uint32_t baud_rate) {
	GPIO_InitTypeDef GPIO_InitS;
	/* USART */
	//PA2 = USART2 TX
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // clock do GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  // clock da USART
	GPIO_InitS.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitS.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitS.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitS);
	//PA3 = USART2 RX
	GPIO_InitS.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitS.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitS);
	/* USART END */

	USART_InitTypeDef USART_InitStruct; // this is for the USART2 initilization
	USART_InitStruct.USART_BaudRate = baud_rate; // the baudrate is set to the value we passed into this init function
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; // we want the data frame size to be 8 bits (standard)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	// we want 1 stop bit (standard)
	USART_InitStruct.USART_Parity = USART_Parity_No;// we don't want a parity bit (standard)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
	USART_Init(USART2, &USART_InitStruct); // again all the properties are passed to the USART_Init function which takes care of all the bit setting

	/* Here the USART2 receive interrupt is enabled
	 * and the interrupt controller is configured
	 * to jump to the USART2_IRQHandler() function
	 * if the USART2 receive interrupt occurs
	 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // enable the USART2 receive interrupt
	NVIC_InitTypeDef NVIC_InitS;
	NVIC_InitS.NVIC_IRQChannel = USART2_IRQn; // we want to configure the USART2 interrupts
	NVIC_InitS.NVIC_IRQChannelSubPriority = 0; // this sets the subpriority inside the group
	NVIC_InitS.NVIC_IRQChannelCmd = ENABLE;	// the USART2 interrupts are globally enabled
	NVIC_Init(&NVIC_InitS);	// the properties are passed to the NVIC_Init function which takes care of the low level stuff

	// finally this enables the complete USART2 peripheral
	USART_Cmd(USART2, ENABLE);
	usart_counter = 0;
	//USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

void setup_GPIO(void)  // setup GPIO
{
  // habilitar o clock no barramento das GPIOs B e C
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  // habilitar o clock do periferico de interrupcao
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  // setup GPIO B1, B0, B13 - OUTPUT - clock, latch, data
  	GPIO_InitTypeDef GPIO_PtB;
  GPIO_PtB.GPIO_Pin = GPIO_Pin_1 + GPIO_Pin_0 + GPIO_Pin_13 +GPIO_Pin_7;
  GPIO_PtB.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_PtB.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_PtB);

  // setup GPIO B6 - interrupção - pushbutton
  GPIO_PtB.GPIO_Pin = GPIO_Pin_6;
  GPIO_PtB.GPIO_Mode = GPIO_Mode_IN_FLOATING; // modo input pino B6
  GPIO_PtB.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_PtB);

  GPIO_InitTypeDef GPIO_PtC;             // estrutura dados config GPIO_C
  	// setup dos vals pars da estrutura de dados da GPIO_C e iniciar
  	GPIO_PtC.GPIO_Pin = GPIO_Pin_13 + GPIO_Pin_14 ;
  	GPIO_PtC.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_PtC.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOC, &GPIO_PtC);



  // config que GPIOB pino 6 sera usado para gerar EXT INT
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
  EXTI_ClearITPendingBit(EXTI_Line6);

}

void setup_systick(uint16_t f_tick)  // fn que configura o SysTick c/ 1ms  (freq_systema/frequency
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  (void) SysTick_Config(RCC_Clocks.HCLK_Frequency / f_tick);
}

void serialdata (int vector[16]) //função para serializar o vetor data
{
	for (int i=0; i<=16; i++)
	{
		if (i==16) //habilita a saída dos registradores (latch)
		{
			GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
			GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
		}
		else
		{
			if (vector[i]==0) 		//lê os bits do vetor[]
			{
				GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);
			}
			else
			{
				GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);
			}
		}
			GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);	//clock
			GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);
	}
}


int main(void) // funcao principal do programa
{
	setup_GPIO();                  // setup GPIOs interface LEDs
	setup_INT_externa();                 // setup Interrupcao externa
	setup_systick(FREQ_TICK);            // set timers p/  1 ms  (1000 Hz)++-/
	setup_USART(9600);



	int data[16]; // {DP,G,F,E,D,C,B,A,X,X,X,X,D1,D2,D3,D4} - {display (MSB - LSB); espaço inútil; escolha do led}
	data[0]=0;
	data[8]=0;
	data[9]=0;
	data[10]=0;
	data[11]=0;

	int uniseg=0, dezseg=0, unimin=0, decseg=0; //variáveis do cronômetro

	//vetores do decoder
	int decsega[]={0,0,0,0,0,0,0,0};
	int unisega[]={0,0,0,0,0,0,0,0};
	int dezsega[]={0,0,0,0,0,0,0,0};
	int unimina[]={0,0,0,0,0,0,0,0};

	int flag=0; //flag do multiplex temporal
	int prx_data = (millis() + dt_data); //controla a frequencia do multiplex temporal
	int prx_dt_toggle = (millis() + dt_toggle);
	int prx_PC13 = (millis() + DT_PC13); // calc prox toggle de PC13
	int prx_PC14 = (millis() + DT_PC14);
	int flag_toggle=0;
	int f13=0, f14=0;

	// entra no loop infinito
	while (1)
	{
		cronometro();
		assemblePayload(Cunimin, Cdezseg,Cuniseg, Cdecseg);

//maquina de estado que controla o cronômetro
	switch (Ler_Modo_Oper()) // dependendo do estado da var Modo_Oper
			  {
				case 0:
						uniseg=Cuniseg;
						dezseg=Cdezseg;
						unimin=Cunimin;
						decseg=Cdecseg;


						GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
						GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);

					break;

				case 1:

					GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
					GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);

					USART_SendData(USART2, 0xFF);
					uniseg=UsartUniSeg;
					dezseg=UsartDezSeg;
					unimin=UsartUniMin;
					decseg=UsartDecSeg;
					break;

				case 2:

					if (millis()>=prx_dt_toggle){
						prx_dt_toggle = (millis() + dt_toggle);
						if (flag_toggle==1){
						flag_toggle=0;
						}
						else{
						flag_toggle=1;
						}

					}

							if (flag_toggle==1){
								uniseg=Cuniseg;
								dezseg=Cdezseg;
								unimin=Cunimin;
								decseg=Cdecseg;


								GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);
								if (millis()>=prx_PC13)
									{
									  // calc proximo tempo para PC13...
										  prx_PC13 = (millis() + DT_PC13);
										  if(f13==0)   // faz toogle LED PC13 entre ligado e desligado
										  {
											f13 = 1;
											GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);  // apaga o LED PC13
										  } else {
											f13 = 0;
											GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); // acende o LED PC13
										  }

									  }
							}

							if (flag_toggle==0){

								USART_SendData(USART2, 0xFF);
								uniseg=UsartUniSeg;
								dezseg=UsartDezSeg;
								unimin=UsartUniMin;
								decseg=UsartDecSeg;

							GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
							if (millis()>=prx_PC14)
								{
								  // calc proximo tempo para PC13...
									  prx_PC14 = (millis() + DT_PC14);
									  if(f14==0)   // faz toogle LED PC13 entre ligado e desligado
									  {
										f14 = 1;
										GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);  // apaga o LED PC13
									  } else {
										f14 = 0;
										GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET); // acende o LED PC13
									  }

								  }
							}



					break;

			  }

//---fim da máquia de estado



if (request==1){
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
}
else{
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
}





											//decoder ---------------------------------------------
													switch (decseg) // decoder dos décimos de segundos
															{
															   case 0:
																   decsega[1]=1;
																   decsega[2]=0;
																   decsega[3]=0;
																   decsega[4]=0;
																   decsega[5]=0;
																   decsega[6]=0;
																   decsega[7]=0;
															   break;

															   case 1:
																   decsega[1]=1;
																   decsega[2]=1;
																   decsega[3]=1;
																   decsega[4]=1;
																   decsega[5]=0;
																   decsega[6]=0;
																   decsega[7]=1;
															   break;

															   case 2:
																   decsega[1]=0;
																   decsega[2]=1;
																   decsega[3]=0;
																   decsega[4]=0;
																   decsega[5]=1;
																   decsega[6]=0;
																   decsega[7]=0;
															   break;

															   case 3:
																   decsega[1]=0;
																   decsega[2]=1;
																   decsega[3]=1;
																   decsega[4]=0;
																   decsega[5]=0;
																   decsega[6]=0;
																   decsega[7]=0;

															   break;

															   case 4:
																   decsega[1]=0;
																   decsega[2]=0;
																   decsega[3]=1;
																   decsega[4]=1;
																   decsega[5]=0;
																   decsega[6]=0;
																   decsega[7]=1;

															   break;

															   case 5:
																   decsega[1]=0;
																   decsega[2]=0;
																   decsega[3]=1;
																   decsega[4]=0;
																   decsega[5]=0;
																   decsega[6]=1;
																   decsega[7]=0;

															   break;

															   case 6:
																   decsega[1]=0;
																   decsega[2]=0;
																   decsega[3]=0;
																   decsega[4]=0;
																   decsega[5]=0;
																   decsega[6]=1;
																   decsega[7]=0;

															   break;

															   case 7:
																   decsega[1]=1;
																   decsega[2]=1;
																   decsega[3]=1;
																   decsega[4]=1;
																   decsega[5]=0;
																   decsega[6]=0;
																   decsega[7]=0;
															   break;

															   case 8:
																   decsega[1]=0;
																   decsega[2]=0;
																   decsega[3]=0;
																   decsega[4]=0;
																   decsega[5]=0;
																   decsega[6]=0;
																   decsega[7]=0;
															   break;

															   case 9:
																   decsega[1]=0;
																   decsega[2]=0;
																   decsega[3]=1;
																   decsega[4]=0;
																   decsega[5]=0;
																   decsega[6]=0;
																   decsega[7]=0;
															   break;

															}

													switch (uniseg) // decoder da unidade de segundos
																	{
															   case 0:
																   unisega[1]=1;
																   unisega[2]=0;
																   unisega[3]=0;
																   unisega[4]=0;
																   unisega[5]=0;
																   unisega[6]=0;
																   unisega[7]=0;
															   break;

															   case 1:
																   unisega[1]=1;
																   unisega[2]=1;
																   unisega[3]=1;
																   unisega[4]=1;
																   unisega[5]=0;
																   unisega[6]=0;
																   unisega[7]=1;
															   break;

															   case 2:
																   unisega[1]=0;
																   unisega[2]=1;
																   unisega[3]=0;
																   unisega[4]=0;
																   unisega[5]=1;
																   unisega[6]=0;
																   unisega[7]=0;
															   break;

															   case 3:
																   unisega[1]=0;
																   unisega[2]=1;
																   unisega[3]=1;
																   unisega[4]=0;
																   unisega[5]=0;
																   unisega[6]=0;
																   unisega[7]=0;

															   break;

															   case 4:
																   unisega[1]=0;
																   unisega[2]=0;
																   unisega[3]=1;
																   unisega[4]=1;
																   unisega[5]=0;
																   unisega[6]=0;
																   unisega[7]=1;

															   break;

															   case 5:
																   unisega[1]=0;
																   unisega[2]=0;
																   unisega[3]=1;
																   unisega[4]=0;
																   unisega[5]=0;
																   unisega[6]=1;
																   unisega[7]=0;

															   break;

															   case 6:
																   unisega[1]=0;
																   unisega[2]=0;
																   unisega[3]=0;
																   unisega[4]=0;
																   unisega[5]=0;
																   unisega[6]=1;
																   unisega[7]=0;

															   break;

															   case 7:
																   unisega[1]=1;
																   unisega[2]=1;
																   unisega[3]=1;
																   unisega[4]=1;
																   unisega[5]=0;
																   unisega[6]=0;
																   unisega[7]=0;

															   break;

															   case 8:
																   unisega[1]=0;
																   unisega[2]=0;
																   unisega[3]=0;
																   unisega[4]=0;
																   unisega[5]=0;
																   unisega[6]=0;
																   unisega[7]=0;

															   break;

															   case 9:
																   unisega[1]=0;
																   unisega[2]=0;
																   unisega[3]=1;
																   unisega[4]=0;
																   unisega[5]=0;
																   unisega[6]=0;
																   unisega[7]=0;

															   break;

																	}

														switch (dezseg) // decoder da dezena de segundos
																{
																   case 0:

																   if(unimin==0 && dezseg==0)
																   {
																	 dezsega[1]=1;
																	 dezsega[2]=1;
																	 dezsega[3]=1;
																	 dezsega[4]=1;
																	 dezsega[5]=1;
																	 dezsega[6]=1;
																	 dezsega[7]=1;
																   }
																   else {
																   dezsega[1]=1;
																   dezsega[2]=0;
																   dezsega[3]=0;
																   dezsega[4]=0;
																   dezsega[5]=0;
																   dezsega[6]=0;
																   dezsega[7]=0;
																   }
															   break;

															   case 1:
																   dezsega[1]=1;
																   dezsega[2]=1;
																   dezsega[3]=1;
																   dezsega[4]=1;
																   dezsega[5]=0;
																   dezsega[6]=0;
																   dezsega[7]=1;
															   break;

															   case 2:
																   dezsega[1]=0;
																   dezsega[2]=1;
																   dezsega[3]=0;
																   dezsega[4]=0;
																   dezsega[5]=1;
																   dezsega[6]=0;
																   dezsega[7]=0;
															   break;

															   case 3:
																   dezsega[1]=0;
																   dezsega[2]=1;
																   dezsega[3]=1;
																   dezsega[4]=0;
																   dezsega[5]=0;
																   dezsega[6]=0;
																   dezsega[7]=0;

															   break;

															   case 4:
																   dezsega[1]=0;
																   dezsega[2]=0;
																   dezsega[3]=1;
																   dezsega[4]=1;
																   dezsega[5]=0;
																   dezsega[6]=0;
																   dezsega[7]=1;

															   break;

															   case 5:
																   dezsega[0]=1;
																   dezsega[1]=0;
																   dezsega[2]=0;
																   dezsega[3]=1;
																   dezsega[4]=0;
																   dezsega[5]=0;
																   dezsega[6]=1;
																   dezsega[7]=0;

															   break;

															   case 6:
																   dezsega[1]=0;
																   dezsega[2]=0;
																   dezsega[3]=0;
																   dezsega[4]=0;
																   dezsega[5]=0;
																   dezsega[6]=1;
																   dezsega[7]=0;

															   break;

															   case 7:
																   dezsega[1]=1;
																   dezsega[2]=1;
																   dezsega[3]=1;
																   dezsega[4]=1;
																   dezsega[5]=0;
																   dezsega[6]=0;
																   dezsega[7]=0;

															   break;

															   case 8:
																   dezsega[1]=0;
																   dezsega[2]=0;
																   dezsega[3]=0;
																   dezsega[4]=0;
																   dezsega[5]=0;
																   dezsega[6]=0;
																   dezsega[7]=0;

															   break;

															   case 9:
																   dezsega[1]=0;
																   dezsega[2]=0;
																   dezsega[3]=1;
																   dezsega[4]=0;
																   dezsega[5]=0;
																   dezsega[6]=0;
																   dezsega[7]=0;

															   break;

																}

														switch (unimin)  // decoder para unidade de minutos
																{
																   case 0:
																	   unimina[0]=1;
																	   unimina[1]=1;
																	   unimina[2]=1;
																	   unimina[3]=1;
																	   unimina[4]=1;
																	   unimina[5]=1;
																	   unimina[6]=1;
																	   unimina[7]=1;
																   break;

																   case 1:
																	   unimina[0]=0;
																	   unimina[1]=1;
																	   unimina[2]=1;
																	   unimina[3]=1;
																	   unimina[4]=1;
																	   unimina[5]=0;
																	   unimina[6]=0;
																	   unimina[7]=1;
																   break;

																   case 2:
																	   unimina[0]=0;
																	   unimina[1]=0;
																	   unimina[2]=1;
																	   unimina[3]=0;
																	   unimina[4]=0;
																	   unimina[5]=1;
																	   unimina[6]=0;
																	   unimina[7]=0;
																   break;

																   case 3:
																	   unimina[0]=0;
																	   unimina[1]=0;
																	   unimina[2]=1;
																	   unimina[3]=1;
																	   unimina[4]=0;
																	   unimina[5]=0;
																	   unimina[6]=0;
																	   unimina[7]=0;

																   break;

																   case 4:
																	   unimina[0]=0;
																	   unimina[1]=0;
																	   unimina[2]=0;
																	   unimina[3]=1;
																	   unimina[4]=1;
																	   unimina[5]=0;
																	   unimina[6]=0;
																	   unimina[7]=1;

																   break;

																   case 5:
																	   unimina[0]=0;
																	   unimina[1]=0;
																	   unimina[2]=0;
																	   unimina[3]=1;
																	   unimina[4]=0;
																	   unimina[5]=0;
																	   unimina[6]=1;
																	   unimina[7]=0;

																   break;

																   case 6:
																	   unimina[0]=0;
																	   unimina[1]=0;
																	   unimina[2]=0;
																	   unimina[3]=0;
																	   unimina[4]=0;
																	   unimina[5]=0;
																	   unimina[6]=1;
																	   unimina[7]=0;

																   break;

																   case 7:
																	   unimina[0]=0;
																	   unimina[1]=1;
																	   unimina[2]=1;
																	   unimina[3]=1;
																	   unimina[4]=1;
																	   unimina[5]=0;
																	   unimina[6]=0;
																	   unimina[7]=0;

																   break;

																   case 8:
																	   unimina[0]=0;
																	   unimina[1]=0;
																	   unimina[2]=0;
																	   unimina[3]=0;
																	   unimina[4]=0;
																	   unimina[5]=0;
																	   unimina[6]=0;
																	   unimina[7]=0;

																   break;

																   case 9:
																	   unimina[0]=0;
																	   unimina[1]=0;
																	   unimina[2]=0;
																	   unimina[3]=1;
																	   unimina[4]=0;
																	   unimina[5]=0;
																	   unimina[6]=0;
																	   unimina[7]=0;

																   break;

																}

											//fim do decoder ----------------------------------------------------------------------------------

					//multiplex temporal
							if (millis()>=prx_data)
							{
								prx_data = (millis() + dt_data);

								switch (flag)
								{
									case 0:
										data[0]=1;
										data[1]=decsega[1];
										data[2]=decsega[2];
										data[3]=decsega[3];
										data[4]=decsega[4];
										data[5]=decsega[5];
										data[6]=decsega[6];
										data[7]=decsega[7];
										data[12]=0;
										data[13]=0;
										data[14]=0;
										data[15]=1;
										flag=1;
									break;

									case 1:
										data[0]=0;
										data[1]=unisega[1];
										data[2]=unisega[2];
										data[3]=unisega[3];
										data[4]=unisega[4];
										data[5]=unisega[5];
										data[6]=unisega[6];
										data[7]=unisega[7];
										data[12]=0;
										data[13]=0;
										data[14]=1;
										data[15]=0;
										flag=2;
									break;
									case 2:
										data[0]=1;
										data[1]=dezsega[1];
										data[2]=dezsega[2];
										data[3]=dezsega[3];
										data[4]=dezsega[4];
										data[5]=dezsega[5];
										data[6]=dezsega[6];
										data[7]=dezsega[7];
										data[12]=0;
										data[13]=1;
										data[14]=0;
										data[15]=0;
										flag=3;
									break;

									case 3:
										data[0]=unimina[0];
										data[1]=unimina[1];
										data[2]=unimina[2];
										data[3]=unimina[3];
										data[4]=unimina[4];
										data[5]=unimina[5];
										data[6]=unimina[6];
										data[7]=unimina[7];
										data[12]=1;
										data[13]=0;
										data[14]=0;
										data[15]=0;
										flag=0;
									break;

									default:
									flag=0;
									break;

								}

								serialdata(data); //código para serializar o vetor
							}

					// fim do multiplex temporal

	} // fim do loop

} // fim da função principal



