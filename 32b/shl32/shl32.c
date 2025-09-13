/*
   @Auteur : Alaa-eddine KADDOURI
   @Projet : X Operating System

   @Description :



 */


#include <types.h>
#include <sys.h>
#include <kbd.h>
#include <kernel.h>
#include <timer.h>
#include <floppy.h>
#include <fat.h>
#include <conio.h>
#include <string.h>
#include <mem_ops.h>
#include <xosdbg.h>
#include <lib.h>
#include <xosmalloc.h>
#include <mem.h>
#include "shlutils.h"

#define CTRL_SHORTCUT(code, commande)  if (sc==(code) && ctrl) {commande break;}		
#define ALT_SHORTCUT(code, commande)  if (sc==(code) && alt) {commande break;}

extern void jump_to_task();
extern unsigned char panic;
char ver[] = "1.05";



extern xfloppy floppy;



extern void _reboot();
int print_txt_logo();
int print_dbhelp_en();
int print_dbhelp_fr();
int print_help_en();
int print_help_fr();
void print_schelp_en();
void print_schelp_fr();
void exec(char* command);

char * fakepointer=(char * )0x2000000;  //pour generer une page fault !!!  :-p



extern short caps;  //caps lock
extern short num;  //caps lock

short upper=0;

short ctrl=0;
short alt=0;
short del=0;
short e0=0;


void ctrlaltdel()
{
   if (ctrl && alt && del)
   {
      printk("\nCtrl+Alt+Del : rebooting computer in 3 seconds...\n");
      delay(3000);      
      _reboot();      
      printk("Reboot failed :(     ... system halted"); //Si on n'arrive pas à rebooter
      while(1);							     //On entre dans une boucle infinie
   }

}


int xs32(void) {

   unsigned char sc=0;
   char shellstr[] = "[Floppy]$ : ";

   char command[256];
   unsigned char buffi=0;

   unsigned short ptr=0;
   unsigned short maxptr=0;
   char exitcmd;



   attrb(0x1F);


   print_txt_logo();
   printk("\nXS32 version %s a 32-bit shell for X-OS\n", ver);
   printk("\n     TAPEZ 'aide' POUR VOIR LES COMMANDES DISPONNIBLES...\n");
   printk("\n     TYPE 'help' TO SEE AVAILABLE COMMANDS...\n\n");



   printk(shellstr);
   do
   {
      e0=0;
      exitcmd=0;      
      sc = getSC();
      if (sc==0xE0) 
      {
	 e0=1;
	 sc=getSC();      
//	 printk("0x%x ",sc);
      }
      else sc--;

      switch(sc){
	 case 0x4B : {			  //LEFT

                      if (!e0)
                        {
                           if (ptr==maxptr) maxptr++;
                               command[ptr++]=key(sc*4);
                            putcar(key(sc*4));
                       }
		else																	     
			if (ptr>0){
			   ptr--;			   
			   gotoxy(getX()-1, getY());
			}
			
		     }break;
	 case 0x4D :			  
		     {
			if (ptr < maxptr){
			   ptr++;
			   gotoxy(getX()+1, getY());
			}
		     }break;		    



	 case 13 :
		     {                     //BACKSPACE
			if (ptr>0)
			{
			   ptr--;	
			   maxptr--;
			   gotoxy(getX()-1, getY());
			   putcar(' ');
			   gotoxy(getX()-1, getY());
			}
		     }
		     break;
	 case 27 :
		     {                    //ENTER

			command[maxptr] = '\0';
			maxptr=ptr=0;			
			putcar('\n');
			exec(command);
			printk(shellstr);
		     }break;
	 case 0x44: switch_num_lock();break; //num lock
	 case 0x45: switch_scroll_lock();break; //scroll lock
	 case 0x39: switch_caps_lock();break; //caps lock


		    // SHIFT
	 case 0x35 : upper = 1;break;
	 case 0x29 : upper = 1;break;
	 case 0xB5 : upper = 0;break;
	 case 0xA9 : upper = 0;break;

		     // CTRL
	 case 0x1C : ctrl = 1;break;
	 case 0x9C : ctrl = 0;break;

		     // ALT
	 case 0x37 : alt = 1;break;
	 case 0xB7 : alt = 0;break;
		     //del
	 case 0x52 :
		     {
			del = 1;
			ctrlaltdel();
		     }break;
	 case 0xd2 : del = 0;break;


	 default :
		     {

			if(sc<0x3b)
			{     //AUTRES
			   if (maxptr<254) {
			      /*Les racoursis (Ctrl+touche)*/

			      /* Ctrl+l : on efface l'\202cran (\205 la linux :) */
			      CTRL_SHORTCUT(0x25, command[maxptr] = '\0';clrscr();gotoxy(0,2);printk("%s%s",shellstr, command);)

				 if (ptr==maxptr) maxptr++;
				 if (upper|iscaps())
				 {				 
				    command[ptr++]=key(sc*4+1);				    
				    putcar(key(sc*4+1));
				 }
				 else
				 {
				    command[ptr++]=key(sc*4);
				    putcar(key(sc*4));
				 }
			      updateCursor();


			   }
			   //upper = 0;

			}

			if(sc>=0x46 && sc <=0x53 && isnum())            //le pav\202 numerique
			{
			   if (ptr==maxptr) maxptr++;
			   command[ptr++]=key(sc*4);
			   putcar(key(sc*4));
			}

		     } break;
      }

      printsc(sc);        //pour debuguage

   }
   while(1);
}


