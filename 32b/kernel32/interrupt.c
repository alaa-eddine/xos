/*
  @Auteur : Alaa-eddine KADDOURI
  @Projet : X Operating System

  @Description :



*/



#include <types.h>
#include <sys.h>
#include <conio.h>
#include <lib.h>
#include <floppy.h>
#include <xosdbg.h>
#include <kbd.h>
#include <panic.h>
#include <scheduler.h>
extern volatile unsigned long int tick_count;



void idt_default_int(void) {
  printk("Unassigned interrupt\n");
}

void idt_syscall(void) {
  printk("XosKERnel32 ::  SysCall\n");
 }

  
void idt_int0(void) {
  printk(" Divide Error\n");
  while(1);
}

void idt_int1(void) {
  printk(" Debug exception\n");
  while(1);
}
void idt_int2(void) {
  printk(" unknown error\n");

  while(1);
}
void idt_int3(void) {
  printk(" Breakpoint\n");

  while(1);
}
void idt_int4(void) {
  printk(" Overflow\n");

  while(1);
}
void idt_int5(void) {
  printk(" Bounds check\n");

  while(1);
}
void idt_int6(void) {
  printk(" Invalid opcode\n");

  while(1);
}
void idt_int7(void) {
  printk(" Coprocessor not available\n");

  while(1);
}
void idt_int8(void) {
  printk(" Double fault\n");

  while(1);
}
void idt_int9(void) {
  printk(" Coprocessor Segment overrun\n");

  while(1);
}
void idt_int10(void) {
  printk(" Invalid TSS\n");

  while(1);
}
void idt_int11(void) {
  printk(" Segment not present\n");

  while(1);
}
void idt_int12(void) {
  printk(" Stack exception\n");

  while(1);
}

void idt_int13(void) {
  panic(" General Protection\n System halted \n\n");

  while(1);
}

void idt_int14(void) {
  printk("\n Page fault at address:  0x%x\n", _read_cr2());

/*asm("pushal \n \
       pushl %esp \n \
       call _dbg_dmp_regs");
       */
asm("pushl %esp \n \
call _dbg_dmp_regs");       
  _panic();
  while(1);
}

void idt_int15(void) {
  printk(" Unknown error\n");

  while(1);
}

void idt_int16(void) {
  printk(" Coprocessor error\n");

  while(1);
}

void idt_int17(void) {
  printk("i17 ");
  while(1);
}

void idt_int18(void) {
  printk("i18 ");
  while(1);
}






void idt_irq0(void) {
  temoin();
  tick_count++;
  switchtask();
  eoi();
}
void idt_irq1(void) {

  kbd_int();
  eoi();

}
void idt_irq2(void) {
  printk("2");
}
void idt_irq3(void) {
  printk("3");
}
void idt_irq4(void) {
  printk("4");
}
void idt_irq5(void) {
  printk("5");
}
void idt_irq6(void) {
#ifdef FDCDEBUG
  printk("\n\nFC CALL\n");  
#endif
  floppy_int();
  eoi();
}
void idt_irq7(void) {
  printk("7");
}
void idt_irq8(void) {
  printk("8");
}
