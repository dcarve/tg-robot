#include "stm32f10x.h"
#include "stm32f1xx_it.h"

#define FREQ_TICK 1000
#define DISPLAY_UPDATE_DELAY 4 // 4 ms (250 Hz)
#define DUAL_DELAY 5000 // 5s

volatile uint8_t started = 0;
uint8_t LED_LOCAL = 0;
uint8_t LED_EXTERNAL = 0;
uint8_t LED_DUAL = 0;
uint8_t DUAL_ENTERED = 0;
uint32_t dual_timing = 0;
uint8_t DUAL_STATE = 0;
void updateDisplay();
void loadBytes(uint8_t a, uint8_t b);

// -- esta secao e nova nesta pratica --
void setup_INT_externa(void) {
	EXTI_InitTypeDef IntExt_PtB;           // declara estrut IntExt_PtB
	NVIC_InitTypeDef NVIC_InitS;             // estrutura NVIC port B
	// configurar dados estrutura interrupcao
	IntExt_PtB.EXTI_Line = EXTI_Line6;           // qual linha pede interrupcao
	IntExt_PtB.EXTI_Mode = EXTI_Mode_Interrupt;  // modo interrupcao
	IntExt_PtB.EXTI_Trigger = EXTI_Trigger_Rising;  // dispara no falling_edge
	IntExt_PtB.EXTI_LineCmd = ENABLE;           // habilita ext_int
	EXTI_Init(&IntExt_PtB);  // chama fun��o que inicializa interrupcao

	// configurar o NVIC (estrutura e funcao no misc.h e misc.c)
	NVIC_InitS.NVIC_IRQChannel = EXTI9_5_IRQn;     // IRQ_ext linha EXTI9_5_IRQn
	NVIC_InitS.NVIC_IRQChannelPreemptionPriority = 1;      // prioridade preempt
	NVIC_InitS.NVIC_IRQChannelSubPriority = 1;       // prioridade 1
	NVIC_InitS.NVIC_IRQChannelCmd = ENABLE;		     // habilitada
	NVIC_Init(&NVIC_InitS);    // chama fn que inicializa NVIC
}

// fn que configura o SysTick c/ 1ms  (freq_systema/frequency
void setup_systick(uint16_t f_tick) {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	(void) SysTick_Config(RCC_Clocks.HCLK_Frequency / f_tick);
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
	GPIO_InitS.GPIO_Mode = GPIO_Mode_IN_FLOATING;
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

void setup_GPIO() {

	// dec ESTRUTURAS dados: GPIO, EXTI (external int) e NVIC (contrl vet interrpt)
	GPIO_InitTypeDef GPIO_InitS;
	// habilitar o clock do periferico de interrupcao
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_StructInit(&GPIO_InitS);
	GPIO_InitS.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitS.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_13 | GPIO_Pin_7; // Pinos para o reg e o led PB7
	GPIO_InitS.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitS);
	GPIO_InitS.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitS.GPIO_Mode = GPIO_Mode_IN_FLOATING; // modo input pino B6
	GPIO_InitS.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitS);

	// Leds do GPIOC
	GPIO_InitS.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitS.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitS.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitS);

	// config que GPIOB pino 6 sera usado para gerar EXT INT
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	EXTI_ClearITPendingBit(EXTI_Line6);
}

// funcao principal do programa
int main(void) {
	setup_INT_externa();                 // setup Interrupcao externa
	setup_systick(FREQ_TICK);            // set timers p/ 1 ms  (1000 Hz)
	setup_GPIO();
	setup_USART(9600);
	while (1) {
		if (!started) {
			started = 1;
			time_start_set();
		} else {
			time_get();
			if (display_timing <= millis()) {
				updateDisplay();
				display_timing = millis() + DISPLAY_UPDATE_DELAY;
			}
		}
		assemblePayload(min1, seg1, seg2, decseg);
		//assemblePayload(1, 2, 3, 4);
		USART_SendBytes(payload, 4); // mandando sempre
	}
}

