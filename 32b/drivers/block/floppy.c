#include <sys.h>
#include <floppy.h>
#include <fat.h>
#include <lib.h>
#include <mem.h>
#include <timer.h>

#define DL 500              //wait delay

char * floppy_type[] = 
{
  "Not installed",
  "360KB 5.25\"",
  "1.2MB 5.25\"",
  "720KB 3.5\"",
  "1.44MB 3.5\"",
  "2.88MB 3.5\""
};


static volatile unsigned int called=0;
static int dchange=0;
boot_sector bs;




/** Fonctions relatives au materiel **/

void detect_floppy()
{
  unsigned char c;
  star();
  printk("Floppy drives:\n");
  outb(0x70, 0x10);  //On demande au CMOS le type de la disquette
  c = inb(0x71);
  printk("      Drive A : %s\n",floppy_type[c >> 4]);  //high 4 bits contain type of A:
  printk("      Drive B : %s\n",floppy_type[c & 0xF]); //low 4 bits contain type of B:
}



void floppy_int()
{
  called=1;
}


void fdmotor_off()
{
#ifdef FDCDEBUG
  printk("FD motor off\n");
#endif

  outb(FD_DOR, 0);
  return;
}


void fdmotor_on()
{
#ifdef FDCDEBUG
  printk("FD motor on\n");
#endif

  outb(FD_DOR, 0x1C);
  delay(500);
  return;
}




void fd_reset()
{
char st0, i, trk;
#ifdef FDCDEBUG
  printk("FD reset\n");
#endif

  called=0;



  outb(FD_DOR, 0);
  outb(FD_DCR, 0);
  outb(FD_DOR, 0x0C);

  
  int reset_timeout = 25000;
  while(!called && reset_timeout > 0) {
    reset_timeout--;
  }
  //outb(FD_DCR, 0);

for (i=0; i<4; i++)
{
#ifdef FDCDEBUG
printk("reset sensei ::");
#endif
fd_cmd(0x08);
st0 = getbyte();
trk = getbyte();
#ifdef FDCDEBUG
printk("st0=0x%x   trk=0x%x\n", st0, trk);
#endif
}


  fd_cmd(FD_SPECIFY);

  fd_cmd(0xdf);
  fd_cmd(0x02);



/*
printk("seeking ...\n");
fd_seek(1);
printk("recalibrating ...\n");
fd_recalib();
printk("done\n");
*/
  dchange = 0;
}


int fd_recalib()
{
char st0, pcn;
#ifdef FDCDEBUG
  printk("FD recalib\n");
#endif

//fdmotor_on();
  called=0;

  fd_cmd(FD_RECALIBRATE);
  fd_cmd(0);

  int recalib_timeout = 25000;
  while(!called && recalib_timeout > 0) {
    recalib_timeout--;
  }
#ifdef FDCDEBUG
  printk("recalib sensei ...\n");
#endif
  fd_cmd(0x08);
  st0 = getbyte();
  pcn = getbyte();

#ifdef FDCDEBUG
printk("st0= 0x%x trk=0x%x done\n", st0, pcn);
#endif


//fdmotor_off();
//  st0=inb(FD_DATA);
  return 0;
}




void fd_status()
{
  char * status[] = {
    "floppy drive 0 in seek mode/busy",
    "floppy drive 1 in seek mode/busy",
    "floppy drive 2 in seek mode/busy",
    "floppy drive 3 in seek mode/busy",
    "FDC read or write command in progress",
    "FDC is in non-DMA mode",
    "I/O direction;  1 = FDC to CPU; 0 = CPU to FDC",
    "data reg ready for I/O to/from CPU (request for master)"};
  int st=inb(FD_STATUS);
  int i;
  for (i=0; i<8; i++)
    {

      if (st & (1 << i)) {
	printk("test %d == %x\n", i, 1<<i);
	printk("%s\n",status[i]);
      }
    }
}


void fdwait_data()
{
#ifdef FDCDEBUG
  printk("FD waiting for data...\n");
#endif
  int timeout = 10000;  // Reduced timeout for faster response
  while(((inb(FD_STATUS)&0xd0)!=0xd0) && timeout > 0) {
    timeout--;
  }
  if (timeout <= 0) {
#ifdef FDCDEBUG
    printk("FD timeout waiting for data ready status\n");
#endif
  }
  return;
}

/* getbyte()  routine from intel manual */
int getbyte()
{
   volatile int msr;
   int tmo;

   for (tmo = 0;tmo < 128;tmo++) {
      msr = inb(FD_STATUS);
      if ((msr & 0xd0) == 0xd0) {
	 return inb(FD_DATA);
      }
      inb(0x80);   /* delay */
   }

   return -1;   /* read timeout */
}


/* sendbyte() (renamed to fd_cmd) routine from intel manual */
void fd_cmd(int byte)
{
   volatile int msr;
   int tmo;

   for (tmo = 0;tmo < 128;tmo++) {
      msr = inb(FD_STATUS);
      if ((msr & 0xc0) == 0x80) {
	 outb(FD_DATA, byte);
	 return;
      }
      inb(0x80);
   }
}