char * getargs(char * s, char * arg)
{
   char * temp=s;
   char * dest=arg;
   if (*temp=='\0')
   {
      *dest='\0';
      return NULL;
   }

   while(*temp == ' ')  temp++;
   while (((*dest++ = *temp++) != ' ') && (*temp!='\0'));

   if (*temp != '\0' || *(dest-1) == ' ') dest--;
   *dest='\0';

   return temp;  
} 


void exec(char* command)
{

   char cmd[255];
   char * args;
   unsigned char i;

   args=getargs(command,  cmd);


   if (!strcmp("cls\0", cmd)) {
      args=getargs(args,  cmd);
      if (*cmd) 
	 printk("XS32 :: cls takes no parameters\n");
      else 
	 clrscr();
	 gotoxy(0,2);
      return;
   }

   if (!strcmp("help\0",command))
   {
      print_help_en();
      return;
   }

   if (!strcmp("aide\0",command))
   {
      print_help_fr();
      return;
   }

   if (!strcmp("ver\0",command)) {
      printk("XS32 version %s \n",ver);
      return;
   }

   if (!strcmp("mem\0", command)) {
      print_mem();
      return;
   }

   if (!strcmp("reboot\0", command)) {
      _reboot();
      return;
   }

   if (!strcmp("ls\0", command)) {
      list_root_dir1(floppy);
      return;
   }


   if (!strcmp("mount\0", cmd)){
      args=getargs(args,  cmd);
      if (!strcmp("floppy\0", cmd))
	 mountfloppy(&floppy);
      else
	 printk("mount() : Unknown block device %s \n", cmd);
      return;
   }

   if (!strcmp("setkeymap\0", cmd)){
      args=getargs(args,  cmd);
      if (!strcmp("fr\0", cmd))
      {	
	 set_keymap(km_fr);
	 return;
      }	

      if (!strcmp("en\0", cmd))
      {	
	 set_keymap(km_en);
	 return;
      }			 

      printk("set_keymap() : unknown map : %s \n", cmd);
      return;
   }



   if (!strcmp("umount\0", cmd)){
      args=getargs(args,  cmd);
      if (!strcmp("floppy\0", cmd))
	 umountfloppy(&floppy);
      else
	 printk("umount() : Unknown block device %s \n", cmd);
      return;
   }


   if (!strcmp("schelp\0", command)){
      print_schelp_en();
      return;
   }

   if (!strcmp("aidesc\0", command)){
      print_schelp_fr();
      return;
   }





   //Les commandes si dessous sont concue pour le debugage
   //pour les activer editer le fichier 32b/kernel/Makefile et ajouter l'argument -DDEBUG a la variable CC

#ifdef DEBUG
   if (!strcmp("delay\0", command)){
      test_delay();
      return;
   }

   if (!strcmp("bitmem\0", command)){
      print_bit_mem();
      return;
   } 


   if (!strcmp("fdread\0", command)){
      fd_read_test(floppy);
      return;
   } 

   if (!strcmp("genpf\0", command)){
      printk(*fakepointer);
      return;
   } 

   if (!strcmp("dbghelp\0", command)){
      print_dbhelp_en();
      return;
   }

   if (!strcmp("aidedbg\0", command)){
      print_dbhelp_fr();
      return;
   }



   ////////////////////kmalloc
   if (!strcmp("_init\0", command)){
      init_cache();
      return;
   }

   if (!strcmp("alloc\0", cmd)){
      args=getargs(args,  cmd);
      alloc((unsigned long)atoi(cmd));
      return;
   }

   if (!strcmp("free\0", cmd)){
      args=getargs(args,  cmd);
      free((unsigned long)atoi(cmd));
      return;
   }

   if (!strcmp("sbinfo\0", command)){
      sb_info();
      return;
   }
   if (!strcmp("freeinfo\0", command)){
      free_chain_info();
      return;
   }
   if (!strcmp("usedinfo\0", command)){
      used_chain_info();
      return;
   }
   if (!strcmp("avlinfo\0", command)){
      avl_chain_info();
      return;
   }


   if (!strcmp("dump\0", cmd)){
      args=getargs(args,  cmd);
      dump_mem((char *)atoi(cmd));
      return;

   }

   if (!strcmp("prgdt\0", cmd)){
      args=getargs(args,  cmd);
      _dbg_print_gdt((unsigned long)atoi(cmd));
      return;

   }


   if (!strcmp("int100\0", command)){
      asm("int $100;");
      return;
     }
			      

   if (!strcmp("task1\0", command)){
      jump_to_tss(0x40);
      //printk("back from task \n");
      //call_tss(0x40);
      return;
   }
   if (!strcmp("task2\0", command)){
         jump_to_tss(0x48);
       //printk("back from task \n");
      //call_tss(0x40);
       return;
   }
			      



#endif

   if (!strcmp("MoUsE\0",command)) {
      egg0();
      return;
   }
   if (!strcmp("\0",command)) {
      return;
   }

   printk("XS32 : '");
   printk(command);
   printk("' Command not found \n\n");
   return;

}


