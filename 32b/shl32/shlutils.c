#include <types.h>
#include <sys.h>
#include <kernel.h>
#include <timer.h>
#include <floppy.h>
#include <fat.h>
#include <conio.h>
#include <string.h>
#include <mem_ops.h>
#include <xosdbg.h>
#include <lib.h>
#include <scheduler.h>
#include <mm.h>
#include <xosmalloc.h>
#include "shlutils.h"




int print_mem() {
   printk("Total RAM size: %dMB\n\n",ram_size/1024);
   printk("Mapped RAM size: %dMB\n", 4);
   print_mapped_mem();
   return 0;
}


int test_delay(){
   unsigned int d=0;
   printk("10 second test ...\n");
   printk("d = %ds\n",d++);
   delay(1000);
   printk("d = %ds\n",d++);
   delay(1000);
   printk("d = %ds\n",d++);
   delay(1000);
   printk("d = %ds\n",d++);
   delay(1000);
   printk("d = %ds\n",d++);
   delay(1000);
   printk("d = %ds\n",d++);
   delay(1000);
   printk("d = %ds\n",d++);
   delay(1000);
   printk("d = %ds\n",d++);
   delay(1000);
   printk("d = %ds\n",d++);
   delay(1000);
   printk("d = %ds\n",d++);
   delay(1000);
   printk("d = %ds\n",d++);

   return 0;
}



void egg0() //un p'tit délire.
{
#ifdef EGGS
   int tx=getX();
   int ty=getY();

   int count=10;
   while(count--)
   {
      printk("\n");
      printk("        O_O     X-OS  \n");
      printk("       (^_^)     is   \n");
      printk("      *(_  _)*  COOL! \n");

      delay(700);
      gotoxy(tx, ty);
      printk("\n");
      printk("        O_O     X-OS  \n");
      printk("       (^_^)     is   \n");
      printk("     *(_  _)*   COOL! \n");
      delay(700);
      gotoxy(tx, ty);
   }
   printk("\n\n\n\n\n");
#else
   printk("\nXS32 :: Huh...\n\n");
#endif

}


void list_root_dir1(xfloppy f)
{
   int j;
   fd_entry entry[16];
   fd_entry * current=entry;
   char name[12];
   char attr[7];
   unsigned int fsize;
   unsigned int nbfiles = 0;
   unsigned long size = 0;

   if (!fdmounted(f))
   {
      printk("Floppy drive is not mounted\n use \"mount floppy\" command to mount the floppy disk\n");
      return;
   }

   attrb(0x1A);
   printk("\nAttributes");
   attrb(0x1B);
   printk("  Name");
   attrb(0x1C);
   printk("           Size\n\n");
   int empty_sectors = 0;
   for(j=19; j<=32;j++)
   {
      int read_result = fd_rds2buffer(j, (char *)entry);
      if (read_result < 0) {
         empty_sectors++;
         if (empty_sectors >= 3) {
            // Stop after 3 consecutive failed reads to avoid long delays
            break;
         }
         continue;
      }
      
      empty_sectors = 0; // Reset counter on successful read
      f.entry = 0;
      int entries_found = 0;
      
      while((current=(fd_entry *)get_nextentry(entry, &f)))
      {
	 get_filename(*current, name);
	 get_attr(*current, attr);	  
	 fsize = current->fsize;

	 if ((name[0] != (char)0xE5) && (name[0] != (char)0 ) && (name[0] > (char)0x20) && (name[0] != (char)' ') && (current->attr != 15))
	 {
	    attrb(0x1A);  
	    printk("%s     ", attr);
	    attrb(0x1B);
	    printk("%s", name);
	    attrb(0x1C);
	    if (fsize >= 1024)
	    {
	       printk("  %d ", fsize / 1024);
	       gotoxy(31, getY());
	       printk("Ko\n");
	    }	      
	    else
	    {
	       printk("  %d ", fsize);
	       gotoxy(31, getY());
	       printk("Oct\n");
	    }
	    size += fsize;
	    nbfiles++;
	    entries_found++;
	 }

	 if (name[0] == 0)
	 {
	    // End of directory entries - stop reading more sectors
	    j = 33;
	    break;
	 }
      }
      
      // If no entries found in this sector, it might be empty
      if (entries_found == 0) {
         empty_sectors++;
         if (empty_sectors >= 2) {
            // Stop after 2 consecutive empty sectors
            break;
         }
      }
   }
   attrb(0x1F);
   printk("\n Total size: %d KB in %d files\n\n",size/1024, nbfiles);


}




void task1()
{

   int c=0,i=0;
   int volatile tx, ty;

   imactiv(1);
lock();
   printk("Look at the counter at the top\n it's a task independent from the kernel  :D \n");
unlock();
   while(1) {
      tx=getX();
      ty=getY();
      i++;
      if (!(i % 10000))
      {
	 lock();
	 attrb(0x5E);
	 gotoxy(40, 0);
	 printk("[ TASK1 : Counting :: %d ]",i/10000);
	 gotoxy(tx, ty);
	 attrb(0x1F);
	 unlock();
      }

   }

}


void task2()
{


   int c=0,i=0;
   int volatile tx, ty;

   imactiv(2);
lock();
   printk("Look at the counter at the top\n task 2 independent from the kernel  :D \n");
unlock();
   while(1) {
      tx=getX();
      ty=getY();
      i++;
      if (!(i % 10000))
      {
	 lock();
	 attrb(0x3A);
	 gotoxy(10, 0);
	 printk("[ TASK2 : Counting :: %d ]",i/10000);
	 gotoxy(tx, ty);
	 attrb(0x1F);
	 unlock();
      }

   }

}
