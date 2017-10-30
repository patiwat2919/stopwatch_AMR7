#include "TIMER.h"
#include "UART.h"
#include <LPC21xx.H>
#include <stdio.h>

void T0isr (void)__irq;
void Timer_initial (void);
void delay(unsigned long int count1);
void printTime (void);
void resetTime (void);

unsigned int ms, s, m;

void T0isr (void)__irq
{
	if (ms > 99)
	{
		ms = 0;
	}
	else if (ms == 99)
	{
		s++;
		ms = 0;
	}
	else
	{
		ms++;
	}

	if (s > 59)
	{
		s = 0;
	}
	else if (s == 59)
	{
		m++;
		s = 0;
	}

	printTime();

	T0IR 		|= 0x00000001;			//Clear match 0 interrupt
	EXTINT  	= 0x00000014;			//Clear the peripheral interrupt flag
	VICVectAddr = 0x00000000;			//Dummy write to signal end of interrupt  	
}

void Timer_initial (void)
{

	VPBDIV = 0x02;

	PINSEL0 	|= 0x00000800;
	
	T0PR 		= 0x0000001E;			//Load prescaler
	T0TCR 		= 0x00000002;			//Reset counter and prescaler
	T0MCR 		= 0x00000003;			//On match reset the counter and generate an interrupt
	T0MR0		= 0x000003E8;			//Set the cycle time

	VICVectCntl0    = 0x00000024;						//Set channel
	VICVectAddr0 	= (unsigned)T0isr; 					//Set the timer ISR vector address
	
	VICIntEnable |= 0x00000010;
}

void delay(unsigned long int count1)
{
 	while(count1 > 0) {count1--;}
}

void printTime (void)
{
	seek2home();
	printf ("\r%02d:%02d:%02d\n", m, s, ms);	
}

void resetTime (void)
{
	ms = 0;
	s = 0;
	m = 0;
}
