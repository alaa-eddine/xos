;----------------------------------------------------------------------------;
;--                                                                        --;
;--                              FAT12 TOOLS                               --;
;--                                                                        --;
;----------------------------------------------------------------------------;
;--- Auteur : Alaa eddine         -------------------------------------------;
;---          KADDOURI            -------------------------------------------;
;****************************************************************************;
;*  Ensemble de procedures et fonction utilisant UNIQUEMENT les             *;
;*  interruptions BIOS pour manipuler le system de ficher FAT12             *;
;****************************************************************************;
;****************************************************************************;
;****************************************************************************;

%DEFINE fatbuffer          0x4000 ;buffer FAT12
%DEFINE buffer             0x5200 ;buffer disquette
%DEFINE file		   0x5400 ;buffer Entree de FAT12


attr:              DB 0
cluster:           DW 0
size:              DW 0
sizelo:            DW 0
filename:          DB 0,'FILENAM'
ext:               DB 'EXT',0




;************************************************************;
;**  structure pour la manipulation des attributs fichier  **;
;************************************************************;
ATTR:    DB $00                                              ;
RO:      DB $01                                              ;
HID:     DB $02                                              ;
SYS:     DB $04                                              ;
LBL:     DB $08                                              ;
DIR:     DB $10                                              ;
ARC:     DB $20                                              ;
                                                             ;
TABATTR: DB $00                                              ;
TRO:     DB 'R'                                              ;
THID:    DB 'H'                                              ;
TSYS:    DB 'S'                                              ;
TLBL:    DB 'V'                                              ;
TDIR:    DB 'D'                                              ;
TARC:    DB 'A'                                              ;
                                                             ;
attributstr:  DB '______',0                                  ;
                                                             ;
;************************************************************;

;**************************************************************;
;**  format de l'octet contenant les attributs d'un fichier  **;
;**************************************************************;
;-----------------------------                                 ;
;  bit  |  attribut                                            ;
;-------|---------------------                                 ;
;   0   |  Lecture seul                                        ;
;   1   |  Cach�                                               ;
;   2   |  System                                              ;
;   3   |  Non de volume                                       ;
;   4   |  Repertoir                                           ;
;   5   |  Archive                                             ;
;   6   |  0                                                   ;
;   7   |  0                                                   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;00 000001  RO      $01
;00 000010  HID     $02
;00 000100  SYS	    $04
;00 001000  LBL     $08
;00 010000  DIR	    $10
;00 100000  ARC     $20
;*****************************************;


;*****************************************;
;*                                       *;
;*****************************************;
;* ENTREE :                              *;
;*****************************************;
;* SORTIE :                              *;
;*****************************************;

;*********************************************;
;* Convertion du secteur logique en phisyque *;
;*********************************************;
;* ENTREE :                                  *;
;* AX := numero logique du secteur (0..2879) *;
;*********************************************;
;* SORTIE :                                  *;
;* cl : numero du secteur (1..18)            *;
;* dh : numero de la piste (0..79)           *;
;* ch : numero de la face (0 ou 1)           *;
;*********************************************;
P_sl2p:
;              push bx
;              push ax

;              mov bx, ax

;              mov dl, 18
;              div dl
;              mov cl, ah
;              inc cl

;              cbw
;              mov dl, 2
;              div dl
;              mov dh, ah

;              mov ax, bx
;              mov dl, 36
;              div dl
;              mov ch, al

;              pop ax
;              pop bx
;              ret
	xor dx,dx             ; pr�paration � une division
	push ax               ; sauvegarde de ax
	push bx               ;    "        " bx
	mov bx,18             ; 18 secteurs/piste
	div bx                ; on divise le n� de secteur par le nb de secteurs/pite
	                      ; pour r�cup�rer le n� de secteur sur la piste.
;	pop bx                ; restauration de bx (� optimiser)
	mov cx,dx             ; le reste de la division dans cx (pour int 13h)
	xor dx,dx             ; pr�paration de la division
;	push bx               ; sauvegarde de bx (� optimiser)
	mov bx,2              ; on divise par 2 le reste de la division pr�c�dante
	div bx                ; pour r�cup�rer sur quelle t�te se trouve le secteur.
	pop bx                ; restauration de bx
	mov dh,dl             ; on place le n� de t�te dans dh (pour int 13h)
;	ror ah,1              ; on d�cale � droite ah
;	ror ah,1              ; encore une fois
	ror ah,2              ; on d�cale 2 fois � droite ah
	xchg al,ah            ; et on �change ah et al
	mov dl, 18            ; 18 secteurs/piste
	sub dl,cl             ; on soustrait le reste de la division qui permet d'obtenir
	                      ; le n� de secteur sur la piste
	inc cl                ; et en ajoutant 1 on obtient le n� de piste
	or cx,ax              ; sans oublier d'ajouter les bits de ax.
	mov dl,al             ; On a le n� de la t�te dans al
	pop ax                ;
ret



;*********************************************;
;*          lecture d'un secteur             *;
;*********************************************;
;* ENTREE :                                  *;
;* AX := numero logique du secteur (0..2879) *;
;* BX := le buffer qui recevra les donn�es   *;
;*********************************************;
;* SORTIE : AUCUNE                           *;
;*********************************************;

