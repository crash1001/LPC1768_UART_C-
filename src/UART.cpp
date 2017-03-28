#include "UART.h"


#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

UART::UART()
{
	UART0Status=0;
	UART1Status=0;
//	UART0_Init_Status=0;
//	UART1_Init_Status=0;
	UART0TxEmpty=0;
	UART1TxEmpty=0;
	UART0Count=0;
	UART1Count=0;
}


void UART::UARTInit(uint8_t UARTPort, uint32_t baudrate)
{
	uint32_t Fdiv;
	uint32_t pclkdiv, pclk;
	if(UARTPort == 0 )
	{
		//POWERING UP UART0
		LPC_SC->PCONP |= (1<<3);
		
		//PHERIPHERAL CLOCK SELECTION
		LPC_SC->PCLKSEL0 &= ~(3<<6);		//MASKING
		LPC_SC->PCLKSEL0 |= (0<<6);			//PCLK = CCLK/4
		pclk=SystemCoreClock/4;
		
		//Set P0.2 as TXD0 and set P0.3 as RXD1
		LPC_PINCON->PINSEL0 &= ~(0x30);	//MASKING
		LPC_PINCON->PINSEL0 |= (2<<4);	//TXD0
		
		LPC_PINCON->PINSEL0 &= ~(0xC0);	//MASKING
		LPC_PINCON->PINSEL0 |= (2<<6);	//RXD1
		
		//INITIALIZATION	
		LPC_UART0->LCR = 0x83;					//8N1, DLAB=1
		Fdiv = (pclk/16) / baudrate;
		LPC_UART0->DLM = Fdiv / 256;
		LPC_UART0->DLL = Fdiv % 256;
		LPC_UART0->LCR = 0x03;					//8N1, DLAB=1
		LPC_UART1->FCR = 0x07;					//FIFO EN, RX/TX FIFO RESET
		
		NVIC_EnableIRQ(UART0_IRQn);
//		LPC_UART0->IER = 0x07;					//FIFO EN, RX/TX FIFO RESET
//LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART1 interrupt */
//LPC_UART1->IER |=  IER_RBR;
//		UART1_Init_Status=1;
	}
	
	if(UARTPort == 1)
	{
		//POWERING UP UART1
		LPC_SC->PCONP |=(1<<4); 
		
		//PHERIPHERAL CLOCK SELECTION
		LPC_SC->PCLKSEL0 &= ~(3<<8);		//MASKING
		LPC_SC->PCLKSEL0 |= (0<<8);			//PCLK = CCLK/4
		pclk = SystemCoreClock / 4;
		
		//Set P2.0 as TXD1 and set P2.1 as RXD1
		LPC_PINCON->PINSEL4 &= ~(0x3);	//MASKING
		LPC_PINCON->PINSEL4 |= (2<<0);	//TXD1
		
		LPC_PINCON->PINSEL4 &= ~(0xC);	//MASKING
		LPC_PINCON->PINSEL4 |= (2<<2);	//RXD1
		
		//INITIALIZATION	
		LPC_UART1->LCR = 0x83;					//8N1, DLAB=1
		Fdiv = (pclk/16) / baudrate;
		LPC_UART1->DLM = Fdiv / 256;
		LPC_UART1->DLL = Fdiv % 256;
		LPC_UART1->LCR = 0x03;				//8N1, DLAB=0
		LPC_UART1->FCR = 0x07;				//FIFO EN, RX/TX FIFO RESET
		
		//ENABLE INTERRUPT
		NVIC_EnableIRQ(UART1_IRQn);
		LPC_UART1->IER = 0x05;			//Enable UART1 Interrupt RBR,THRE,RLS
//		
//		UART1_Init_Status=1;sdf
	}
	
}
void UART::UARTSendChar(uint8_t UARTPort,char ch)
{
	if(UARTPort==0)
	{
		while(!(LPC_UART0->LSR & (1<<5)));
		LPC_UART0->THR=ch;
	}
	if(UARTPort==1)
	{
		while(!(LPC_UART1->LSR & (1<<5)));
		LPC_UART1->THR=ch;
	}
}
char UART::UARTRecieveCharPolling(uint8_t UARTPort)
{
	char ch;
	if(UARTPort==0)
	{
	while((LPC_UART0->LSR & LSR_RDR) ==0);
		ch=LPC_UART0->RBR;
		return ch;
	}
	if(UARTPort==1)
	{
		while(!(LPC_UART1->LSR & 0x01));
		return (LPC_UART1->RBR);
		
}
	}