int print_help_en()
{
   printk("==================\n");
   printk("=  XS32 Help     =\n");
   printk("============================================================================\n");
   printk(" help            : display this help screen\n");
   printk(" cls             : clear the screen\n");
   printk(" ver             : display shell version\n");
   printk(" mem             : display RAM information\n");
   printk(" reboot          : reboot the machine (may crash)\n");
   printk(" mount           : mount a block device (\"mount floppy\" for floppy disk)\n");
   printk(" umount          : unmount a block device\n");
   printk(" ls              : list files in root directory\n");
   printk(" setkeymap map   : load character map, map = fr or en \n");

   printk(" schelp          : Display help for X-OS keyboard shortcuts\n");
#ifdef DEBUG
   printk(" dbghelp         : display help for debugging functions\n");
#endif
   //  printk(" notes           : notes on the XOS project for developers\n");
   printk("============================================================================\n");
   return 0;
}

int print_help_fr()
{
   printk("==================\n");
   printk("=  Aide de XS32  =\n");
   printk("============================================================================\n");
   printk(" aide            : affiche cet \202cran d'aide\n");
   printk(" cls             : efface l'\202cran\n");
   printk(" ver             : affiche la version du shell\n");
   printk(" mem             : donne des informations sur la RAM\n");
   printk(" reboot          : reboot la machine (peut planter)\n");
   printk(" mount           : monte un block (\"mount floppy\" pour la disquette)\n");
   printk(" umount          : demonte un block\n");
   printk(" ls              : liste les fichiers du repertoire racine\n");
   printk(" setkeymap map   : charge la table de caract\212re map, map = fr ou en \n");

   printk(" aidesc          : Affiche l'aide sur les racourcis clavier de X-OS\n");
#ifdef DEBUG
   printk(" aidedbg         : affiche l'aide pour les fonctions de debugage\n");
#endif
   //  printk(" notes           : notes sur le projet XOS pour les developpeurs\n");
   printk("============================================================================\n");
   return 0;
}

