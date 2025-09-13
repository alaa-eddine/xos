/*
   @Auteur : Alaa-eddine KADDOURI
   @Projet : X Operating System

   @Description : Le gestionaire des allocations dynamiques pour X-OS

 */


/*
   PRINCIPE DE FONCTIONNEMENT :
   j'ai écrit ce gestionnaire d'allocation mémoire de façon
   à ce qu'il soit le plus simple possible.
   xosmalloc n'est certainement pas l'un des allocateur les plus performant
   à cause des parcours de listes chaînées mais j'espère qu'il soit le plus 
   simple à comprendre.

   Son principe est très simple , il repose sur deux listes chaînées
   une pour la liste des blocs libres, et l'autre pour les blocs utilisés
   chaque maillon d'une des deux liste stock l'adresse de base du bloc mémoire,
   sa taille et un pointeur vers le maillon suivant, ainsi on évite au maximum 
   la fragmentation interne.

   Comment ça marche ?
   Tout d'abord on doit initialiser toutes ces structures.
   Pour cela la fonction init_cache fait les opérations suivantes :
1 : alloue une page (elle va contenir les listes des éléments 
libres et utilisés ainsi qu'une autre liste dont on parlera plus tard)
2 : crée un premier maillon dans la liste des blocs libres.
3 : crée un maillon (avl_chain) qui stock l'espace libre dans la page qu'on vient d'allouer
quel est le rôle de cet élément ?
ce que vous devez retenir pour l'instant est que cet élément va nous permettre de savoir
si la page prévue pour stocker nos listes chaînée contient encore de l'espace libre pour d'autres maillons
et éventuellement d'allouer une nouvelle page dans le cas contraire
c'est comme une liste de blocs libre pour le cache et dont chaque bloc fait la taille d'un élément d'une liste (utilisé ou libre)

4 : la liste des blocs utilisée est mise à NULL



La page allouée                                    la première page de RAM
pour le cache                                      Référencée par notre liste
 __________                                         ___________
|          |                                       |           |
|          |                                       |           |
|          |                                       |           |
|          |                                       |           |
|          |
...       

|          |                                       |           |
|__________|                                       |           |
|x |  4096 |----------------------------+          |           | 
|__|_______|                            |          |           |
|  |   |   | Le super bloc              |          |           |
-----------                             +---------> -----------




cette doc n'est pas encore complete.
 */

#include <xosmalloc.h>
#include <types.h>
#include <mm.h>
#include "mm.h"
#include <lib.h>


struct _super_bloc * sb;
/*
   pour avl :
   si base = 0 et size = 0  alors next pointe vers le prochain;
   si base = 0xFFFFFFFF alors next pointe sur un tableau contigu et size contient la taille du tableau
 */

u32 max_small_alloc=_PAGE_SIZE;

int init_cache()
{
   page * p;
   unsigned long temp;
   //  printk("SIZEOF(bloc) = 0x%x\n", sizeof(bloc));

   sb = (struct _super_bloc *)alloc_npages(1);  //a remplacer par une constante
   p = (page *)sb;
   p->next=NULL;              //prochaine page pour les petite allocation 

   //  printk("[ super bloc @ 0x%x ]\n", sb);
   sb->used_chain=NULL;       //liste des pointeurs de blocs utilisés

   temp=(u32)sb+sizeof(super_bloc);
   //  printk(" * temp = 0x%x\n", temp);

   sb->free_chain=(bloc *)((u32)sb+sizeof(super_bloc));          //liste des blocs libres (juste derrière le super bloc)
   sb->free_chain->base=alloc_npages(1);  //on alloue une première page pour l'utilisé lors des allocations de moins de 4096 oct

   sb->free_chain->size=_PAGE_SIZE;
   sb->free_chain->next=NULL;


   sb->avl_chain=(bloc *)((u32)sb->free_chain+sizeof(bloc));
   sb->avl_chain->base=AVL_CONTIG;
   sb->avl_chain->size=_PAGE_SIZE-(2*sizeof(bloc)+sizeof(super_bloc));
   sb->avl_chain->next=NULL;
   //sb->avl_chain->next=(u32)sb->avl_chain+(u32)sizeof(bloc);
   return 1;
}


