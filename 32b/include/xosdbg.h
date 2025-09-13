#ifndef _XOSDBG_H_
#define _XOSDBG_H_

extern unsigned long _read_cr0();
extern unsigned long _read_cr1();
extern unsigned long _read_cr2();
extern unsigned long _read_cr3();
extern unsigned long _read_cr4();

extern unsigned long _read_dr0();
extern unsigned long _read_dr1();
extern unsigned long _read_dr2();
extern unsigned long _read_dr3();
extern unsigned long _read_dr6();
extern unsigned long _read_dr7();


extern unsigned long _read_stack();
void _dbg_dmp_regs(u32 esp);
void _dbg_print_gdt(int);

#endif