void updateDisplay() {
	uint8_t m1 = 0, s1 = 0, s2 = 0, ds = 0;
	switch (get_estado_crono()) {
	case LOCAL: {
		if (!LED_LOCAL) {
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
			LED_LOCAL = 1;
		}
		if (LED_EXTERNAL) {
			GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);
			LED_EXTERNAL = 0;
		}
		if (LED_DUAL) {
			GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
			LED_DUAL = 0;
		}
		if (DUAL_ENTERED)
			DUAL_ENTERED = 0;
		if (DUAL_STATE)
			DUAL_STATE = 0;

		m1 = min1, s1 = seg1, s2 = seg2, ds = decseg;
		break;
	}
	case EXTERNAL: {
		if (LED_LOCAL) {
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
			LED_LOCAL = 0;
		}
		if (!LED_EXTERNAL) {
			GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);
			LED_EXTERNAL = 1;
		}
		if (LED_DUAL) {
			GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
			LED_DUAL = 0;
		}
		m1 = min1USART, s1 = seg1USART, s2 = seg2USART, ds = decsegUSART;
		break;
	}
	case DUAL: {
		if (LED_LOCAL && !DUAL_ENTERED) {
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
			LED_LOCAL = 0;
		}
		if (LED_EXTERNAL) {
			GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);
			LED_EXTERNAL = 0;
		}
		if (!LED_DUAL) {
			GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
			LED_DUAL = 1;
		}
		if (!DUAL_ENTERED)
			DUAL_ENTERED = 1;

		uint32_t now = millis();
		if (dual_timing <= now) {
			DUAL_STATE = !DUAL_STATE;
			LED_LOCAL = !LED_LOCAL;
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, !LED_LOCAL);
			dual_timing = now + DUAL_DELAY;
		}

		if (!DUAL_STATE)
			m1 = min1USART, s1 = seg1USART, s2 = seg2USART, ds = decsegUSART;
		else
			m1 = min1, s1 = seg1, s2 = seg2, ds = decseg;

		break;
	}
	default:
		break;
	}
	// PB13 SER
	// PB0  RCK
	// PB1  SCK
	switch (estado_reg) {
	case 0:
		if ((m1 + s1 + s2 + ds) == 0) // Se todos forem zero (funciona pois nunca serão negativos)
				{
			loadBytes(0xFF, DIGIT_4); // Código 7seg para todos apagados (~0x0)
			break;
		}
		loadBytes(seg7(ds), DIGIT_4);
		break;
	case 1:
		if ((m1 + s1 + s2) == 0) // Se os últimos 3 forem zero
				{
			loadBytes(0xFF, DIGIT_4); // Código 7seg para todos apagados (~0x0)
			break;
		}
		loadBytes(~((~seg7(s2)) + 0x1), DIGIT_3); // +0x1 para inserir o ponto
		break;
	case 2:
		if ((m1 + s1) == 0) // Se os últimos 2 forem zero
				{
			loadBytes(0xFF, DIGIT_4); // Código 7seg para todos apagados (~0x0)
			break;
		}
		loadBytes(seg7(s1), DIGIT_2);
		break;
	case 3:
		if (m1 == 0) // Se o último for zero
				{
			loadBytes(0xFF, DIGIT_4); // Código 7seg para todos apagados (~0x0)
			break;
		}
		loadBytes(~((~seg7(m1)) + 0x1), DIGIT_1); // +0x1 para inserir o ponto
		break;
	default:
		estado_reg = 0;
		break;
	}
	estado_reg = (estado_reg + 1) % 4;
}

void loadBytes(uint8_t a, uint8_t b) {
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
	int i;
	for (i = 0; i < 8; i++) {
		GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_13, (a >> i) & 0x1);
		GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);
	}
	for (i = 0; i < 8; i++) {
		GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_13, (b >> i) & 0x1);
		GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);
	}
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
}