/**
insert_bloc : insert le bloc "new" dans la liste de blocs "chain" 
par ordre croissant sur "l'adresse de base"+"la taille".



 */
void insert_bloc(bloc * new, bloc ** chain, int contig)
{
   bloc * current = *chain;
   bloc * prev = NULL;
   while (current && current->base+current->size <= new->base+current->size )
   {
      prev = current;
      current = current->next;
   }

   if (!prev) // insertion en tete de liste
   {
      if (contig && current && (new->base+new->size==current->base))
      {
	 current->base = new->base;
	 current->size += new->size;

	 new->base=0;
	 new->size=0;
	 insert_bloc(new, &(sb->avl_chain), 0);
	 return;
      }		

      new->next=current;
      *chain=new;		
      return;
   }

   //insertion au milieu ou en fin de liste ...
   if (!contig)
   {
      new->next=current;
      prev->next=new;	
      return;
   }


   if(prev->base+prev->size==new->base)
   {
      prev->size += new->size;

      if (current && (prev->base+prev->size==current->base))
      {
	 prev->size += current->size;	
	 prev->next = current->next;

	 current->base=0;
	 current->size=0;
	 insert_bloc(current, &(sb->avl_chain), 0);

	 new->base=0;
	 new->size=0;
	 insert_bloc(new, &(sb->avl_chain), 0);

      }			
   }
   else
   {

      if (current && (new->base+new->size==current->base))
      {
	 current->base = new->base;
	 current->size += new->size;

	 prev->next=current;

	 new->base=0;
	 new->size=0;
	 insert_bloc(new, &(sb->avl_chain), 0);


      }
      else {
	 new->next=current;
	 prev->next=new;	
	 return;

      }



   }

}


/*

 */
int adjust_avl(bloc * avl)
{

   page * p;

   if (avl->base==0xFFFFFFFF)
   {	 
      avl[1].size = avl->size-sizeof(bloc);
      avl[1].base = avl->base;
      avl[1].next = NULL;

      if (avl[1].size == sizeof(bloc))				  
	 avl[1].size = avl[1].base = 0;			      
      sb->avl_chain = &avl[1];	    
   }
   else
   {
      sb->avl_chain = avl->next;
      if (!sb->avl_chain)             //plus de blocs dans le cache ?
      {
	 p = (page *)sb;
	 if (!(p->next = (page *)alloc_npages(1))) return NULL;   //on alloue une nouvelle page de 4096 octets
	 sb->avl_chain = (bloc *)p;
	 sb->avl_chain->base=0xFFFFFFFF;
	 sb->avl_chain->size=4092;   //4092 pour les blocs est 4 octets pour le pointeur vers la page suivante
	 p->next = NULL;
      }
   }

   return 1;

}






int alloc_small(u32 size, bloc ** freeptr)
{
   bloc * cur_free=*freeptr;
   bloc * prev=NULL;
   bloc * avl=sb->avl_chain;

   bloc * new_used;
   bloc * new_free;
   page * p;



   while (cur_free)
   {
      printk("sizes   0x%x # x%x\n", cur_free->size, size);
      if (cur_free->size >= size)
      {
	 new_used=avl;

	 if (!adjust_avl(avl)) return NULL;

	 new_used->size=size;
	 new_used->base=cur_free->base;
	 insert_bloc(new_used, &(sb->used_chain), 0);



	 cur_free->size-=size;              //ajustement de la taille libre dans le maillon
	 if (cur_free->size==0)             //si la taille est devenu null on enlève le maillon de la liste libre
	 {	                              	     
	    if (!prev)
	       *freeptr=cur_free->next;
	    else
	       prev->next=cur_free->next;

	    cur_free->base=0;                            //on met l'espace occupé par le pointeur de bloc libre dans
	    insert_bloc(cur_free, &(sb->avl_chain), 0);     //la liste des blocs réservés pour le cache "avl_chain"
	 }
	 else 
	    cur_free->base+=size;


	 return new_used->base;
      }	
      prev=cur_free;
      cur_free=cur_free->next;
   }


   //aucun block ne peut accueillir la nouvelle taille requise on alloue donc un nouvel espace
   new_used=avl;
   if (!adjust_avl(avl)) return NULL;
   new_used->size=size;

   if (!(new_used->base=alloc_npages(0)) ) return NULL;
   insert_bloc(new_used, &(sb->used_chain), 0);


   
      avl=sb->avl_chain;
      new_free=avl;
      if(!adjust_avl(avl)) return NULL;

      new_free->size=_PAGE_SIZE - size;
      new_free->base = (new_used->base)+size;
      insert_bloc(new_free, &(sb->free_chain), 1);
    


   return new_used->base;


   /*return NULL;*/

}

