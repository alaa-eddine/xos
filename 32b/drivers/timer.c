
/*
   @Auteur : Alaa-eddine KADDOURI
   @Projet : X Operating System

   @Description :



 */

#include <sys.h>
#include <timer.h>
#include <kernel.h>

unsigned long int tick_count;


void set_timer(float freq)
{
   unsigned int rate=0;
   tick_count=0;
   rate=1193182/freq;
   outb(Timer_cr, 0x36);
   outb(Timer_port, (unsigned char) rate);
   outb(Timer_port, (unsigned char) (rate>>8));
}

void delay(unsigned int ms)
{

/*
unsigned long int target_tick = (ms/1000.0)*FREQ_SYS;

   target_tick += tick_count;
   while (tick_count < target_tick);
   return;
*/	    


unsigned long int n=ms*FREQ_SYS/1000;
   unsigned long int target=tick_count+n;
   while(tick_count < target);
   
}




