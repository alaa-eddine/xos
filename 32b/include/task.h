#ifndef _TASK_H_
#define _TASK_H_

#define TSSBASE 0x81000


struct _tss {

  u16 ptl;   //previous task link  (pointeur vers la tach appellante)
  u16 res;

  u32 _esp0;
  u16 _ss0;
  u16 _res0;

  u32 _esp1;
  u16 _ss1;
  u16 _res1;

  u32 _esp2;
  u16 _ss2;
  u16 _res2;

  u32 _cr3;  //pdbr
  u32 _eip;
  u32 _eflags;
  u32 _eax;
  u32 _ecx;
  u32 _edx;
  u32 _ebx;
  u32 _esp;
  u32 _ebp;
  u32 _esi;
  u32 _edi;
  
  u16 _es;
  u16 _res3;
  u16 _cs;
  u16 _res4;
  u16 _ss;
  u16 _res5;
  u16 _ds;
  u16 _res6;
  u16 _fs;
  u16 _res7;
  u16 _gs;
  u16 _res8;

  u16 _ldtss;  //LDT segment selector
  u16 _t;      //seul le bit 0 est utilisé
  u16 _iomapaddr;
};



void init_task();

#endif