int alloc_large(u32 size, bloc ** freeptr)
{
   bloc * cur_free=*freeptr;
   bloc * avl=sb->avl_chain;

   bloc * new_used;
   bloc * new_free;


   u32 required_pages = size / _PAGE_SIZE;
   u32 next_size = size - (required_pages*_PAGE_SIZE);




   if (next_size) required_pages++;


   new_used=avl;
   if (!adjust_avl(avl)) return NULL;
   new_used->size=size;
   if (!(new_used->base=alloc_npages(required_pages)) ) return NULL;
   insert_bloc(new_used, &(sb->used_chain), 0);


   if (next_size)
   {

      avl=sb->avl_chain;
      new_free=avl;
      if(!adjust_avl(avl)) return NULL;

      new_free->size=_PAGE_SIZE-next_size;
      new_free->base = (new_used->base)+size;
      insert_bloc(new_free, &(sb->free_chain), 1);
   }

   return new_used->base;



}

/**
  Cette fonction alloue size octets dans le cache
  elle fait appel à une routine d'allocation en 
  fonction de la taille que l'on veut allouer  
 */
int alloc(u32 size)
{
   if (size <= 0) return NULL;
   if (size < max_small_alloc)
      return alloc_small(size, &(sb->free_chain));

   if (size >= _PAGE_SIZE)
      return alloc_large(size, &(sb->free_chain));

   return NULL;  
}




/**
  Cette fonction libère le bloc dont la base est 
  fournie en paramètre.   
  Elle retourne la taille libérée en cas de succès
  et NULL sinon

 */