#ifdef DEBUG
int print_dbhelp_en(){
   printk("=======================================\n");
   printk("=  Debugging Functions Help          =\n");
   printk("============================================================================\n");
   printk(" delay     : test the timer delay() function\n");
   printk(" bitmem    : display a bitmap of mapped memory pages\n");
   printk(" alloc n   : allocate n bytes in kernel cache\n");
   printk(" free BASE : free the block with base address equal to BASE\n");
   printk(" sbinfo    : display cache superblock information\n");
   printk(" usedinfo  : display list of allocated memory blocks\n");
   printk(" freeinfo  : display list of free memory blocks\n");
   printk(" fdread    : read 1st sector of floppy into floppy buffer\n");
   printk(" prgdt ENTRY : display information about GDT entry ENTRY\n"); 
   printk(" dump ADDR : dump memory at ADDR (can be decimal or hex)\n");
   printk("   NOTE1  hex address format: 0x[0..9][A..F] \n   with UPPERCASE letters\n");
   printk("   NOTE2  attempting to dump unmapped address will\n   generate a Page Fault\n");

   //printk(" fdbuffer        : display floppy buffer contents\n");
   printk("============================================================================\n");
   return 0;
}

int print_dbhelp_fr(){
   printk("=======================================\n");
   printk("=  Aide sur les fonctions de debugage  =\n");
   printk("============================================================================\n");
   printk(" delay     : teste la fonction delay() du timer\n");
   printk(" bitmem    : affiche un bitmap des pages de la memoire mapp\202\n");
   printk(" alloc n   : alloue n octets dans le cache du noyau\n");
   printk(" free BASE : lib\212re le bloc dont la base est egale \205 BASE\n");
   printk(" sbinfo    : affiche des informations sur le superbloc du cache\n");
   printk(" usedinfo  : affiche la liste des blocs  memoire allou\202s\n");
   printk(" freeinfo  : affiche la liste des blocs memoire libres\n");
   printk(" fdread    : lit le 1er secteur de la disquette dans le buffer disquette\n");
   printk(" prgdt ENTRY : donne les informations sur l'entr\202e ENTRY de la gdt\n"); 
   printk(" dump ADDR : dump la memoire ADDR peut etre en d\202cimale ou en haxa\n");
   printk("   NOTE1  en hexa l'addresse est de la forme 0x[0..9][A..F] \n   avec des lettre en MAJUSCULES\n");
   printk("   NOTE2  si vous tentez de dumper une addresse nom map\202e vous\n   g\202n\202rez une Page Fault\n");

   //printk(" fdbuffer        : affiche le contenu du buffer disquette\n");
   printk("============================================================================\n");
   return 0;
}
#endif

void print_schelp_en()
{
   printk("=======================================\n");
   printk("=  Keyboard Shortcuts Help           =\n");
   printk("============================================================================\n");
   printk(" Ctrl+Alt+Del     : Reboot the machine (equivalent to reboot command)\n");
   printk(" Ctrl+l           : Clear screen except current line (like in Linux :)\n");
   printk("============================================================================\n");


}

void print_schelp_fr()
{
   printk("=======================================\n");
   printk("=  Aide sur les raccourcis clavier     \n");
   printk("============================================================================\n");
   printk(" Ctrl+Alt+Suppr   : Reboot la machine (\202quivalent de la commande reboot)\n");
   printk(" Ctrl+l           : Efface l'\202cran sauf la ligne cournate (Comme sous linux :)\n");
   printk("============================================================================\n");


}


int print_txt_logo() {


printk("  __  _   __  __  \n");
printk("  \\ \\/ / / / /_  \n");
printk("   \\/_/ /_/ __/ 32 ]-------------+\n");
printk("   / /\\                          |\n");
printk("  __/\\_\\ By Alaa-eddine KADDOURI |\n");
printk("         https://xos.eurekaa.org |\n");
printk("---------------------------------+\n");
				


   return 0;
}
