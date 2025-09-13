#include <sys.h>
#include <floppy.h>
#define DL 500              //wait delay

char * floppy_type[] = 
{
  "Non installée",
  "360Ko 5.25\"",
  "1.2Mo 5.25\"",
  "720Ko 3.5\"",
  "1.44Mo 3.5\"",
  "2.88Mo 3.5\""
};


static volatile unsigned int called=0;
char buffer[512];


void detect_floppy()
{
  unsigned char c;
  printk(" Lecteurs de disquettes :\n");
  outb(0x70, 0x10);  //On demande au CMOS le type de la disquette
  c = inb(0x71);
  printk("          Lecteure A : %s\n",floppy_type[c >> 4]);  //les 4 bits de poid fort contiennent le type de A:
  printk("          Lecteure B : %s\n",floppy_type[c & 0xF]); //les 4 bits de poid faible contiennent le type de B:
}


void floppy_int()
{
  //printk("\nFD_IRQ\n");
  called=1;
  return;
}


void fdmotor_off()
{
  outb(FD_DOR, 0);

  return;
}

void fdmotor_on()
{
  //printk("FD motor on\n");
  outb(FD_DOR, 0x1C);

  delay(500);
  return;
}




void fd_reset()
{
  outb(FD_DOR, 0);
  outb(FD_DCR, 0);   
  outb(FD_DOR, 0x0C);  
  called=0;
  while(!called);

  outb(0x3f7, 0);    

  fd_cmd(0x03);
  fd_cmd(0xdf);
  fd_cmd(0x02);

  fd_recalib();
  return
  fdmotor_off();
}


int fd_recalib()
{
  //printk("FD recalib\n");
  fdmotor_on();

  fd_cmd(FD_RECALIBRATE);
  fd_cmd(0);
  called=0;
  while(!called);


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

/*
static int fd_wait()
{
  
  int st, i;
  

  for (i=0; i<10000; i++)
    {
      st=inb(FD_STATUS);
      if (st == STATUS_READY) return st;
    }

  return -1;

}
*/
void fdwait_data()
{
  while(((inb(FD_STATUS)&0xd0)!=0xd0));
  return;
}


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

/* sendbyte() routine from intel manual */


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




/*
static int fd_cmd(unsigned char cmd)
{
  int st;

  if ((st=fd_wait()) < 0)
    return -1;
  if ((st & (STATUS_READY|STATUS_DIR|STATUS_DMA))==STATUS_READY)    
    {
      outb(0x3f5, cmd);
      return 0;
    }
  fd_status();

  return -1;
}


static int fd_cmd(unsigned char cmd)
{
  outb(FD_DATA, cmd);
}
*/

int fd_seek(unsigned char trk)
{
  //printk("FD seek\n");

  //fdmotor_on();

  fd_cmd(FD_SEEK);
  fd_cmd(0);
  fd_cmd(trk);

  called=0;
  while(!called);

  delay(16);
  //fdmotor_off();


}

void setDMA()
{
  //printk("Setting DMA\n");

  cli;
  //outb(0xa,0x2);
  outb(0xa,0x6);  //
  outb(0xc,0);    //
  outb(0xb,0x46);

  outb(0x4,0);   //Lo base offset
  outb(0x4,0);   //high base offset

  outb(0x81,0x8);  //0x81h page mem de la chainne DMA 2

  outb(0x5,0xFF);
  outb(0x5,0x01); //0x10

  outb(0xc,0);



  //outb(0xb,0x46);





  outb(0xa,0x2);
  sti;

}


int fd_read()
{
  int i;
  int try=6;
  char readok=0;
  char st0, st1, st2, C, H, N, S;


  fd_recalib();  
  delay(500);
  fd_seek(0);            


  setDMA();

  while (!readok && try)
    {
      setDMA();           
      fd_cmd(0x66);
      fd_cmd(0);
      fd_cmd(0);
      fd_cmd(0);
      fd_cmd(1);
      
      fd_cmd(2);
      fd_cmd(18);
      fd_cmd(0x1B);
      fd_cmd(0xFF);

      called=0;
      while (!called);
      fdwait_data();
      //fd_wait();
      

      printk("ST0 %x \n",st0=inb(FD_DATA));
      printk("ST1 %x \n",st1=inb(FD_DATA));
      printk("ST2 %x \n",st2=inb(FD_DATA));
      printk("Cyl %x \n",C=inb(FD_DATA));
      printk("Hd  %x \n",H=inb(FD_DATA));
      printk("Sct %x \n",S=inb(FD_DATA));
      printk("NBS %x \n",N=inb(FD_DATA));


      if (((st0 & 0xc0)==0) && (S==2))
	{	  
	  readok=1;
	  memcpy(buffer, 0x80000, 512);
	} 
      else
	{
	  try--;	  
	  delay(200);
	  //fd_recalib();
	}


    }
  if (try <=0) 
    {
      for (i=0; i<=20;i++) buffer[i] = 1;
      printk("echec de lecture...\n");
      fd_reset();
      fdmotor_off;
      delay(500);
      fdmotor_on();
      fd_recalib();      
    }
  

  //printk(" %d tries left\n", try);

      



  fdmotor_off();


}

int fd_rd()
{


  setDMA();

//while (!fd_rdread());

  fd_cmd(FD_READ);
  fd_cmd(0);
  fd_cmd(0);
  fd_cmd(0);
  fd_cmd(3);
  fd_cmd(2);
  fd_cmd(18);
  fd_cmd(0x1D);
  fd_cmd(0xFF);

  called=0;
  while (!called);
  //delay(DL);



  printk("ST0 %x \n",inb(FD_DATA));
  printk("ST1 %x \n",inb(FD_DATA));
  printk("ST2 %x \n",inb(FD_DATA));
  printk("Cyl %x \n",inb(FD_DATA));
  printk("Hd  %x \n",inb(FD_DATA));
  printk("Sct %x \n",inb(FD_DATA));
  printk("NBS %x \n",inb(FD_DATA));



  memcpy(buffer, 0x80000, 512);




}


void print_fd_buffer()
{
  int i;
  char * dest=0x80000;
  for (i=0;i<512; i++)
    {
      printk("%x ",buffer[i]&0x000000FF);
    }
}
