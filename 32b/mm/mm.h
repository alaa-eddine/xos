extern unsigned long * kernelsize;
#define INITIAL_PAGES_NB 4096 // Nombre initial de pages memoire 
#define MIN_REQ_PAGES 3 //Nombre minimum de pages requis pour la maintenance de la gestion memoire
                        

#define _PAGE_SIZE 0x1000  //Taille d'une page memoire (4Ko)
#define MAPSIZE  128  //Taiile du bitmap des pages memoire jusqu'a 4Mo (en identity mapping)



#define _VRAM_START 0xA0000     //adresse de base de la ram video
#define _ROMBIOS_END 0x100000 //limite de la ROM BIOS

#define FIRST_VRAM_PAGE (_VRAM_START >> 12)
#define LAST_ROM_PAGE  (_ROMBIOS_END >> 12)



#define FIRST_SYS_PAGE 0x100                    //première page occupée par le noyeau
#define LAST_SYS_PAGE  (FIRST_SYS_PAGE+ (*kernelsize/_PAGE_SIZE)+1)        //dernière page occupée par le noyeau
                              

#define _PAGING_BIT 0x80000000  //le bit 31 dans le registre CR0

#define CACHE_START_ADDR (0x400000 - 2*MIN_REQ_PAGES*_PAGE_SIZE)
#define FIRST_CACHE_PAGE (CACHE_START_ADDR >> 12) //première page pour les allocations dynamiques

#define _PD_ADDR_ (CACHE_START_ADDR - _PAGE_SIZE) //addresse de la PAGE DIRECTORY
#define _PT_ADDR_ (_PD_ADDR_ - _PAGE_SIZE) //addresse de la PAGE TABLE



typedef struct _st_page{
  unsigned int ord;         //Numero d'ordre de la PAGE TABLE
  char * this;                //pointeur vers cette PAGE TABLE
  struct _st_page * next;           //pointeur vers le mallion suivant
  struct _st_page * prev;           // ------------------------precedant 
} st_page;



extern unsigned long _read_cr0();
extern void _write_cr0(unsigned long);
extern unsigned long _read_cr3();
extern void _write_cr3(unsigned long);



int init_map(void);
int setused(unsigned int pos);

