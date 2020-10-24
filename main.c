#include "TM4C123.h"
#include <stdint.h> 

int count = 0; 

int main()
{
	SYSCTL->RCGCTIMER |= 0x1; // Enable the clock to Timer0 
	TIMER0->CTL &= ~0x1; // Disable the timer before configuration    
	TIMER0->CFG = 0x0; // Select the 32-bit timer configuration  
	TIMER0->TAMR = 0x2; // Select periodic mode 
	TIMER0->TAILR = 0x4C4B400; // Write this value for 5 sec since the clock frequency is 16 MHz
	TIMER0->IMR |= 0x1; // Enable time-out interrupt 
	NVIC->ISER[0] |= 0x80000; // Enable Timer0A interrupt by setting the bit 19 
	TIMER0->CTL |= 0x1; // Enable the timer and start counting 
	SYSCTL->RCGCGPIO |= 0x20; //Enable the clock to PortF 
	GPIOF->LOCK = 0x4C4F434B; // Unlock the GPIOCR register  
	GPIOF->CR |= 0x1; // Uncommit PF0 by setting bit 0   
	GPIOF->PUR |= 0x11; // Enable weak pull up resistor of PF0,PF4 
	GPIOF->DIR |= 0xA; // Configure PF1,PF3 to be an output
	GPIOF->DIR &= ~0x11; // Configure PF0,PF4 to be an input
  	GPIOF->DEN |= 0x1B; // Enable digital functions for PF0,PF1,PF3,PF4
	GPIOF->DATA = 0x2; // Turn on the red LED 
	while(1) // Wait for interrupt if push buttons are not pressed  
	{
		if((GPIOF->DATA & 0x11) == 0x0) // if both push buttons are pressed 
		{
			GPIOF->DATA = 0x8; // Turn on the green LED 
			count = 2; // When push buttons are unpressed first turn on the red LED 
		}
	}		 	
	return 0; 
}

void TIMER0A_Handler()
{ 
	if(count == 0)
	{
		TIMER0->ICR |= 0x1; // Clear the status flag
		GPIOF->DATA = 0xA; // Turn on the red and green LEDs for yellow light  
		count++;
	}
	else if(count == 1) 
	{
		TIMER0->ICR |= 0x1; // Clear the status flag
	  GPIOF->DATA = 0x8; // Turn on the green LED   
		count++; 
	}
	else if(count == 2)
	{
		TIMER0->ICR |= 0x1; // Clear the status flag
		GPIOF->DATA = 0x2; // Turn on the red LED  
		count = 0; 
	}
}
