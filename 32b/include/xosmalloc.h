#include <types.h>
#ifndef _XOSMALLOC_H_
#define _XOSMALLOC_H_


#define AVL_CONTIG 0xFFFFFFFF
#define AVL_NCONTIG 0
typedef struct _bloc
{
  unsigned long base;    
  unsigned long size;
  struct _bloc * next;
} bloc;


typedef struct _super_bloc
{
  bloc * free_chain;
  bloc * used_chain;
  bloc * avl_chain;
} super_bloc;

typedef struct _page
{
  bloc data[341];
  struct _page * next;
} page;





int init_cache();
void insert_bloc(bloc * new, bloc ** chain, int contig);
int alloc(u32 size);
int free(u32 base);
void sb_info();
void free_chain_info();
void used_chain_info();
void avl_chain_info();
#endif
