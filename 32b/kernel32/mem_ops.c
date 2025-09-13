/*
@Auteur : Alaa-eddine KADDOURI
@Projet : X Operating System

@Description :



*/



#include <sys.h>
#include <mem.h>
#include <conio.h>
#include <lib.h>
extern unsigned long taille_ram;

unsigned long count_memory(void)
{
	register unsigned long *mem;
	unsigned long mem_count, mem_end, bse_end, a;
	unsigned short memkb;
	unsigned char irq1, irq2;


	irq1=inb(0x21);
	irq2=inb(0xA1);

	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);

	mem_count=0;
	memkb=0;

	asm volatile ("wbinvd");

	do
	{
		memkb++;
		mem_count+=1024*1024;
		mem=(unsigned long*)mem_count;

		a=*mem;

		*mem=0x55AA55AA;

		asm("":::"memory");
		if(*mem!=0x55AA55AA)
			mem_count=0;
		else
		{
			*mem=0xAA55AA55;
			asm("":::"memory");
			if(*mem!=0xAA55AA55)
				mem_count=0;
		}

		asm("":::"memory");
		*mem=a;
	} while(memkb<4096 && mem_count!=0);

	mem_end=memkb<<10;
	mem=(unsigned long*)0x413;
	bse_end=((*mem)&0xFFFF)<<6;

	outb(0x21, irq1);
	outb(0xA1, irq2);

	return mem_end;
}


void dump_mem(char * start)
{
  int i;
  int j=0;
  int k;
  char digit[3];
  
  printk("------------[ DUMP @ 0x%x ]--------------------------------------------------\n",start);
  digit[2] = 0;


  for (i=0; i<512; i++)
    {

      if (!j)
	{
	  printk("0x%x", start+i);
	  gotoxy(11, getY());
	  printk("| ");
	}
      
      j++;
      if (((unsigned long)start[i]&0x000000FF) < 0x10) 
	printk("0");
      printk("%x ", start[i]&0x000000FF);

      if (j>=16)
	{
	  printk("| ");
	  for (k=i-j+1; k<i; k++)
	    if (start[k] > 20)
	      printk("%c", start[k]);
	    else
	      printk(".");

	  printk("\n");
	  j=0;
	}
    }
  printk("===============================================================================\n");
}
