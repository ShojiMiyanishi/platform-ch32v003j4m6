// Could be defined here, or in the processor defines.
#define SYSTEM_CORE_CLOCK 48000000

#include "ch32v00x.h"
#include <stdio.h>
#include "debug.h"

#define APB_CLOCK SYSTEM_CORE_CLOCK

// Blinky pin is PC2!
// Blinky pin is PC4!
// Blinky pin is PA2!
// Button pin is PD6!

int main()
{
	SystemInit48HSI();

	int cfglr=0;
	// Enable GPIOC.
	//RCC->APB2PCENR |= RCC_APB2Periph_GPIOA;
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD;
	// Enable GPIOD and UART.
	RCC->APB2PCENR |= RCC_APB2Periph_USART1;
	RCC->APB2PCENR |= RCC_APB2Periph_AFIO;


	// GPIO C1 Push-Pull, 10MHz Slew Rate Setting
	GPIOC->CFGLR &= ~(0xf<<(4*2));//PC2
	GPIOC->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*2);//PC2
	//GPIOC->CFGLR &= ~(0xf<<(4*4));//PC4
	//GPIOC->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*4);//PC4
	GPIOC->CFGLR &= ~(0xf<<(4*4));//PC4 zero clear
	GPIOC->CFGLR |= ( GPIO_CNF_IN_FLOATING)<<(4*4);//PC4
	//GPIOD->CFGLR &= ~(0xf<<(4*6));//PD6 zero clear
	//GPIOD->CFGLR |= ( GPIO_CNF_IN_FLOATING)<<(4*6);//PD6
	cfglr=GPIOC->CFGLR ;
	// PD6   AlterFunction TX 
	AFIO->PCFR1 &= ~((1<<21)|(1<<2));//UART1_REMAP clear
	//AFIO->PCFR1 &= ~(0x00400004);//UART1_REMAP clear
	AFIO->PCFR1 |= ((1<<21)|(0<<2));//PD6=TX
	//AFIO->PCFR1 |=  (0x00400000);//PD6=TX
	GPIOD->CFGLR &= ~(0xf<<(4*6));//PD6
	GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<(4*6);//PD6
	// 115200, 8n1.  Note if you don't specify a mode, UART remains off even when UE_Set.
	USART1->CTLR1 = USART_WordLength_8b | USART_Parity_No | USART_Mode_Tx;
	USART1->CTLR2 = USART_StopBits_1;
	USART1->CTLR3 = USART_HardwareFlowControl_None;

	int baud=115200;
	int div,div16,frac;
	div16=APB_CLOCK/baud;
	div=(div16/100)<<4;
	frac=div16-(100*(div>>4));
	USART1->BRR = div;
	USART1->CTLR1 |= CTLR1_UE_Set;
	//SetupUART(115200);
	Delay_Ms( 1000 );
	printf("SystemClk: %u\r\n", (unsigned)SYSTEM_CORE_CLOCK);
	//printf("DeviceID: %08x\r\n", (unsigned)DBGMCU_GetDEVID());
	printf("cfglr : %04x",cfglr);
	printf("This is printf example\r\n");

	int status=0,last=0;
	while(1)
	{
		//status = (GPIOA->INDR) & (1<<2);//PA4
		status = (GPIOC->INDR) & (1<<4);//PC4
		if(status!=last){
			printf("status : %08x\n",status);
			if (status!=0){
				GPIOC->BSHR = 1 << 2;	 // Turn on GPIOC2
				//GPIOC->BSHR = 1 << 4;	 // Turn on GPIOC4
				//GPIOD->BSHR = 1 << 6;	 // Turn on GPIOD6
				//GPIOA->BSHR = 1 << 2;	 // Turn on GPIOC1
			}else{
				GPIOC->BCR = 1 << 2;    // Turn off GPIOC2
				//GPIOC->BCR = 1 << 4;    // Turn off GPIOC4
				//GPIOD->BCR = 1 << 6;    // Turn off GPIOD6
				//GPIOA->BCR = 1 << 2;    // Turn off GPIOD6
			}
			last=status;
			Delay_Ms( 100 );
		}
	}
}
void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void NMI_Handler(void) {
				GPIOC->BSHR = 1 << 2;	 // Turn on GPIOC2

}
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void)
{
	while (1)
	{
	}
}