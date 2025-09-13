


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
%include "keyboard.inc"    ;**
%include "mouse.inc"    ;**
%include "fat12.inc"
%include "tools.inc"    ;*
filetoload: db 0,0,0,0,0,0,0,0,0,0,0,0,0

;************************************************************;
;>>> SOUS-PROGRAMMES
;************************************************************;





;*************************************************************
P_vesademo:
    push ds

    mov ax, 01000h
    mov ds, ax
    call P_vesa_grlogo

    pop ds

    call      P_inittext
    mov       bh, byte [clsattr]
    call      P_clscr
    mov       bl, [Red]
    call      P_setbkcolor
    ret




;P_old:
;         jmp 06000h:0000h
;         ret
;*************************************************************

;P_int21h:
;         int 21h
;         ret



msg_shell:    db   13,10,'XS16 version 1.0',13,10
              db   'Auteur Alaa eddine KADDOURI ',13,10
	      db   'Tapez aide pour voir les commandes disponnibles',13,10,0



P_init:


         ;mov ax, 03000h      ;ax est desorme initialis� par le kernel 16bits   16b/kernel/xosker.asm
	mov ax, cs
         mov es, ax
         mov ds, ax

         cmp dx, 0ABCDh      ; si on revien au shell a partir d'un programme
         je .nomsg            ; on n'affiche pas de message

         writeln MSG_LOGO16   ; sinon on affiche le message du shell
         writeln msg_shell   ; sinon on affiche le message du shell
         jmp .continue
  .nomsg:
         write ln
  .continue:
         call P_init_keybuf     ;on initialise le buffer clavier
         ret



;************************************************************;
P_OS_loop:

  go:

         write     OS_prompt

  get_command:
         call P_readkey
         push ax
         call P_formatkey
         call P_printch
         pop  ax

         cmp al, 0           ;caract�re ASCII �tendu ??
         je  get_command

         cmp al, 13          ;appui sur entrer
         je  return

         call P_update_keybuf
         jmp get_command


  return:
    write ln

    cmp byte [bpos], 0       ;buffer vide on revien au prompt
    je go
    cmp byte [bpos], 254     ;on a attein la taille max du buffer!!!
    je nontrouve             ;==> commande inexistante

    mov al, 0
    call P_update_keybuf

    mov dx, 0
 deb:
    inc dx

    mov bx, ptcom
    add bx, dx
    cmp [bx], word $0FFFF
    je nontrouve

    is_equal [bx], keybuf

    cmp ax, 1
    je trouve
    jmp fin
 trouve:
    mov bx, comm
    add bx, dx
    call [bx]
    jmp ndeb

fin:
    jmp deb

 nontrouve:
 mov si, keybuf
 mov di, comfile
 call P_formatbuffer
 cmp cx, 0
 je nobin

 mov ax, 06000h
 mov di, comfile

 call P_load
 cmp ax, 0 ; fichier non trouv�
 je nobin

 cmp ax, 1 ; trouv� mais non executable
 je nobin

 cmp ax, 2 ; le fichier contient la signature XBIN16
 jne nobin

 jmp 06000h:0000h

nobin:

 writeln MSG_errcomm
 write ln

ndeb:


         call P_init_keybuf

;         writeln MSG_end

  end:
         jmp go

         ret




;************************************************************;
; sous programmes ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;************************************************************;
comfile: db '000000000000',0,0,0,0
mfic:    db 'Nom        ',0
matr:    db 'attributs',0
mspace:  db '     ',0

swt32:	db 'SW32    XSS',0
MSG_errswt32 :	db '|i|> Le fichier SW32.XSS est introuvable',10,13
		db 'ce fichier est necassaire pour basculer vers le mode 32bits',10,13,
		db '|e|> Impossible de basculer vers le mode protege ',10,13,0


P_xos32:
 mov ax, 02000h
 mov di, swt32
 call P_load
 cmp ax, 0
 je .nosw32

 ;mov dx, 0DCBAh      ; signature de basculation  en mode 32bits

;mov ax, 02000h
 jmp 02000h:0
.nosw32:

 writeln MSG_errswt32
 write ln

	ret;

P_mem1Mo:
         ;*************************************************************
                             ;L'interruption 12h
         write   MSG_mem     ;renvoi dans ax la taille
         int 12h             ;de la memoire conventionnelle
                             ;libre en Ko
         writenum ax         ;
         writeln   MSG_KO    ;
         ret
         ;*************************************************************

