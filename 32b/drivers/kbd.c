/*
   @Auteur : Alaa-eddine KADDOURI
   @Projet : X Operating System

   @Description :



 */
#define _KBD_DRIVER_
#include <kbdmap.h>
#include <types.h>
#include <sys.h>
#include <kbd.h>
#define SCROLL_LOCK 1
#define NUM_LOCK 2
#define CAPS_LOCK 4

char id[] = "MODULE_Keyb";



static int waitsc=1;
static unsigned int sc;
static char kbdstatus = 0;

static short caps=0;  //caps lock
static short num=0;  //caps lock


short isnum()
{
  return num;
}

short iscaps()
{
  return caps;
}


/* 
   [ Get ScanCode ]
   cette fonction permet de lire le scancode de la 
   derniere touche pressee ou relachee.
 */
char getSC(void)
{  
  waitsc=1;		// attente d'un ScanCode
  while(waitsc);		// waitsc sera remis a 0 par l'IRQ clavier lors de l'appuis sur une touche
  return sc;
}
/*[/Get ScanCode]*/



/*
   [ IRQ clavier ]

   @desc : 
   L'IRQ clavier est invoquee lors de l'appuis sur une touche
   elle lit le port 0x64 et test le bit de poid le plus faible
   si celui ci est a 1 alors un ScanCode est pret a etre lu,
   dans ce cas on lit le ScanCode via le port 0x60

 */
void kbd_int(void) {	               

  do {			      
    sc=inb(0x64);	         //Lecture du port 0x64
  } while(!(sc & 0x01));       //test du bit le plus faible

  sc=inb(0x60);			//Un scan code est pret on le recupere dans sc
  waitsc=0;
}

/*[/IRQ clavier]*/




char * kb_drvid(void)
{
  return (char *)id;
}

void kbd_led(char status)
{
  int st;

  do {
    st = inb(0x64);
  } while (st&0x02);

  outb(0x60, 0x0ed);
  do {
    st = inb(0x64);
  } while (st&0x02);

  outb(0x60, status&0x07);

}

void switch_num_lock()
{
  if (num) num=0; else num=1;
  if(!(kbdstatus&NUM_LOCK))  
  {
    kbdstatus |= NUM_LOCK;
  }
  else    
    kbdstatus &= (CAPS_LOCK|SCROLL_LOCK);


  kbd_led(kbdstatus);   
}

void switch_caps_lock()
{
  if (caps) caps=0; else caps=1;
  if(!(kbdstatus&CAPS_LOCK)) 
  {
    kbdstatus |= CAPS_LOCK;
  }
  else    
    kbdstatus &= (NUM_LOCK|SCROLL_LOCK);


  kbd_led(kbdstatus);   
}

void switch_scroll_lock()
{

  if(!(kbdstatus&SCROLL_LOCK)) 
  {
    kbdstatus |= SCROLL_LOCK;
  }
  else    
    kbdstatus &= (NUM_LOCK|CAPS_LOCK);


  kbd_led(kbdstatus);   
}

char key(unsigned int val)
{
  return kbdmap[val];
}

int set_keymap(keymaps km)
{
  if( (km > km_en) || (km < km_fr) ) return 0;
  switch (km)
  {
    case km_fr : kbdmap=kbd_fr;break;
    case km_en : kbdmap=kbd_en;break;
  }
  return 1;
}
