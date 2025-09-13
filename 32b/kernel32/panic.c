#include <sys.h>
#include <panic.h>
#include <kbd.h>
#include <conio.h>
#include <lib.h>

unsigned char panic=0;

void _panic()
{
unsigned long i;

  attrb(0x1F+128);
  printk("\n\nXOSKER32 :: !!! KERNEL PANIC !!!  :(\n");
  //printk(str);
  attrb(0x1F);

do
{
switch_num_lock();
for (i=0; i<0xFFFFF0;i++);
switch_caps_lock();
for (i=0; i<0xFFFFF0;i++);
switch_scroll_lock();
for (i=0; i<0xFFFFF0;i++);
}
while(1);
}

void kernpanic()
{
	panic=1;
}
