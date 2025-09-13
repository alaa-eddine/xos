#ifndef TIMER_H
#define TIMER_H

#define Timer_port  0x40
#define Timer_cr    0x43

#define LOW_BYTE(n) (n & 0x00ff)
#define HIGH_BYTE(n) ((n>>8)&0x00ff)

void set_timer(float freq);
void delay(unsigned int ms);
 
#endif



