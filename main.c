#include <LPC21xx.H>
#include <stdio.h>
#include "UART.h"
#include "TIMER.h"

void EXTINT1VectoredIRQ (void)__irq;
void EXTINT2VectoredIRQ (void)__irq;
void EXTINT3VectoredIRQ (void)__irq;

unsigned int start_flag, reset_flag;

void main (void)
{
	start_flag = 0;
	reset_flag = 0;

	PINSEL0 |= 0x80000000;			//Enable the EXTINT2 interrupt
	PINSEL0 |= 0x20000000;			//Enable the EXTINT1 interrupt
	PINSEL0 |= 0x000C0000;			//Enable the EXTINT3 interrupt
	
	Timer_initial();
	UART0_initial();                                                                                      

	printTime();
                                                                                                                                                        	                                    
	VICVectCntl1 	= 0x00000020 | 0x0000000F;  		//select a priority slot for a given interrupt
	VICVectAddr1 	= (unsigned)EXTINT1VectoredIRQ;		//pass the address of the IRQ into the VIC slot
	VICVectCntl2 	= 0x00000020 | 0x00000010;  		//select a priority slot for a given interrupt
	VICVectAddr2 	= (unsigned)EXTINT2VectoredIRQ;		//pass the address of the IRQ into the VIC slot
	VICVectCntl3 	= 0x00000020 | 0x00000011;  		//select a priority slot for a given interrupt
	VICVectAddr3 	= (unsigned)EXTINT3VectoredIRQ;		//pass the address of the IRQ into the VIC slot

	VICIntEnable 	|= 0x00020000;						//enable EXTINT3 interrupt
	VICIntEnable 	|= 0x00010000;						//enable EXTINT2 interrupt
	VICIntEnable 	|= 0x00008000;						//enable EXTINT1 interrupt

	while(1);
}

void EXTINT1VectoredIRQ (void)__irq
{
	if (start_flag == 0)
	{
		start_flag = 1;
		reset_flag = 1;
		T0TCR 		= 0x00000001;			//enable timer
	}

	EXTINT  = 0x00000002;					//Clear the peripheral interrupt flag
	VICVectAddr 	= 0x00000000;			//Dummy write to signal end of interrupt
}
void EXTINT2VectoredIRQ (void)__irq
{
	if (start_flag == 1)
	{
		start_flag = 0;
		reset_flag = 0;
		T0TCR 		= 0x00000000;			//disable timer
	}

	EXTINT  = 0x00000004;					//Clear the peripheral interrupt flag
	VICVectAddr 	= 0x00000000;			//Dummy write to signal end of interrupt
}
void EXTINT3VectoredIRQ (void)__irq
{
	if (reset_flag == 0)
	{
		resetTime();
		reset_flag = 1;
		printTime();
	}

	EXTINT  = 0x00000008;					//Clear the peripheral interrupt flag
	VICVectAddr 	= 0x00000000;			//Dummy write to signal end of interrupt
}
