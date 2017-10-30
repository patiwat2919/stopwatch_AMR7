#include "UART.h"
#include <LPC21xx.H>
#include <stdio.h>

int putchar (int ch);
int getchar (void);
void UART0_initail (void);
void display (void);
void seek2home (void);

void UART0_initial (void)	
{
	VPBDIV = 0x02;
	PINSEL0 |= 0x00000005;
		
	U0LCR = 0x83;
	U0DLL = 0xC2;
	U0LCR = 0x03;	
}

int putchar (int ch) 			  /* Write character to Serial Port    */
{                  		
  if (ch == '\n')  {
    while (!(U0LSR & 0x20));
    U0THR = CR;              	   /* output CR */
  }
  while (!(U0LSR & 0x20));
  return (U0THR = ch);
}

int getchar (void) 				/* Read character from Serial Port   */
{                    

  while (!(U0LSR & 0x01));

  return (U0RBR);
}

void seek2home (void)
{
	putchar(27);
	printf("[H");
}
