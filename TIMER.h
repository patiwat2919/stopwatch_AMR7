#ifndef __TIMER_H
#define __TIMER_H

void T0isr (void)__irq;
void Timer_initial (void);
void delay(unsigned long int);
void printTime (void);
void resetTime (void);

#endif