int fd_seek(unsigned char trk)
{
char st0, pcn;
#ifdef FDCDEBUG
  printk("FD seek...\n");
#endif
  called=0;

  fd_cmd(FD_SEEK);
  fd_cmd(0);
  fd_cmd(trk);



  int seek_timeout = 25000;
  while(!called && seek_timeout > 0) {
    seek_timeout--;
  }
#ifdef FDCDEBUG
printk("Seek sensei ...");
#endif
fd_cmd(0x08);
st0 = getbyte();
pcn = getbyte();
#ifdef FDCDEBUG
printk("st0= 0x%x trk=0x%x done\n", st0, pcn);
#endif



  delay(15);
//  st0=inb(FD_DATA);
  return 0;
}

void setDMA()
{

#ifdef FDCDEBUG
  printk("FD setting DMA...\n");
#endif
  cli;


  //outb(0xa,0x2);
  outb(0xa,0x6);  //
  outb(0xc,0);    //
  outb(0xb,0x46);

  outb(0x4,0);   //Lo base offset
  outb(0x4,0);   //high base offset

  outb(0x81,0x9);  //page mem de la chainne DMA 2

  outb(0x5,0xFF);
  outb(0x5,0x01); //0x10

  outb(0xc,0);

  outb(0xa,0x2);

  sti;

}


void lba2hts(int lba, char* head, char* track, char* sector)
{
	*head = (lba % (18 * 2)) / (18);
	*track = lba / (18 * 2);
	*sector = lba % 18 + 1;
}



int fd_read_sector(unsigned int lba)
{

  int try=5;
  char readok=0;
  char head, track, sect;
  char st0, pcn;




  lba2hts(lba, &head, &track, &sect);



  if (inb(FD_DIR)&0x80)
    {
      dchange = 1;
      fd_seek(1);
      fd_recalib();
      return 0;
    }

  fd_seek(1);
  fd_recalib();

  fdmotor_on();

  while (try)
    {

      setDMA();
      fd_cmd(FD_READ);
      fd_cmd(head<<2);
      fd_cmd(track);
      fd_cmd(head);
      fd_cmd(sect);
      
      fd_cmd(2);
      fd_cmd(18);
      fd_cmd(0x1B);
      fd_cmd(0xFF);

      called=0;
      int read_timeout = 50000;  // Reduced timeout for faster response
      while (!called && read_timeout > 0) {
        read_timeout--;
        if ((read_timeout % 10000) == 0) {
          // Small delay every 10000 iterations to prevent busy waiting
          for (volatile int delay_count = 0; delay_count < 100; delay_count++);
        }
      }
      
      if (read_timeout <= 0) {
#ifdef FDCDEBUG
        printk("FD read_sector timeout waiting for interrupt\n");
#endif
        try = 1;  // Force exit on timeout
      }
      
      fdwait_data();

      st0=inb(FD_DATA);

      if (((st0 & 0xc0)==0))
	  break;
      else
	  try--;

      fd_recalib();

    }
  if (try <=0) 
    {
     // printk("echec de lecture...\n");
     return -1;
    }
  

  //printk(" %d tries left\n", try);

  fdmotor_off();


  return 0;
}

int fd_rds2buffer(unsigned int lba, char * buffer)
{

  int try=5;
  char readok=0;
  char head, track, sect;
  char st0, st1, st2, st3, c, h, r, n;

  lba2hts(lba, &head, &track, &sect);

  fdmotor_on();

  fd_recalib();

  /*
  if (inb(FD_DIR) & 0x80) {
    dchange = 1;
    fd_seek(1);
    fd_recalib();
    fdmotor_off();
    return 0;
  }
*/
  


  while (try)
    {
      setDMA();
      fd_cmd(FD_READ);
      fd_cmd(head<<2);
      fd_cmd(track);
      fd_cmd(head);
      fd_cmd(sect);

      fd_cmd(2);
      fd_cmd(18);
      fd_cmd(0x1B);
      fd_cmd(0xFF);

      called=0;
      int interrupt_timeout = 50000;  // Reduced timeout for faster response
      while (!called && interrupt_timeout > 0) {
        interrupt_timeout--;
        if ((interrupt_timeout % 10000) == 0) {
          // Small delay every 10000 iterations to prevent busy waiting
          for (volatile int delay_count = 0; delay_count < 100; delay_count++);
        }
      }
      
      if (interrupt_timeout <= 0) {
#ifdef FDCDEBUG
        printk("FD timeout waiting for interrupt\n");
#endif
        // Try to continue anyway, but reduce retry count
        try = 1;
      }

	fdwait_data();


      st0=inb(FD_DATA);
      st1=inb(FD_DATA);
      st2=inb(FD_DATA);
      c=inb(FD_DATA);
      h=inb(FD_DATA);
      r=inb(FD_DATA);
      n=inb(FD_DATA);

      if (((st0 & 0xc0)==0))
	  break;
      else
	  try--;

      fd_recalib();
    }


  if (try<=0)
    {
//      printk("echec de lecture...\n");
	return -1;
    }


  //printk(" %d tries left\n", try);

  fdmotor_off();
  memcpy(buffer, (void *)0x90000, 512);

  st0=inb(FD_DATA);

  return 0;
}

int fd_read_test(xfloppy myfloppy)
{
  return 0;

}



void print_fd_buffer(char * buffer)
{
  int i;
//  char * dest=0x80000;
  for (i=0;i<512; i++)
    {
      printk("%x ",buffer[i]&0x000000FF);
    }
}
