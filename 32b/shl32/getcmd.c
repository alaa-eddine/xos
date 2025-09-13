#include <types.h>
#include <sys.h>
#include <kbd.h>
#include <conio.h>
#include <string.h>
#include <mem_ops.h>
#include <lib.h>

unsigned char sc=0;
char got_command=0;
unsigned char ptr=0;
unsigned char maxptr=0;

void got_enter();
void got_backspace();
void got_numlock();
void got_scrolllock();
void got_capslock();
void got_lshiftup();
void got_rshiftup();
void got_lshiftdown();
void got_rshiftdown();
void got_lctrlup();
void got_rctrldown();
void got_laltup();
void got_laltdown();
void got_normalkey();

void getcmd(char * command)
{

   while (!got_command)
   {
      sc = getSC();
      if (sc==0xE0)
      {		
	 sc=getSC();
      }
      else sc--;

      switch(sc){
	 case 0x1B : got_enter();break;

	 case 0x0D : got_backspace();break;

	 case 0x44 : got_numlock();break;

	 case 0x45 : got_scrolllock();break;

	 case 0x39 : got_capslock();break;

	 case 0x35 : got_lshiftup();break;
	 case 0x29 : got_rshiftup();break;
	 case 0xB5 : got_lshiftdown();break;
	 case 0xA9 : got_rshiftdown();break;

	 case 0x1C : got_lctrlup();break;
	 case 0x9C : got_rctrldown();break;

	 case 0x37 : got_laltup();break;
	 case 0xB7 : got_laltdown();break;

	 default : got_normalkey();

      }
   }
}


void got_enter()
{
   got_command=1;
}

void got_backspace()
{

}


