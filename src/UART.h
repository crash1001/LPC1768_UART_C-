#include <LPC17xx.h>

class UART
{

	private:
			volatile uint32_t UART0Status, UART1Status;
//			 int UART0_Init_Status, UART1_Init_Status;
			volatile uint8_t	UART0TxEmpty, UART1TxEmpty;
			volatile uint8_t UART0Buffer[128], UART1Buffer[128];
			volatile uint32_t UART0Count, UART1Count;
			
	
	public:
		UART();
		void UARTInit(uint8_t UARTPort, uint32_t buadrate);
		void UARTSendChar(uint8_t UARTPort,char ch);
		char UARTRecieveCharPolling(uint8_t UARTPort);

		
};