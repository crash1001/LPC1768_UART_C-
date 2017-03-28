#include <lpc17xx.h>
#include <UART.h>
UART uart1;



int main()
{
//	UART uart1;
	char ch;
	uart1.UARTInit(1,9600);
	while(1)
	{
//	ch = uart1.UARTRecieveCharPolling(1);
//		uart1.UARTSendChar(1,uart1.UARTRecieveCharPolling(1));
//			
	}
	
}

extern "C" void UART1_IRQHandler( void )
{
	char ch;
	while(!(LPC_UART1->LSR & 0x01));
		ch = (LPC_UART1->RBR);

		uart1.UARTSendChar(1,ch);
}

