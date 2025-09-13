/**/
/*
Pour l'instant ce module ne permet pas d'activer le mode vesa, 
mais seulement de récuperrer les informations relatives au vesa
*/
/**/
/**/
#include <vesa.h>
#include <lib.h>
#include <conio.h>
#include <string.h>
char * get_sig(vbe_info * vbe)
{
  return (char *) (vbe->sig);
}

int isvesa(vbe_info * vbe)
{
  return (!strcmp(get_sig(vbe), "VESA")) || (!strcmp(get_sig(vbe), "VBE2"));
}

char * get_oemname(vbe_info * vbe)
{
  return  (char *) ( ((vbe->seg_name)*0x10)+(vbe->ofs_name) );
}

u16 * get_vram(vbe_info * vbe)
{
  return (u16 *)((vbe->vram)*64);
}

void print_vinfo(vbe_info * vbe)
{
  printk("%s ", get_sig(vbe));  /*La signature*/
  printk("%d.%d \n", ((vbe->ver&0xFF00)>>8), ((vbe->ver)&0x00FF));
}
