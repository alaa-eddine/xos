;****************************************************************************;
;**                                                                        **;
;**                              OUTPUT TOOLS                              **;
;**                                                                        **;
;****************************************************************************;
;*** Auteur : Alaa eddine         *******************************************;
;***          KADDOURI            *******************************************;
;****************************************************************************;
;*  Ensemble de procedures et fonction utilisant UNIQUEMENT les ints BIOS   *;
;*  pour l'affichage (chaines ASCIIZ, entier, binaire, hexa...)             *;
;*                                                                          *;
;*  Set of procedures and functions usin BIOS ints for output               *;
;*                                                                          *;
;****************************************************************************;
;****************************************************************************;

;*****************************************;
;*                                       *;
;*****************************************;
;* ENTREE :                              *;
;*****************************************;
;* SORTIE :                              *;
;*****************************************;

;*****************************************;
;*     affichage d'une chaine ASCIIZ     *;
;*****************************************;
;* ENTREE :                              *;
;* SI := adresse de la chaine a afficher *;
;*****************************************;
;* SORTIE : AUCUNE                       *;
;*****************************************;
P_print:
              push ax
              push bx
              push si

              cld
 .next:
              lodsb
              or   al, al
              jz   .end
              mov  ah, 0Eh
              mov  bl, 0007h
              int  10h
              jmp  .next

 .end:
              pop  si
              pop  bx
              pop  ax
              ret


;*****************************************;
;*    affichage d'un caractŠre ascii     *;
;*****************************************;
;* ENTREE :                              *;
;* AL := code ASCII                      *;
;*****************************************;
;* SORTIE : AUCUNE                       *;
;*****************************************;

P_printch:
              push ax
              push bx
              cmp al, 0
              je .end
              mov  ah, 14
              mov  bl, 15
              int  10h
.end:
              pop  bx
              pop  ax
              ret



;*****************************************;
;*        affichage d'un entier          *;
;*****************************************;
;* ENTREE :                              *;
;* ax := nombre … affiche                *;
;*****************************************;
;* SORTIE : AUCUNE                       *;
;*****************************************;

P_pr_ent:
              push ax
              push cx
              push bx
              push dx

              mov cx, 0
              mov bx, 10
    .TQ:

              mov  dx, 0
              div  bx
              push dx

              inc  cx

              cmp  ax, 0
              je  .FTQ

              jmp  .TQ
    .FTQ:

    .next:
              pop  ax
              add  ax, 48
              call P_printch
              loop .next

              pop dx
              pop bx
              pop cx
              pop ax
              ret


;*****************************************;
;*        Calcul de la taille d'une chaine ASCIIZ          *;
;*****************************************;
;* ENTREE :                              *;
;* si := chaine de caractères                *;
;*****************************************;
;* SORTIE : cx = taille                       *;
;*****************************************;


P_size:
    push si
    mov cx, 0
.df:
    cmp [si], byte 0
    je  .ff
    inc si
    inc cx
    jmp .df
.ff:
    pop si
    ret

;*****************************************;
;*        Ceci est une macro pour tester si seux chaines sont identiques          *;
;*****************************************;
;* ENTREE :                              *;
;* les deux chaines             *;
;*****************************************;
;* SORTIE :
;                 ax = 0 les chaines sont differentes
;                 ax = 1 les chaines sont égales                       *;
;*****************************************;
%macro is_equal 2 ;Compare deux chaines ASCIIZ

    push cx
    push si
    push di

    mov si, %2
    push si
    call P_size
    pop di

    mov ax, cx

    mov si, %1
    push si
    call P_size
    pop si

    cmp ax, cx
    jne .non

    rep cmpsb
    je  .oui

.non:
    mov ax, 0
    jmp .fin
.oui:
    mov ax, 1
.fin:

    pop di
    pop si
    pop cx
%endmacro