P_readsector:
              push ax
              push dx

              call P_sl2p

              mov ah, 02h
              mov al, 01h
              mov dl, 0
              int 13h

              pop dx
              pop ax
              ret







;**********************************************;
;* extraction d'une entree du repertoir       *;
;* principal depuis un buffer                 *;
;**********************************************;
;* ENTREE :                                   *;
;* BX : buffer source 512 octets              *;
;* DL : numero de l'entrer (1..16)            *;
;* DI : buffer cible 32 octets                *;
;*                                            *;
;**********************************************;
;* SORTIE : AUCUNE                            *;
;**********************************************;

P_buf2file:
              push ax
              push bx
              push cx
              push si

              mov al, 32     ;on calcule le rang de l'entr�e qu'on veut
              mul dl         ;extraire
              sub ax, 32     ;
              add bx, ax     ;on ajoute le rang au buffer

              mov si, bx     ;puis on affectue notre copie  ;-)
              mov cx, 32     ;chaque entr�e contien 32 octets
    .getfile:
              lodsb
              stosb
              loop .getfile

.fin:
              pop si
              pop cx
              pop bx
              pop ax
              ret

;**********************************************;
;* decomposition des entr�es du rep principal *;
;**********************************************;
;* ENTREE :                                   *;
;* BX : le buffer qui contient l'entr�e a     *;
;*      decompos�e (32 octets)                *;
;* di : le buffer du nom de fichier           *;
;**********************************************;
;* SORTIE :                                   *;
;*   filename: nom du fichier                 *;
;*   attr : contien les attribut              *;
;*   cluster : contien le 1er cluster du      *;
;*             fichier                        *;
;**********************************************;
P_decompfile:
              push ax
              push bx
              push cx
              push di
              
              mov si, BX
              mov di, filename
              mov cx, 11

    .getfilename:
              lodsb
              cmp cx, 11
              jne .stock
               cmp al, 31
               jg  .stock
               mov al, 0
               stosb
               jmp .nofile
    .stock:
              stosb

              loop .getfilename


              add bx, 11
              mov si, bx
              lodsb
              mov [attr], al

              add bx, 15
              mov si, bx
              lodsw
              mov [cluster], ax

		inc bx
		mov si, bx
		lodsw
		mov [size], ax
		lodsw
		mov [sizelo], ax

    .nofile:

              pop di
              pop cx
              pop bx
              pop ax
              ret

;**********************************************;
;*    lecture du prochain cluster dans ax     *;
;**********************************************;
;* ENTREE :                                   *;
;* BX : buffer source 512 octets              *;
;* AX : numero du cluster courant [0..2879]   *;
;*                                            *;
;**********************************************;
;* SORTIE :                                   *;
;* AX : numero du cluster suivant             *;
;**********************************************;
;* REMARQUE : pour connaitre le secteur       *;
;* correspendant a un cluster la formule est  *;
;* numsecteur = 33 + numcluster - 2           *;
;*                                            *;
;* > 33 est le numero du 1er secteur dans une *;
;*   FAT12                                    *;
;* > 2 est le nombre d'elements de 12 bits    *;
;*   utilis�e pour disteinguar une FAT12.     *;
;*   la table FAT12 commence par :            *;
;*   F0F FFF                                  *;
;**********************************************;

P_getnextcluster:
         push bx        ;en sauveguarde le buffer 2 fois
         push bx        ;car il va doit recuper� 2 fois

         add ax, 1      ;

         mov bx, 12     ;
         mul bx         ;ax := ax*12
         mov bx, 8      ;
         div bx         ;ax := ax DIV bx
                        ;dx := ax MOD bx

         pop bx         ;on recupere le buffer
         add bx, ax     ;on ajoute le nombre de deplacements necessaires

         cmp  dx, 4     ;si le reste de ax/12 = 4
         je   .R4
.R0:
         sub  bx, 2                   ;si le reste = 0 on ajuste bx
         mov  ax, word [bx]           ;on lit le buffer a la position bx
         sar  ax, 4                   ;puis on decale ax de 4 a droite
         and  ax, 0000111111111111b   ;MASK des 4 premiers bits
         jmp  .endr                   ;car on a besoin de 12bits seulement
.R4:
         sub  bx, 1                   ;si le reste = 4 on ajuste bx
         mov  ax, word [bx]           ;on lit
         and  ax, 0000111111111111b   ;puis on MASK
.endr:
         pop  bx                      ;on recupere notre buffer initial
         ret

;Cette procedure met a jour la chaine attributstr en fonction
;de la valeur recuper�e dans la variable [attr]
;(VOIR LE FICHIER fat12.inc)
P_getattr:
         mov di, attributstr

         mov bx, 1
   .isattr:
         mov al, '_'
         mov ah, [attr]
         and ah, [ATTR+BX]
         xor ah, [ATTR+BX]
         jnz .notattr
         mov al, [TABATTR+BX]
   .notattr:
         stosb
         cmp al, 'D'
         je  .rep

         cmp bx, 6
         je  .end
         inc bx
         jmp .isattr
    .rep:
         mov di, attributstr
         mov al, '['
         stosb
         mov al, 'R'
         stosb
         mov al, 'E'
         stosb
         mov al, 'P'
         stosb
         mov al, ']'
         stosb
         mov al, ' '
         stosb
    .end:

         ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
