#ifndef __MEM_OPS_H__
#define __MEM_OPS_H__

#include "types.h"

void *memset (void *, int, long);
void *memmove (void *, const void *, long);
void *memcpy (void *, const void *, long);
int memcmp (const void *, const void *, long);

#endif
