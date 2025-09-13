#include <types.h>
#include <gdt.h>
#include <task.h>
#include <gdt.h>
#include <mem.h>
#include "../mm/mm.h"

#define stack_size      1024
#define max_tasks       3


char task_stack[max_tasks-1][stack_size]; /* stacks for tasks (except main()) */
char pl0_stack[max_tasks-1][stack_size];  /* pl#0 stacks for tasks (except main()) */
void task1();	
void task2();	
extern void v86();
int xs32(void);

void init_task()
{
//  u32 gbase, gsize;
  int i;
  struct gdtdesc ktss, tss, tss2;

  struct _tss * xtss=(struct _tss *)TSSBASE;

  //0x38 tss du kernel
  init_gdt_desc((u32)&xtss[0], sizeof(struct _tss), PRESENT|0x09, GRANULARITY, &ktss);

  //0x40 tss de la  tache 1
  init_gdt_desc((u32)&xtss[1], sizeof(struct _tss), PRESENT|0x09|DPL3, GRANULARITY, &tss);

  //0x40 tss de la  tache 2
  init_gdt_desc((u32)&xtss[2], sizeof(struct _tss), PRESENT|0x09|DPL3, GRANULARITY, &tss2);

  memset(xtss, 0, 512);

  for(i=0; i<max_tasks; i++)
    {      
      xtss[i]._ldtss=0;
      xtss[i]._t=0;
      xtss[i]._iomapaddr=sizeof(struct _tss);
      xtss[i]._cr3 = (u32)_PD_ADDR_;
      if (i)
	{
	  xtss[i]._fs = xtss[i]._gs = 0x30;
	  xtss[i]._ds = xtss[i]._es = xtss[i]._ss = 0x30;
	  xtss[i]._cs = 0x28;
	  xtss[i]._esp = (u32)&task_stack[i];
	  xtss[i]._ss0 = 0x30;
	  xtss[i]._esp0 = (u32)&pl0_stack[i];	

	}
    }
  xtss[2]._eflags = 0x0202L;
  xtss[1]._eflags = 0x0202L;

  xtss[1]._eip = (u32)&task1;
  xtss[2]._eip = (u32)&task2;
      

  add_gdt_desc(ktss);
  add_gdt_desc(tss);
  add_gdt_desc(tss2);


}

