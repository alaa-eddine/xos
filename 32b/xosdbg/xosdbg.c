#include <types.h>
#include <task.h>
#include <xosdbg.h>
#include <gdt.h>
#include <string.h>
#include <lib.h>
#include <conio.h>
#include <xosdbg.h>
/*
void dump_kern_tss()
{
  printk("");
}
*/

void _dbg_print_gdt(int i)
{
  struct gdtdesc * gdtp=(struct gdtdesc *)GDTBASE;
  char t[]= "SYSTEM   ";
  switch( ((gdtp[i].acces&0x18)>>3))
    {
    case 2 : strcpy(t, "DATA");break;
    case 3 : strcpy(t, "CODE");break;
    }
  printk("GDT is loaded at address 0x%x\n",GDTBASE);
  printk("------------------------------------------------------------\n");
  printk("Base   = 0x%x\n",gdtp[i].base0_15|gdtp[i].base16_23|gdtp[i].base24_31);
  printk("Limit  = 0x%x\n\n", gdtp[i].lim0_15|gdtp[i].lim16_19);

  printk("Type   = 0x%x  : %s\n\n",((gdtp[i].acces&0x18)>>3), t);

  printk("Acces  = 0x%x\n\n",gdtp[i].acces);
  printk("  DPL       0x%x \n",((gdtp[i].acces)&0x60)>>5);
  printk("  PRESENT   0x%x \n",(gdtp[i].acces&PRESENT?1:0));
  printk("  EXPANSION 0x%x \n",(gdtp[i].acces&EXPANSION?1:0));
  printk("  WRITABLE  0x%x \n",(gdtp[i].acces&WRITABLE?1:0));
  printk("  ACCESSED  0x%x \n",(gdtp[i].acces&ACCESSED?1:0));

  printk("\nOther  = 0x%x\n\n",gdtp[i].other);	
  printk("  GRANULARITY 0x%x\n",(gdtp[i].other&GRANULARITY?1:0));
  printk("  AVAILABLE   0x%x\n",(gdtp[i].other&AVAILABLE?1:0));
  printk("------------------------------------------------------------\n\n");
}

void _dbg_dmp_regs(u32 esp)
{  
  printk("------------------------------------------------------------\n");
  printk(" EAX = 0x%x",_read_stack(esp+7*4));
  gotoxy(19, getY());
  printk(" EBX = 0x%x\n", _read_stack(esp+4*4));
  printk(" ECX = 0x%x",_read_stack(esp+6*4));
  gotoxy(19, getY());
  printk(" EDX = 0x%x\n",_read_stack(esp+5*4));
  printk(" ESP = 0x%x",_read_stack(esp+3*4));
  gotoxy(19, getY());
  printk(" EBP = 0x%x\n",_read_stack(esp+2*4));
  printk(" ESI = 0x%x",_read_stack(esp+1*4));
  gotoxy(19, getY());
  printk("EDI = 0x%x\n",_read_stack(esp+0*4));

  printk("\n EIP   = 0x%x \n",  _read_stack(esp+13*4));
  printk(" CS    = 0x%x \n",  _read_stack(esp+14*4));
  printk(" eflags = 0x%x \n",  _read_stack(esp+15*4));  

  printk("\n CR0 = 0x%x",  _read_cr0());
//  printk("  CR1 = 0x%x",  _read_cr1());
  printk("   CR2 = 0x%x\n",  _read_cr2());

  printk(" CR3 = 0x%x",  _read_cr3());
  printk("   CR4 = 0x%x\n",  _read_cr4());
  printk("------------------------------------------------------------\n");
  return;
}
