;--------------------------------------------------------------------------
; X Operating Systeme
;
; @Auteur : Alaa eddine KADDOURI
; @projet : X Operating System (X-OS)
; @version : XOS-KERnel 16bits  v0.6
;--------------------------------------------------------------------------
; -Pour toute remarque, question, bug, suggestion...
;  Contactez moi a webmaster@wonderworlds.net.
;
;
; -Pour participer au projet X-OS ou suggerer tout autre projet en asm ou
;  en c, rendez-vous sur www.wonderworlds.net > projets > X-OS
;
;--------------------------------------------------------------------------

[BITS 16]		;Set code generation to 16 bit mode
[ORG 0x0000]		;Set code start address to 0000h

[SEGMENT .text]		;Main code segment

              jmp start


%include "sysinfo.inc"


;>>> PACKAGES UTILISES
;************************************************************;
%include "output.inc"
%include "macros.inc"   ;**
%include "colors.inc"   ;**
%include "video.inc"    ;**
%include "vesa.inc"     ;**
%include "vga.inc"     ;**
%include "int21h.inc"   ;**
%include "mouse.inc"    ;**
%include "fat12.inc"
%include "tools.inc"    ;**
filetoload: db 0,0,0,0,0,0,0,0,0,0,0,0,0

;************************************************************;
SX32	db 'SW32.XSS',0
;********** PROGRAMME PRINCIPALE ****************************;
start:

         call P_init      ;initialisation
         cmp ax, 0
         je noshell

	 mov ax, 09200h
         jmp 09200h:0000h ; saut vers l'addresse ou � �t� charg� le shell
                          ;

noshell:
         write MSG_e
         writeln MSG_shellnotfound


         write MSG_i
         writeln MSG_hang
         jmp $; HANG ??   ;si tout va bien cette ligne
                          ;n'est jamais execut�e :-p






P_init:

;         mov ax, 09000h     ; AX est desormais initialis� par le secteur de boot

         mov es, ax
         mov ds, ax


         mov di, logoname
         mov ax, 01000h
         writeln msg_loadgr
         call P_load        ;chargement du logo


         cmp ax, 0
         je nologo

logo:
         push ds
         mov ax, 01000h
         mov ds, ax
         ;mov cx, 0
         call      P_aff_grlogo
         pop ds
         jmp endlogo

nologo:
         writeln msg_nologo
         call P_readkey


endlogo:



         call P_read_conffile  ; r�cuperation de la config de demarrage

         mov ax, 9200h
         mov di, shellname
         call P_load
         push ax

         mov ah, 0
         mov al, 3h
         int 10h

	 call      P_inittext
         call      P_clscr
         mov       bl, [Red]
         call      P_setbkcolor


         write   MSG_logo
;         write ln



         write   MSG_version
         call    P_print_version_number
         call    P_print_last_compil_date
;         write   MSG_001
;         write ln


         call P_print_vesainfo
         write ln

         ;****************************D�tournement de l'interruption 21h
	 ; les sous fonctions de l'interruption peuvent etres appell�s par les programmes execut�s par XOS
         writeln MSG_det21h
         mov ax, cs
         push ds
         push ds
         xor bx, bx
         mov ds, bx
         mov ax, [ds:$21*4]
         mov dx, [ds:$21*4+2]
         pop ds

;         mov word [ofs13], ax
;         mov word [seg13], dx


         xor bx, bx
         mov ax, xos_int21h
         mov dx, cs
         mov ds, bx
         cli
         mov word [ds:$21*4], ax
         mov word [ds:$21*4+2], dx
         sti

         pop ds
__ok:
         pop ax

         ret



P_clscr:
         mov       bh, byte [clsattr]
         call      P_cls
         ret



;************************************************************;
;Cette procedure attend que l'utilisateur press une touche et
;renvoi le code ascii de cette derni�re dans al
P_readkey:

;   .getkey:
;    	      mov ah, 1
;             int 16h
;             jnz .getkey

              mov ah, 0
              int 16h
              ret

%DEFINE		confshellname   0x1200
%DEFINE		linbuffer	0x1300

P_read_conffile:
             push es
             push ds

             mov ax, 01000h
             mov di, bootconf
             call P_load
             cmp ax, 0
             je .ret
             mov ax, 01000h
             mov ds, ax
             mov si, 0
             mov di, linbuffer
             cld
             mov al, 0
