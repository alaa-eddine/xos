
xos_int21h:
   .c00:                ;retour au shell appelant
         cmp ah, $00
         jne .c01
         jmp .ah00

   .c01:
         cmp ah, $01
         jne .c02
         jmp .ah01

   .c02:
         cmp ah, $02
         jne .c03
         jmp .ah02
   .c03:
         cmp ah, $03
         jne .c04
         jmp .ah03

   .c04:
         cmp ah, $04
         jne .c05
         jmp .ah04

   .c05:
         cmp ah, $05
         jne .c06
         jmp .ah05

   .c06:
         cmp ah, $06
         jne .c07
         jmp .ah06

   .c07:
         cmp ah, $07
         jne .c08
         jmp .ah07

   .c08:
         cmp ah, $08
         jne .c09
         jmp .ah08



;>>>>>>>>  Fonction 09h <<<<<<<<
   .c09:
         cmp ah, $09
         jne .c0A
         jmp .ah09



   .c0A:				
         cmp ah, $0A
         jne .c0B
         jmp .ah0A

   .c0B:
         cmp ah, $0B
         jne .c0C
         jmp .ah0B

   .c0C:
         cmp ah, $0C
         jne .cFF            ;ou .c0D  si on veut continu‚ ...
         jmp .ah0C


   .cFF:
         cmp ah, $0FF
         jne .exit
         jmp .ahFF

;*************************************************************************


;>>>>>>>>  Fonction 00h <<<<<<<<
; Fin du programme en cours => retour au shell (chargé à l'addresse 0x3000:0x0000)
    .ah00:
         mov dx, 0ABCDh ;signature ;-p
         jmp 03000h:0000h

         jmp .exit      ;cette ligne n'est jamais execut‚e (normalement ;-D)



    .ah01:
         jmp .exit

    .ah02:
         jmp .exit

    .ah03:
         jmp .exit

    .ah04:
         jmp .exit

    .ah05:
         jmp .exit

    .ah06:
         jmp .exit

    .ah07:
         jmp .exit

    .ah08:
         jmp .exit



;>>>>>>>>  Fonction 09h <<<<<<<<
;
;Affichage d'une chaine de caractŠre (ASCIIZ)
;-----------------------------------
;Entr‚es : ah = 09h
;
;          DS:SI = pointeur vers la chaine … afficher.
;
;Sortie  : Aucune  (la chaine s'affiche … l'‚cran)
;
;RQ : la chaine doit ‚tre termin‚e par le code ascii 0. (ASCIIZ)

    .ah09:
;              push ax
;              push bx
;              push si

              cld
 .next:
              lodsb
              or al, al      ; al = 0 ?
              jz   .end      ; oui ==> fin de chaine
              mov  ah, 0Eh   ; non ==> on affiche le caractaire et on passe au suivant
              mov  bl, 0007h
              int  10h
              jmp  .next

 .end:
;              pop  si
;              pop  bx
;              pop  ax

         jmp .exit




    .ah0A:
         jmp .exit

    .ah0B:
         jmp .exit

    .ah0C:
         jmp .exit


    .ahFF:
         jmp .exit


    .exit:
         iret
