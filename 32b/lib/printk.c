/*
 * X-OS Printf Implementation
 * 
 * @Author: Alaa-eddine KADDOURI
 * @Project: X Operating System
 * 
 * IMPORTANT LICENSE NOTICE:
 * This module is derived from the KOS project (http://kos.enix.org)
 * and adapted for X-OS formatted output.
 * 
 * Original code from KOS project - license unknown (likely GPL/BSD)
 * Adaptations for X-OS are under MIT license.
 * 
 * This file may contain third-party licensed content.
 */

#include <stdarg.h>
#include <conio.h>
#define MAXBUF (sizeof(long int) * 8)


   static void printnum(u, base)
   register unsigned long  u;      /* number to print */
   register int            base;
{
   char    buf[MAXBUF];    /* build number here */
   register char * p = &buf[MAXBUF-1];
   static char digs[] = "0123456789abcdef";

   do {
      *p-- = digs[u % base];
      u /= base;
   } while (u != 0);

   while (++p != &buf[MAXBUF])
      putcar(*p);
}


void itoa (char *buf, int base, int d)
{
   char *p = buf;
   char *p1, *p2;
   unsigned long ud = d;
   int divisor = 10;

   // If %d is specified and D is minus, put `-' in the head.
   if (base == 'd' && d < 0)
   {
      *p++ = '-';
      buf++;
      ud = -d;
   }
   else if (base == 'x')
      divisor = 16;

   // Divide UD by DIVISOR until UD == 0.
   do
   {
      int remainder = ud % divisor;

      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
   }
   while (ud /= divisor);

   // Terminate BUF.
   *p = 0;

   // Reverse BUF.
   p1 = buf;
   p2 = p - 1;
   while (p1 < p2)
   {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
   }
}


void printk (const char *format, ...)
{
   va_list ap;
   int c;
   char buf[20];

   va_start(ap, format);

   while ((c = *format++) != 0)
   {
      if (c != '%')
	 putcar (c);
      else
      {
	 char *p;

	 c = *format++;
	 switch (c)
	 {
	    case 'd':
	    case 'u':
	    case 'x':
	       itoa (buf, c, va_arg(ap, int));
	       p = buf;
	       while (*p) putcar (*p++);
	       
	       break;

	    case 's':
	       p = va_arg(ap, char*);
	       if (! p)
		  p = "(null)";

	       while (*p) putcar (*p++);
	       break;

	    default:
	       putcar (va_arg(ap, int));
	       break;
	 }
      }
   }
   va_end(ap);
}
