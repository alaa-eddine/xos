/*
   Memory Manager

   @Auteur : Alaa-eddine KADDOURI
   @Projet : X Operating System

   @Description :



 */
#include <types.h>
#include <mem.h>
#include <conio.h>
#include <lib.h>
#include <gdt.h>
#include <idt.h>
#include <task.h>
#include "mm.h"


//A ce stade on utilisera une table de bits pour referencer les zones libres et utilisées de la RAM
// 1 bit = une page memoire => 128 octets = 4Mo (car pour l'instant on utilise seulement 4Mo de RAM)
//Par la suite nous utiliserons deux listes chainées : une pour les pages libres et l'autre pour les pages utilisées

static unsigned char BITMAP[MAPSIZE];
static unsigned int fpcount = INITIAL_PAGES_NB;  //free pages counter : contient le nombre de pages 
//libres dans la RAM actuellement mappée

int set_paging()
{
	//On initilise deux pointeurs pour pouvoir acceder a la "PAGE DIRECTORY"
	// et la première "PAGE TABLE"

	unsigned long *page_dir = (unsigned long *) _PD_ADDR_;
	unsigned long *page_tab = (unsigned long *) _PT_ADDR_;


      
	unsigned long current_addr =  0; //utilisée pour le parcours de la mem physique
	// lors de l'initialisation de la PAGE TABLE
	unsigned int i=0;

	//initialisation de la PAGE TABLE
	//les premièrs 4Mo de memoire serons referencés par la table
	//a ce stade nous utilisons l'IDENTITY MAPING (il parrait que c'est la convention ;)

	for (i=0; i<1024; i++)
	{
		page_tab[i] = current_addr | 3;
		current_addr += _PAGE_SIZE;
	}



	page_dir[0] = (unsigned long)page_tab;
	page_dir[0] = page_dir[0] | 3;

	//Pour le reste de la memoire il n'y a pas encore de PAGE TABLES
	// on remplis donc la PAGE DIRECTORY en indiquant que les pages 
	// restantes ne sont pas encore presentes en memoire

	for(i=1; i<1024; i++)
	{
		page_dir[i] = 0 | 3;
	}


	//Activation de la pagination
	_write_cr3((unsigned long)page_dir);
	_write_cr0(_read_cr0() | _PAGING_BIT);

	return 0;
}

int init_mem()
{
	unsigned int i;

	// initialisation de la table de bits
	init_map();
	

	// on marque la première page comme utilisé pour proteger le vecteur des interruptions BIOS
	// qui serons utiles en cas de reteour en mode réél ou en v86
	setused(0);
	
	// on protège aussi les structures système : GDT, IDT, TSS ...
	for (i=(0x80000>>12); i < (0x90000>>12); i++)
		setused(i);
	
	// ensuite on marque la zone memoire entre A0000 (RAM VIDEO) jusqu'a 0x100000 (fin de la BIOS ROM) comme utilisée
	for (i=FIRST_VRAM_PAGE;i < LAST_ROM_PAGE; i++)
		setused(i);

	// puis on marque la zone occupé par notre kernel
	// à partir de 0x1000 ...
	for (i=FIRST_SYS_PAGE;i<LAST_SYS_PAGE;i++)
		setused(i);

	//allocation des pages initiales pour le tas du noyau (heap kernel)
	setused(FIRST_CACHE_PAGE);
	setused(FIRST_CACHE_PAGE+1);
	setused(FIRST_CACHE_PAGE+2);


	//et enfin on ne doit pas oublier de mapper les tables elles memes ;-)
	setused(_PD_ADDR_/_PAGE_SIZE);  //la page directory
	setused(_PT_ADDR_/_PAGE_SIZE);  //et la première page table

	return 0;
}

/* marque la page de position pos comme utilisé*/
int setused(unsigned int pos)
{
	unsigned short bit=0;
	bit = (128 >> (pos % 8));
	BITMAP[pos >> 3] |= bit;
	return 0;
}


int sqr2(int val)
{
int i, res;
res=1;
for(i=0;i<val;i++) res *= 2;

return res;
}

/* marque la page de position pos comme libre*/
int setfree(unsigned int pos)
{
	unsigned short bit=0;
	bit = (255 - (sqr2(7-(pos % 8)) ) );
	BITMAP[pos >> 3] &= bit;
	return 0;
}

/*lit le contenu du bit bitpos dans la BITMAP */
/*Cette fonction sera supprimée par la suite puisque je ne compte pas geerer la memoire par un BITMAP */
int getbit(unsigned int bitpos)
{
	unsigned short bit=0;
	bit = (128 >> (bitpos % 8));
	return ((BITMAP[bitpos >> 3] & bit)!=0);
}


/* initialisation de la structure de gestion des pages memoire */
int init_map(void)
{
	int i;
	for (i=0; i<MAPSIZE; i++)
		BITMAP[i] = 0;

	return 0;
}



/* compte le nombre de pages libres */
/* inutilisé pour l'instant
   unsigned int kcount_free_pages()
   {
   int i;
   unsigned int nb=0;
   for (i=0; i<MAPSIZE*8; i++)
   {
   if(!getbit(i)) nb++;
   }
   return nb;
   }


/*
cette fonction recherche une suite de npages successives libres
dans la table des pages et renvoi l'index de la première page
 */
int get_sfp_idx(unsigned int npages)
{
	int i, lasti=0;
	for (i=1;i < MAPSIZE*8; i++)
	{
		if (!getbit(i))
		{
			if (!lasti) 
			{
				lasti=i;
			}
			else
			{
				if(i-lasti >= npages) return lasti;
			}

		}
		else lasti=0;
	}
	return 0;
}


unsigned long alloc_npages(unsigned int npages)
{
	int i, idx=get_sfp_idx(npages);

	if (idx)
	{
		for (i=idx;i<idx+npages;i++)
		{
			fpcount--;        //on décremente le nombre de pages libres
			setused(i);
		}
		return (idx << 12);   //un decalage à gauche de 12 bits
		//est équivalant à une multiplication par 4096
		//qui est la taille d'une page memoire
	}
	printk("XOSKER32 :: Unable to allocate more memory\n");
	return 0;
}

int print_bit_mem(void)
{
	int i, j=1;
	printk("\n------------------------------------\n");
	for (i=0; i<MAPSIZE; i++, j++)
	{
		printOctetBin(BITMAP[i]);
		if (j==8) {
			printk("\n");
			j=0;
		}
		else printk(" ");
	}
	printk("\n------------------------------------\n");

	return 0;
}


int print_mapped_mem(void)
{
	unsigned int i, lasti;
	unsigned short last;

	lasti=0;
	last=getbit(0);

	printk("--=[ MAPPED RAM ]=-------------------\n");
	for (i=1;i<MAPSIZE*8+1;i++)
	{
		if ((getbit(i) != last) || i==1024)
		{
			if (last==0) {
				printk("0x%x",lasti*4096);
				gotoxy(12, getY());
				printk("-  0x%x",i*4096);
				gotoxy(24, getY());
				printk(" : FREE \n");
			}
			else {
				printk("0x%x",lasti*4096);
				gotoxy(12, getY());
				printk("-  0x%x",i*4096);
				gotoxy(24, getY());
				printk(" : USED \n");
			}

			lasti = i;
			last = getbit(i);
		}
	}
	printk("------------------------------------\n");

	return 0;
}

