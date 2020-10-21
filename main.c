#include "TM4C123.h"
#include <stdint.h> 

int main()
{
	SYSCTL->RCGCTIMER |= 0x1; // Enable the clock to Timer0 
	TIMER0->CTL &= ~0x1; // Disable the timer before configuration    
	TIMER0->CFG = 0x0; // Select the 32-bit timer configuration  
	TIMER0->TAMR = 0x2; // Select periodic mode 
	TIMER0->TAILR = 0xF42400; // Write this value for 1 sec since the clock frequency is 16 MHz
	TIMER0->IMR |= 0x1; // Enable time-out interrupt 
	NVIC->ISER[0] |= 0x80000; // Enable Timer0A interrupt by setting the bit 19 
	TIMER0->CTL |= 0x1; // Enable the timer and start counting 
	SYSCTL->RCGCGPIO |= 0x20; //Enable the clock to PortF 
	GPIOF->DIR |= 0x2; // Configure PF1 to be an output
  GPIOF->DEN |= 0x2; // Enable digital functions for PF1
	GPIOF->DATA &= ~0x2; // Turn off the red LED(which is connected to PF1)
	while(1); // Wait for interrupt 
	return 0; 
}

void TIMER0A_Handler()
{
	TIMER0->ICR |= 0x1; // Clear the status flag
	GPIOF->DATA = ~GPIOF->DATA; // Toggle the red LED   
}