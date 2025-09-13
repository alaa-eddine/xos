/*



Un support du systeme de fichier fat12 très très basique, je le réecrirai quand j'aurai le temps.

*/
#include <fat.h>
#include <floppy.h>
#include <string.h>
#include <lib.h>

/** Fonctions independante du materiel **/
void init_floppy(xfloppy * floppy)
{
  floppy->mounted = 0;
  floppy->motor_delay = (unsigned char)500;
  floppy->head = 0;
  floppy->track = 0;
  floppy->sector = 1;
  floppy->entry = 0;
}

int fdmounted(xfloppy floppy)
{
  return floppy.mounted;
}

int mountfloppy(xfloppy * floppy)
{
  fd_reset();

  //fd_seek(1);



  //fdmotor_on();
  //fd_recalib();

  init_floppy(floppy);

   floppy->mounted = 1;
  /*Lecture du premier secteur pour verifier le syst}me de fichier*/
/*  fd_read_sector(0); */
  /**** A completer ****/

  //fdmotor_off();
  return 0;
}

void mstatus(xfloppy floppy)
{
  printk("mstatus = %d\n", fdmounted(floppy));
}
int umountfloppy(xfloppy * floppy)
{
  fdmotor_off();
  floppy->mounted = 0;
  return 0;
}
/*************************************************************/


void get_bs_info(boot_sector * bs)
{
  char buffer[512];
  fd_rds2buffer(0, buffer);
  //memcpy(&bs, 0x80000, sizeof(boot_sector));  
}

fd_entry * get_nextentry(fd_entry * entries, xfloppy * floppy)
{
  if (floppy->entry < MAX_FLOP_BUFF_ENTRIES)
    return &entries[floppy->entry++];
  else 
    return 0;
}


void get_filename(fd_entry entry, char * name)
{
  int size=8;
  char * src=entry.name;
  int i=0;
  while (size--)
    {
      name[i++]=*src++;
    }
  size=3;
  name[i++]=' ';
  while (size--)
    {
      name[i++]=*src++;
    }

  //strcpy(name, entry.name, 8);
  
  name[12]='\0';
}



void get_attr(fd_entry entry, char * attrchar)
{
  strcpy(attrchar, "-r----");
  if (entry.attr&DIR)
    attrchar[0]='d';
  if (!(entry.attr&RO))
    attrchar[2]='w';
  if (entry.attr&ARC)
    attrchar[3]='a';
  if (entry.attr&SYS)
    attrchar[4]='s';
  if (entry.attr&HID)
    attrchar[5]='h';
  attrchar[6]='\0';
}