.getpart:
             lodsb
             stosb
             cmp al, '='
             jne .getpart
	     pop ds
	     push ds
             is_equal linbuffer, shellstr
             cmp ax, 0
	     je .noshellstring
	     mov ax, 01000h
	     mov ds, ax
.getshell:
             mov di, confshellname
.lodshell:
             lodsb
;	     cmp al, ':'
;	     je .endlodshell
             cmp al, 10
;             je .is13
;.is13:
;             cmp al, 13
             je .endlodshell

	     stosb
	     jmp .lodshell
.endlodshell
	     mov al, 0
	     stosb

;	    xor bx, bx
;.getaddr:
;	     xor ax, ax
;             lodsb
;             cmp al, 10
;             je .is13
;.is13:
;             lodsb
;             cmp al, 13
;             je .endgetaddr

;	     sub ax, 40
;	     add bx, ax
;	     jmp .getaddr
.;endgetaddr:
;	     mov ax, bx
;	     writenum ax
;	     call P_readkey
             pop ds
             pop es

.end

.noshellstring:

.ret:
	     mov si, confshellname
	     mov di, shellname
	     call P_formatbuffer
             ret

P_formatbuffer:
;ENTREE
;SI : buffer source
;DI : buffer cible (12 oct)
;Sortie:
;AUCUNE (buffer format�)
         call P_size

         cmp cx, 12
         jg  .notcomfile

         mov cx, 11
         xor ax, ax
.deb:
         cmp ah, ' '
         je .nolod
         jmp .lod
.nolod:
         cmp cx, 3
         jg .nolodsb

.lod:
         lodsb
.nolodsb:
         cmp al, '.'
         je .point
         mov ah, al
         jmp .end
.point:

         cmp cx, 3
         jg .sup3
         je .eq3
         mov ah, al
         jmp .end
.sup3:
         mov ah, ' '
         jmp .end
.eq3:
         jmp .lod
.end:
         mov al, ah
         stosb
         loop .deb

         mov al, 0
         stosb

         mov cx, 1
         jmp .ret

.notcomfile:
         mov cx, 0
.ret:
         ret

[SEGMENT .data]		;Initialised data segment
;************************************************************;



grlin      dw 0
grcol      dw 0
gradr      dw 0

clsattr  db 31



shellstr      db 'SHELL=',0
logostr       db 'LOGO=',0
shellname     db '00000000000',0
bootconf      db 'BOOTCONFCFG',0
logoname      db 'XOSLOGO BMP',0;non utilis� pour l'instant

kernel32	db	'XOSKER32XSS',0
logosize      dw 17078


;**********************************************************************
MSG_shellnotfound:  DB "!!! ERREUR !!!",13,10
                    DB "L'interpreteur de commandes est introuvable",10,13
                    DB "le systeme n'arrive pas a charger un shell de commande",13,10
                    DB "assurez vous que le fichier bootconf est present sur la disquette et qu'il contient un nom de shell valide",13,10,0
MSG_hang:           DB "ARRET DU SYSTEME",0

MSG_i:              DB '|i|> ',0
MSG_e:              DB '|e|> ',0

MSG_det21h:         DB 'Detournement de int 21h...',0;**


MSG_logo:	 DB 'Lancement de X-OS KERnel 16bits...',10,13,0 ;**

;**
MSG_001 :        DB 'Site web https://xos.eurekaa.org',10,13,0
                 ;DB 'Pour connaitre les commandes disponnibles tapez aide ',10,13,0


MSG_version:     DB 'X-OS version ',0;**
MSG_lastcompil:  DB 'd�rni�re compilation le ',0;**

MSG_MO:          DB ' Mo',0 ;**
MSG_KO:          DB ' Ko',0

pix:             DB '.',0 ;**
slash:           DB '/',0 ;**
space:           DB ' ',0

;************************************************************;
mode_16	db '16B',0
mode_32	db '32B',0
msg_loadgr:   db 13,10,'Chargement termin�',13,10,'Lancement du systeme...',0
msg_getconf	db 'R�cuperation des param�tres de demarrage',0
msg_nologo:   db 'logo non trouv�!!! ',10,13
              db 'Assurez vous que le fichier xoslogo.bmp se trouve sur la disquette X-os',13,10
              db 'Appuyez sur une touche pour ignorer le probl�me',0


