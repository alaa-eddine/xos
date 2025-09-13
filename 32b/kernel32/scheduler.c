/*
   un scheduler tres simple qui bascule entre le noyeau est une tache
 */

#include <sys.h>
#include <scheduler.h>
#include <conio.h>
#include <lib.h>


extern void jump_to_tss();
#define MAXCOUNT 1

#define MAXTASKS 3
static volatile int activ=0;
static volatile int ke=1;
static volatile int count=MAXCOUNT;


static volatile int locked=0;
//static char stat='S';

volatile char i=0;


char t[MAXTASKS];

void activate()
{
   activ = 1;
   t[0]=1;
   t[1]=0;
   t[2]=0;
}

void imactiv(char j)
{
   t[j]=1;
   i=j;
}

void unactivate()
{
   activ = 0;
}
int isactiv()
{
   return activ;
}

void lock()
{
   locked=1;
}

void unlock()
{
   locked=0;
}


void switchtask()
{
   int tattr, tx, ty, j, notask=-1;
   for(j=0; j<MAXTASKS; j++)
   {
      if (t[j]) notask++;
   }

   tx = getX();
   ty = getY();
   tattr=getattrb();
   gotoxy(0,0);
   attrb(0x0B);
   printk("[SCHED=%d]",i);
   attrb(tattr);
   gotoxy(tx, ty);
   //activ=0;
   if (activ && !locked && notask)
   {
      count--;

      if (!count) {      
	 count=MAXCOUNT;

	 do
	 {
	 i++;	 
	 if (i > 2) i=0;
	 }
	 while(!t[i]);
	 /*activ = 0;*/
	 if (t[i])
	 {
	    outb(0x20, 0x20);
	    jump_to_tss(0x38+(i*8));
	 }
	 /*
	    if (ke)
	    {
	    ke = 0;
	    outb(0x20, 0x20);
	    jump_to_tss(0x38);	  	   	   

	    }
	    else
	    {
	    ke = 1;
	    outb(0x20, 0x20);
	    jump_to_tss(0x40);

	    }
	  */
      }

   }


}
