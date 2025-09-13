#include <types.h>

#ifndef _FAT_H_
#define _FAT_H_

#define MAX_FLOP_BUFF_ENTRIES 16

#define RO       0x01                                              
#define HID      0x02                                              
#define SYS      0x04                                              
#define LBL      0x08                                              
#define DIR      0x10                                              
#define ARC      0x20      

typedef struct _boot_sector
{
  u8   bs_jmp[3];  //ignoré (c'est le jump vers le code du secteur de boot
  u8   bs_OEM[8];  //
  u16  bs_bps;     //octets/secteur
  u8   bs_spc;     //secteur/cluster
  u16  bs_rs;      //nombre de secteurs réservés
  u8   bs_nbfat;   //nombre de FATs
  u16  bs_nbent;   //nombre d'entrées dans la FAT
  u16  bs_ts16;    //nombre total de secteurs (sur 16bits, si le nombre de secteur ne peut pas tenir ici, on utilise bs_ts32)
  u8   bs_media;   //type de media
  u16  bs_fatsz;   //taille de la fat (pour les fat 12 et 16 seulement)
  u16  bs_spt;     //nombre de secteurs par piste
  u16  bs_nbh;     //nombre de tetes (faces)
  u32  bs_hds;     //nombre de secteurs cachés
  u32  bs_ts32;    //utilisé si le nombre total de secteur ne peut pas tenir dans bs_ts16
                   //Note : seul un des deux enregistrements (bs_ts16/bs_ts32) est utilisé, l'autre doit etre mis à 0
  
  ////pour l'instant cette structure suffit pour traiter les fat 12 et 16////
  ////les données pour la fat32 seront ajoutés plus tard /////    
} boot_sector;

typedef struct _fat_directory_entry
{
  char name[11];        //nom du fichier
  u8 attr;              //attributs
  u8 NTres;             //reservé pour windows NT
  u8 crttimet;          //miliseconde du temp de creation du fichier
  u16 crttime;          //temps de creation du fichier
  u16 crtdate;          //date de création du fichier;
  u16 lstdate;          //date de dernier acces
  u16 hi_fclt;          //poid fort du numero du premier cluster (toujours 0 pour la fat 12 et 16)
  u16 wrttime;          //temps de dérnière modification
  u16 wrtdate;          //date de dérnière modification
  u16 fclt;             //poid faible du premier cluster
  u32 fsize;            //taille du fichier
} fd_entry;


typedef struct _floppy_device {
  unsigned char mounted;
  unsigned char motor_delay;
  unsigned char head;
  unsigned char track;
  unsigned char sector;
  unsigned int entry;
//Cette structure n'est que temporaire, elle va etre etendue par la suite   
} xfloppy __attribute__ ((packed));

void list_root_dir();

int mountfloppy(xfloppy * floppy);
int umountfloppy(xfloppy * floppy);
int fdmounted(xfloppy floppy);

void get_bs_info(boot_sector * bs);
fd_entry * get_nextentry(fd_entry * entries, xfloppy * floppy);
void get_filename(fd_entry entry, char * name);
void get_attr(fd_entry entry, char * attrchar);

#endif
