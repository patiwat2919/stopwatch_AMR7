#ifndef __UART_H
#define __UART_H

#define CR 0x0D

int putchar (int ch);
int getchar (void);
void UART0_initial (void);
void seek2home (void);

#endif
