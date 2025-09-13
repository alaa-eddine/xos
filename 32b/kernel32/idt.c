/*
@Auteur : Alaa-eddine KADDOURI
@Projet : X Operating System

@Description :



*/

#include <types.h>
#include <idt.h>
#include <sys.h>
#include <mem.h>

void k_syscall(void);
void k_default_int(void);

void k_int0(void);
void k_int1(void);
void k_int2(void);
void k_int3(void);
void k_int4(void);
void k_int5(void);
void k_int6(void);
void k_int7(void);
void k_int8(void);
void k_int9(void);
void k_int10(void);
void k_int11(void);
void k_int12(void);
void k_int13(void);
void k_int14(void);
void k_int15(void);
void k_int16(void);
void k_int17(void);
void k_int18(void);
void k_irq0(void);
void k_irq1(void);
void k_irq2(void);
void k_irq3(void);
void k_irq4(void);
void k_irq5(void);
void k_irq6(void);
void k_irq7(void);
void k_irq8(void);


struct idtr kidtr;

/* table de IDT */
idtdesc * kidt = (struct idtdesc *)IDTBASE;


/* pointeur sur un descripteur libre dans la IDT */
unsigned int kidtptr = 0;





/*
 * 'init_idt_desc' initialise un descripteur de segment situe en idt.
 * 'desc' est l'adresse lineaire du descripteur a initialiser.
 * L'argument 'type' doit avoir pour valeur INTGATE, TRAPGATE
 * ou TASKGATE.
 */
void init_idt_desc(u32 offset, u16 select, u16 type, idtdesc* desc) {
	desc->offset0_15 = (offset & 0xffff);
	desc->select = select;
	desc->type = type;
	desc->offset16_31 = (offset & 0xffff0000) >> 16;
	return;
}

/*
 * 'add_idt_desc' ajoute dans la IDT le descripteur passe en argument.
 * 'kidtptr', definie dans 'idt.h' est une variable globale correspondant
 * a un index dans la IDT. Elle pointe sur un descripteur vacant dans 
 * la table. Attention, aucun controle n'est effectue pour empeche
 * les debordements de la IDT.
 */
void add_idt_desc(idtdesc desc) {
	kidt[kidtptr++] = desc;	
}

/*
 * Cette fonction initialise la IDT apres que le kernel soit charge 
 * en memoire.
 */
void init_idt(void) {
	idtdesc desc;
	int i;

	/* initialisation des descripteurs systeme */
	for(i=0;i<IDTSIZE;i++) {
		init_idt_desc((u32)k_default_int, 0x08, INTGATE, &desc);
		add_idt_desc(desc);
	}

	init_idt_desc((u32)k_int0, 0x08, INTGATE, &kidt[0]);
	init_idt_desc((u32)k_int1, 0x08, INTGATE, &kidt[1]);
	init_idt_desc((u32)k_int2, 0x08, INTGATE, &kidt[2]);
	init_idt_desc((u32)k_int3, 0x08, INTGATE, &kidt[3]);
	init_idt_desc((u32)k_int4, 0x08, INTGATE, &kidt[4]);
	init_idt_desc((u32)k_int5, 0x08, INTGATE, &kidt[5]);
	init_idt_desc((u32)k_int6, 0x08, INTGATE, &kidt[6]);
	init_idt_desc((u32)k_int7, 0x08, INTGATE, &kidt[7]);
	init_idt_desc((u32)k_int8, 0x08, INTGATE, &kidt[8]);
	init_idt_desc((u32)k_int9, 0x08, INTGATE, &kidt[9]);
	init_idt_desc((u32)k_int10, 0x08, INTGATE, &kidt[10]);
	init_idt_desc((u32)k_int11, 0x08, INTGATE, &kidt[11]);
	init_idt_desc((u32)k_int12, 0x08, INTGATE, &kidt[12]);
	init_idt_desc((u32)k_int13, 0x08, INTGATE, &kidt[13]);
	init_idt_desc((u32)k_int14, 0x08, INTGATE, &kidt[14]);
	init_idt_desc((u32)k_int15, 0x08, INTGATE, &kidt[15]);
	init_idt_desc((u32)k_int16, 0x08, INTGATE, &kidt[16]);
	init_idt_desc((u32)k_int17, 0x08, INTGATE, &kidt[17]);
	init_idt_desc((u32)k_int18, 0x08, INTGATE, &kidt[18]);

	init_idt_desc((u32)k_irq0, 0x08, INTGATE, &kidt[32]);
	init_idt_desc((u32)k_irq1, 0x08, INTGATE, &kidt[33]);
	init_idt_desc((u32)k_irq2, 0x08, INTGATE, &kidt[34]);
	init_idt_desc((u32)k_irq3, 0x08, INTGATE, &kidt[35]);
	init_idt_desc((u32)k_irq4, 0x08, INTGATE, &kidt[36]);
	init_idt_desc((u32)k_irq5, 0x08, INTGATE, &kidt[37]);
	init_idt_desc((u32)k_irq6, 0x08, INTGATE, &kidt[38]);
	init_idt_desc((u32)k_irq7, 0x08, INTGATE, &kidt[39]);
	init_idt_desc((u32)k_irq8, 0x08, INTGATE, &kidt[40]);

	init_idt_desc((u32)k_syscall, 0x08, INTGATE, &kidt[100]);

	/* initialisation de la structure pour IDTR */
	kidtr.limite = IDTSIZE*8;
	kidtr.base = IDTBASE;

	/* recopie de la IDT a son adresse */
//	memcpy((void *)kidtr.base, kidt, kidtr.limite);

	/* chargement du registre IDTR */
	asm("lidtl (kidtr)");
}
