/*
 * "include/gdt.h"
 */
#include <types.h>
#ifndef __GDT_H
#define __GDT_H



#define GRANULARITY 0x8
#define D 4
//#define B 4
//#define ZERO 2
#define AVAILABLE 1


#define PRESENT 128


#define DPL0 0x00
#define DPL1 0x20
#define DPL2 0x40
#define DPL3 0x60

#define S 16

//#define 8
#define EXPANSION 4
#define WRITABLE 2 
#define ACCESSED 1

#define T_STACK 7



#define GDTBASE	0x80800/* addr. physique ou doit resider la gdt */
#define GDTSIZE	0xFF	/* nombre max. de descripteurs dans la table */



/* descripteur de segment */
struct gdtdesc {
	u16 lim0_15;	
	u16 base0_15;
	u8 base16_23;
	u8 acces;
	u8 lim16_19 : 4;
	u8 other : 4;
	u8 base24_31;
} __attribute__ ((packed));

/* registre GDTR */
struct gdtr {
	u16 limite ;
	u32 base ;
} __attribute__ ((packed));





void init_gdt_desc(u32 base, u32 limite, u8 acces, u8 other, struct gdtdesc* desc);
void init_code_desc(u32 base, u32 limite, struct gdtdesc* desc);
void init_data_desc(u32 base, u32 limite, struct gdtdesc* desc);
void add_gdt_desc(struct gdtdesc desc);
void init_gdt(void);
#endif
