/*
@Auteur : Alaa-eddine KADDOURI
@Projet : X Operating System

@Description :



*/

#include <mem_ops.h>


/**
   region of memory with the given value
   * @s: Pointer to the start of the area.
   * @c: The byte to fill the area with
   * @count: The size of the area.
   *
   * Do not use memset() to access IO space, use memset_io() instead.
   */
void * memset(void * s,int c,long count)
{
  char *xs = (char *) s;
		
  while (count--)
    *xs++ = c;
					
  return s;

}
/* memcpy - Copy one area of memory to another
* @dest: Where to copy to
* @src: Where to copy from
* @count: The size of the area.
*
* You should not use this function to access IO space, use memcpy_toio()
     * or memcpy_fromio() instead.
  */

void * memcpy(void * dest,const void *src,long count)
{
  char *tmp = (char *) dest, *s = (char *) src;

  while (count--)
    *tmp++ = *s++;

  return dest;
}




void * memmove(void * dest,const void *src,long count)
{
  char *tmp, *s;

  if (dest <= src) {
    tmp = (char *) dest;
    s = (char *) src;
    while (count--)
      *tmp++ = *s++;
  }
  else {
    tmp = (char *) dest + count;
    s = (char *) src + count;
    while (count--)
      *--tmp = *--s;
  }

  return dest;
}





/**
 * memcmp - Compare two areas of memory
 * @cs: One area of memory
 * @ct: Another area of memory
 * @count: The size of the area.
 */
int memcmp(const void * cs,const void * ct,long count)
{
  const unsigned char *su1, *su2;
  int res = 0;

  for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
    if ((res = *su1 - *su2) != 0)
      break;
  return res;
}



						
/*
 
void *memset(void* d, int c, long n)
{
  long i=n;

  while(n--)
    ((char*)d)[i-n-1]=c;
}

void *memmove(void* src, void* dest, long n)
{
	long i=n;
	int x=0;

	if (dest<src)
	  while(n--)
	    ((char*)dest)[i-n-1]=((char*)src)[i-n-1];
	else
	  while(n--)
	    ((char*)dest)[x]=((char*)src)[x++];
	return dest;
}

void *memcpy(void* dest, void* src, long n)
{

  if (dest+n < src || dest > src+n)
    return memmove(dest, src, n);
  return -1;
}

int memcmp(void *s1, void *s2, long n)
{
	long i=n;

	while(n--)
	  if(((char*)s1)[i-n]!=((char*)s2)[i-n])
	    return ((char*)s1)[i-n]-((char*)s2)[i-n];

	return 0;
}
*/
