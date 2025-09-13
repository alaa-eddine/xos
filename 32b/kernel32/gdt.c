/*
@Auteur : Alaa-eddine KADDOURI
@Projet : X Operating System

@Description :



*/


#include <types.h>
#include <gdt.h>
#include <task.h>
#include <mm.h>

#define stack_size      1024
#define max_tasks       2




static struct gdtr kgdtr;
/* table de GDT */
//struct gdtdesc kgdt[GDTSIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static struct gdtdesc * kgdt = (struct gdtdesc *)GDTBASE;

/* pointeur sur un descripteur libre dans la GDT */
static unsigned int kgdtptr = 1;



/*
 * 'init_desc' initialise un descripteur de segment situe en gdt ou en ldt.
 * 'desc' est l'adresse lineaire du descripteur a initialiser.
 */
void init_gdt_desc(u32 base, u32 limite, u8 acces, u8 other, struct gdtdesc* desc) {
	desc->lim0_15 = (limite & 0xffff);
	desc->base0_15 = (base & 0xffff);
	desc->base16_23 = (base & 0xff0000)>>16;
	desc->acces = acces;
	desc->lim16_19 = (limite & 0xf0000)>>16;
	desc->other = (other & 0xf);
	desc->base24_31 = (base & 0xff000000)>>24;
	return;
}

/*
 * 'init_code_desc' initialise un descripteur de segment de code.
 * 'init_data_desc' initialise un descripteur de segment de data.
 * 'desc' est l'adresse lineaire du descripteur a initialiser.
 */
void init_code_desc(u32 base, u32 limite, struct gdtdesc* desc) {
	init_gdt_desc(base, limite, 0x9a, 0x0D, desc);
}

void init_data_desc(u32 base, u32 limite, struct gdtdesc* desc) {
	init_gdt_desc(base, limite, 0x92, 0x0D, desc);
}

/*
 * 'add_gdt_desc' ajoute dans la GDT le descripteur passe en argument.
 * 'kgdtptr', definie dans 'gdt.h' est une variable globale correspondant
 * a un index dans la GDT. Elle pointe sur un descripteur vacant dans 
 * la table. Attention, aucun controle n'est effectue pour empeche
 * les debordements de la GDT.
 */
void add_gdt_desc(struct gdtdesc desc) {
	kgdt[kgdtptr++] = desc;
}

/*
 * Cette fonction initialise la GDT apres que le kernel soit charge 
 * en memoire. Une GDT est deja operationnelle, mais c'est celle qui
 * a ete initialisee par le secteur de boot et qui ne correspond
 * pas forcement a celle que l'on souhaite pour bosokernel.
 */
void init_gdt(void) {

  struct gdtdesc kcode, kdata, kstack, tcode, tdata, text;

	
  /* initialisation des descripteurs de segment */

  //0x8 Code 
  init_code_desc(0x0, 0xFFFFF, &kcode);    //Code du kernel 0 --> 1 Go
  //0x10 Data 
  init_data_desc(0x0, 0xFFFFF, &kdata);    //donnée du kernel 0 --> 1 Go



  //0x18 Stack 
  init_gdt_desc(0, 0x10, PRESENT|S|T_STACK, GRANULARITY|D|AVAILABLE, &kstack);

  //0x20 video seg
  init_gdt_desc(0xB8000, 0xFFFF, 0x92|0x60, 0x0D, &text);

  //0x28 task code
  init_gdt_desc(0x0, 0xFFFFF, 0x9a, GRANULARITY|D, &tcode);
  //0x30 task data
  init_gdt_desc(0x0, 0xFFFFF, 0x92, GRANULARITY|D, &tdata);



  add_gdt_desc(kcode);
  add_gdt_desc(kdata);
  add_gdt_desc(kstack);
  add_gdt_desc(text);
  add_gdt_desc(tcode);
  add_gdt_desc(tdata);

	
  /* initialisation de la structure pour GDTR */
  kgdtr.limite = GDTSIZE*8;
  kgdtr.base = GDTBASE;

  /* recopie de la GDT a son adresse */	
  //memcpy(kgdtr.base, kgdt, kgdtr.limite);


  /* chargement du registre GDTR */
  asm volatile("lgdt %0" : : "m" (kgdtr));

  /* initialisation des segments */
  asm volatile("movw $0x10,%%ax	\n\t"
		"movw %%ax, %%ds	\n\t"
		"movw %%ax, %%es	\n\t"
		"movw %%ax, %%fs	\n\t"
		"movw %%ax, %%gs	\n\t"
		"movw %%ax, %%ss	\n\t"
		"movl $0x10FFF,%%esp	\n\t"
		"ljmp $0x08, $1f	\n\t"
		"1:	\n\t" : : : "memory");

 
}