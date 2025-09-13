
#ifndef _MM_H_
#define _MM_H_
int set_paging();
int init_mem();
unsigned long alloc_npages(unsigned int npages);
int print_bit_mem(void);
int print_mapped_mem(void);

#endif
