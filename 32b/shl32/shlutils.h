#ifndef _SHLU_
#define _SHLU_

#include <types.h>
#include <sys.h>

#include <kernel.h>
#include <timer.h>
#include <floppy.h>
#include <fat.h>


extern unsigned long ram_size;
int print_mem();
int test_delay();
void egg0();
void list_root_dir1(xfloppy f);
void task1();


#endif