;*************************************************************


P_clscr:
         mov       bh, byte [clsattr]
         call      P_cls
         ret

;*************************************************************

P_ls:
         write ln
         write mfic
         write mspace
         writeln matr
         write ln

         mov ax, 19
.lo1:
         push ax
         mov bx, buffer

         Call  P_readsector
         mov cx, 16
         mov dl, 1
.lo2:
         push cx
         mov di, file
         mov bx, buffer
         call P_buf2file
         mov bx, file
         call P_decompfile
         cmp byte [filename], 0
         je .next1
         cmp word [attr], 15  ;15 <==> fichier supprim�
         je .next1

             write filename
             call P_getattr
             write mspace
             writeln attributstr

.next1:
         inc  dl

         pop cx
         loop .lo2
         pop  ax
         cmp  ax, 32
         jge  .finish
         inc  ax
         jmp  .lo1
.finish:
         write ln
         write ln
         ret

;************************************************************;



P_ver:
         write   MSG_version
         call    P_print_version_number
         call    P_print_last_compil_date
         ret

;************************************************************;

P_help:
         writeln CM_help
         ret



;************************************************************;

P_reboot:
         call s_reboot
         ret



;************************************************************;








;********** PROGRAMME PRINCIPALE ****************************;
start:

         call P_init      ;initialisation

         call P_OS_loop   ;boucle systeme

         jmp $; HANG ??   ;si tout va bien cette ligne
                          ;n'est jamais execut�e :-p

;************************************************************;





[SEGMENT .data]		;Initialised data segment





;******************************************************************;
;**  Table des pointeurs vers les commandes internes du syst�me  **;
;******************************************************************;

;;Les routines execut�s par les commandes
comm:         DB $00
ver:          dw P_ver
help:         dw P_help
reboot:       dw P_reboot
mem1Mo:       dw P_mem1Mo
cls:          dw P_clscr
ls:           dw P_ls
vesa:         dw P_vesademo
;int21h:       dw P_int21h
xos32:        dw P_xos32
;_test:        dw P_old
              dw $0FFFF


;;Les commandes
TABCOM        DB $00
C_ver         db 'ver',0
C_help        db 'aide',0
C_reboot      db 'reboot',0
C_mem1Mo      db 'mem',0
C_cls         db 'cls',0
C_ls          db 'ls',0
C_vesa        db 'VESADEMO',0
;C_int21h      db 'int21h',0
C_xos32        db 'xos32',0
;C_test        db '_Test',0
              db $0FF


;;Les pointeurs : (Relation entre les noms de commandes et les routines)

ptcom         db $00              ;     NOMS               ADRESSES          SOUS PROGRAMMES
PT_ver        dw C_ver            ;     _________          ________          ___________
PT_help       dw C_help           ;    |Table des|        |Poiteurs|        |Les        |
PT_reboot     dw C_reboot         ;    |commandes|        |        |        |routines   |
PT_mem1Mo     dw C_mem1Mo         ;    |Contien  |    --->|liaisons|------->|(Sous      |
PT_cls        dw C_cls            ;    |les noms |   |    |vers les|        |programmes)|
PT_ls         dw C_ls             ;    |des      |---     |routines|        |-----------|
PT_vesa       dw C_vesa           ;    |commandes|        |�       |
;PT_int21h     dw C_int21h         ;     ---------         |executer|
PT_xos32      dw C_xos32
;PT_test       dw C_test
              dw $0FFFF           ;                       ----------
;************************************************************;



grlin      dw 0
grcol      dw 0
gradr      dw 0

clsattr  db 31



ofs13    dw 0
seg13    dw 0

bpos               db 0; position dans le buffer clavier


;********************************************************************;
%include "messages.inc" ;Ce fichier contient les messages systeme
MSG_LOGO16:                        ;
                 DB 10,13
                 DB ' ����������������������������Ŀ',10,13
                 DB ' �  \ /                       ��',10,13
                 DB ' �  / \ Operating system      ��',10,13
                 DB ' �               Mode 16 bits ��',10,13
                 DB ' �  Auteur : Alaa eddine      ��',10,13
                 DB ' �                KADDOURI    ��',10,13
                 DB ' �������������������������������',10,13
                 DB '  ������������������������������',10,13
		 DB '          https://xos.eurekaa.org',10,13,0
                        ;
;********************************************************************;

logoname db 'XOSLOGO BMP',0  ;logo de X-OS




