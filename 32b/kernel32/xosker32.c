/*
   @Auteur : Alaa-eddine KADDOURI
   @Projet : X Operating System

   @Description : Le noyau 32bits de X-OS



 */

#include <types.h>
#include <sys.h>
#include <kbd.h>
#include <kernel.h>
#include <timer.h>
#include <floppy.h>
#include <fat.h>
#include <conio.h>
#include <string.h>
#include <mem_ops.h>
#include <xosdbg.h>
#include <lib.h>
#include <scheduler.h>
#include <mm.h>
#include <xosmalloc.h>
#include <pic.h>
#include <task.h>
#include <idt.h>
#include <gdt.h>
#include <kerinfo.h>
#include <panic.h>
#include <vesa.h>

unsigned long ram_size;




unsigned int kernelbase = 0x100000;
unsigned long * kernelsize=(unsigned long * )0x90FF2;

vbe_info * vbe=(vbe_info *)VESABASE;

xfloppy floppy;
char kernelver[] = "0.56";

char xosver[] = "0.3.4";


int xs32(void);

int _main( void* mbd, unsigned int magic ) {
	//  struct _tss * test=(_tss)0x9000;
	//  struct gdtdesc * gptr=0x800;

	fdmotor_off();		//D'abord on eteind le moteur de la disquette s'il est encore allumé
	//kbd_led(0);		//on eteind toutes les leds du clavier ??

	attrb(0x1F);
	clrscr();
        
	printk("\n\nXOSKER32 :: XOS-KERnel32 loaded at address 0x%x\nkernel size = %d bytes\n", kernelbase, *kernelsize);
	printk("XOSKER32 :: Starting XOS-KERnel32 v%s    \n", kernelver);
	printk("            Compiled on %s\\%s\\%s ", compilday, compilmonth, compilyear);
	//  printk("            Compilé le %d\\%d\\%d ",compilmonth,compilyear);
#ifdef DEBUG
	attrb(0x4F);
	printk("!!! DEBUG FUNCTIONS ENABLED !!!");
	attrb(0x1F);
#endif

	printk("\n\n");

	attrb(0x13);
	XOSmsgln("Detecting hardware necessary for X-OS");
	//Cette partie du code va detecter le materiel installé
	//de nouvelles détections serons rajoutés ici au fur et à mesures des besoins de X-OS

	detect_floppy();
	star();
	if(isvesa(vbe))
	{
		printk("VESA video card: ");
		print_vinfo(vbe);
		printk("      %s \n", get_oemname(vbe));
		printk("      VRAM size: %dKB\n", get_vram(vbe));
	}
	else printk("No VESA card detected\n");
	star();
	printk("Installed RAM: ");
	ram_size = count_memory();
	printk("%dMB \n",ram_size/1024);  
	////////////////////////////////////////////////////////////////////////////

	XOSmsgln("Completed");
	attrb(0x1F);

	printk("---=[ X-OS v%s ]=---------------------------------------------------------\n", xosver);

	printk("Loading new GDT...");
	init_gdt();
	ok();

	printk("Initializing task manager...");
	init_task();
	asm ("ltr %%ax": :"a"(0x38));    //from this point we consider task 0 as the kernel
	ok();



	printk("Initializing IDT...");
	init_idt();
	ok();

	printk("Initializing PIC...");
	PIC_init();
	ok();






	printk("Initializing timer...");
	set_timer(FREQ_SYS);
	ok();

	printk("Enabling interrupts...");
	sti;
	ok();

	printk("_irq_unmasq IRQ1 (%s)", kb_drvid());
	irq_unmask(1);
	ok();

	printk("_irq_unmasq IRQ6 (Floppy)");
	irq_unmask(6);
	ok();


#ifdef XOS_PAGING
	printk("Enabling paging...");
	cli;
	set_paging();
	init_mem();
	sti;
	ok();
	
	printk("Initializing dynamic allocation manager !!Beta!! ...");
	init_cache();
	ok();
#else
	printk("!!WARNING!! paging is not enabled, to enable it recompile X-OS with XOS_PAGING option\n");
#endif



	printk("Mounting floppy drive...");
	mountfloppy(&floppy);
	ok();

	printk("Enabling numeric keypad...");
	switch_num_lock();
	ok();

	printk("Enabling scheduler...");
	activate();
	ok();
	printk("-------------------------------------------------------------------------------\n");
	printk("XOSKER32 :: System OK  (^_^)   ...   Starting XS32 \n\n");


	xs32();     //lancement du shell integré...
	_panic();
	while(1);
}