int free(u32 base)
{
   bloc * used=sb->used_chain;
   bloc * prev=NULL;
   bloc * new_free;
   bloc * cur_free;
   u32 oldbase, oldsize, i, freed=0;
   while(used) //parcours de la liste de maillons utilisés
   {
      if (used->base == base) //trouvé :)
      {
	 oldbase=base;
	 oldsize=used->size;

	 if (!prev) sb->used_chain=used->next;   
	 else prev->next=used->next;                


	 new_free=used;

	if (!(new_free->base % _PAGE_SIZE) && (new_free->size >= _PAGE_SIZE))
	{
	 printk("base = 0x%x  size = 0x%x\n", new_free->base, new_free->size);
         for (i=new_free->base / _PAGE_SIZE; i < (new_free->base+new_free->size) / _PAGE_SIZE; i++) 
             {
               printk("freeing %d \n",i);
               setfree(i);
	       freed++;
             }
	 if (new_free->size % _PAGE_SIZE)
	 {
	    printk("insert free\n");
	    new_free->base += freed*_PAGE_SIZE;
	    new_free->size -= freed*_PAGE_SIZE;
	    insert_bloc(new_free, &(sb->free_chain), 1);
	 }
	 else
	 {
	    printk("insert avl\n");
	    new_free->base = 0;
	    new_free->size = 0;
	    insert_bloc(new_free, &(sb->avl_chain), 0);
	 }
	}
	else	 insert_bloc(new_free, &(sb->free_chain), 1);
	 printk("*** base=0x%x  size=0x%x\n", new_free->base, new_free->size);
	 /*
	    cur_free=sb->free_chain;
	    while (cur_free)
	    {
	    if (cur_free->next)
	    {
	    printk("-> 0x%x..0x%x",cur_free->base, cur_free->size);
	    if (!(cur_free->base % _PAGE_SIZE) && (cur_free->size >= _PAGE_SIZE)) 
	    {
	    if (cur_free->size % _PAGE_SIZE)		
	    {
	    for (i=new_free->base / _PAGE_SIZE;i<= (new_free->base + new_free->size) / _PAGE_SIZE; i++)
	    setfree(i);

	    new_free->base=0;
	    new_free->size=0;
	    insert_bloc(new_free, &(sb->avl_chain), 0);
	    }
	    else
	    {
	    for(i=new_free->base / _PAGE_SIZE;i<= ((new_free->base + new_free->size) / _PAGE_SIZE)-1; i++)
	    setfree(i);

	    new_free->base=new_free->base+ (((new_free->base + new_free->size) / _PAGE_SIZE)-1)*_PAGE_SIZE ;
	    new_free->size=new_free->size-  (((new_free->base + new_free->size) / _PAGE_SIZE)-1)*_PAGE_SIZE ;
	    }

	    }
	    }
	    else  //unseul element dans la liste
	    {
	    if (!(new_free->base % _PAGE_SIZE) && (new_free->size > _PAGE_SIZE))
	    {
	    for (i=(new_free->base / _PAGE_SIZE)+1; i<= ((new_free->base +new_free->size) / _PAGE_SIZE)-1; i++)
	    setfree(i);
	    new_free->size=_PAGE_SIZE;	
	    }
	    printk("un element dans la liste d'une taille de 0x%x\n",cur_free->size);
	    }
	    cur_free = cur_free->next;
	    }

	  */
	 return oldsize;
      }
      prev=used;
      used=used->next;
   }
   return NULL;
}






/* 
   Les routines qui suivent sont utilisée pour le débogage
   elle affichent les informations relatives aux cache
 *liste des bloc utilisés
 *liste des blocs libres
 *liste des pages utilisées par le cache
 *liste des blocs utilisables par le cache
 */
void sb_info()
{
   printk("===[ MALLOC SUPER BLOC ]====================\n");
   printk("[ super bloc @ 0x%x ]\n", sb);
   printk("free chain = 0x%x\n", sb->free_chain);
   printk("used chain = 0x%x\n", sb->used_chain);
   printk("available chain = 0x%x\n", sb->avl_chain);
   printk("next available chain = 0x%x\n", sb->avl_chain->next);
   printk("============================================\n");
}

void free_chain_info()
{
   bloc * cur_free=sb->free_chain;
   printk("===[ MALLOC FREE CHAIN ]====================\n");
   while (cur_free)
   {
      printk("  [ free bloc @ 0x%x ]\n", cur_free);
      printk("base 0x%x\n", cur_free->base);
      printk("size 0x%x\n", cur_free->size);
      printk("next 0x%x\n", cur_free->next);
      cur_free = cur_free->next;
   }
   printk("============================================\n");
}

void used_chain_info()
{
   bloc * cur_used=sb->used_chain;
   printk("===[ MALLOC USED CHAIN ]====================\n");
   while (cur_used)
   {
      printk("  [ used bloc @ 0x%x ]\n", cur_used);
      printk("base 0x%x\n", cur_used->base);
      printk("size 0x%x\n", cur_used->size);
      printk("next 0x%x\n", cur_used->next);
      cur_used = cur_used->next;
   }
   printk("============================================\n");
}
void avl_chain_info()
{
   bloc * cur_avl=sb->avl_chain;
   printk("===[ MALLOC AVL CHAIN ]====================\n");
   while (cur_avl)
   {
      printk("  [ avl bloc @ 0x%x ]\n", cur_avl);
      printk("base 0x%x\n", cur_avl->base);
      printk("size 0x%x\n", cur_avl->size);
      printk("next 0x%x\n", cur_avl->next);
      cur_avl = cur_avl->next;
   }
   printk("============================================\n");
}
///////FONCTIONS A RAJOUTER
//cache_chain_info()

