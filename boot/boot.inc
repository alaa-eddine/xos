;----------------------------------------------------------------------------;
;--                                                                        --;
;--                              BOOT  TOOLS                               --;
;--                                                                        --;
;----------------------------------------------------------------------------;
;--- Auteur : Alaa eddine         -------------------------------------------;
;---          KADDOURI            -------------------------------------------;
;****************************************************************************;
;Une serie d'outils permettant au secteur de boot de géerer la fat12
;et d'afficher des messages
;
;
;
;****************************************************************************;
%define cluster 0xFF0



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
	xor dx,dx             ; préparation à une division
	push ax               ; sauvegarde de ax
	push bx               ;    "        " bx
	mov bx,18             ; 18 secteurs/piste
	div bx                ; on divise le n° de secteur par le nb de secteurs/pite
	                      ; pour récupérer le n° de secteur sur la piste.

	mov cx,dx             ; le reste de la division dans cx (pour int 13h)
	xor dx,dx             ; préparation de la division

	mov bx,2              ; on divise par 2 le reste de la division précédante
	div bx                ; pour récupérer sur quelle tête se trouve le secteur.
	pop bx                ; restauration de bx
	mov dh,dl             ; on place le n° de tête dans dh (pour int 13h)


	ror ah,2              ; on décale 2 fois à droite ah
	xchg al,ah            ; et on échange ah et al
	mov dl, 18            ; 18 secteurs/piste
	sub dl,cl             ; on soustrait le reste de la division qui permet d'obtenir
	                      ; le n° de secteur sur la piste
	inc cl                ; et en ajoutant 1 on obtient le n° de piste
	or cx,ax              ; sans oublier d'ajouter les bits de ax.
	mov dl,al             ; On a le n° de la tête dans al
	pop ax                ;
ret



;*********************************************;
;*          lecture d'un secteur             *;
;*********************************************;
;* ENTREE :                                  *;
;* AX := numero logique du secteur (0..2879) *;
;* BX := le buffer qui recevra les donnes   *;
;*********************************************;
;* SORTIE : AUCUNE                           *;
;*********************************************;

P_readsector:
              push ax
              push dx

              call P_sl2p         ;conversion secteur logique en physique

              mov ah, 02h         ;fonction de lecture
              mov al, 01h         ;d'un secteur
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

              mov al, 32     ;on calcule le rang de l'entre qu'on veut
              mul dl         ;extraire
              sub ax, 32     ;
              add bx, ax     ;on ajoute le rang au buffer

              mov si, bx     ;puis on affectue notre copie  ;-)
              mov cx, 16     ;chaque entre contien 32 octets
    .getfile:
              movsw
              loop .getfile

.fin:
              pop si
              pop cx
              pop bx
              pop ax
              ret

;**********************************************;
;* decomposition des entres du rep principal *;
;**********************************************;
;* ENTREE :                                   *;
;* BX : le buffer qui contient l'entre a     *;
;*      decompose (32 octets)                *;
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



              add bx, 26
              mov si, bx
              mov di, cluster     ;cluster = 0xFF0   
              mov cx, 3           ;lecture de 6 octets
              rep movsw		  ;2 octets pour le premier cluster suivi de 4 octets de la taille du kernel
    .nofile:                        

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
;*   utilise pour disteinguar une FAT12.     *;
;*   la table FAT12 commence par :            *;
;*   F0F FFF                                  *;
;**********************************************;

P_getnextcluster:
         push bx        ;en sauveguarde le buffer 2 fois
         push bx        ;car il va doit recuper 2 fois

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
;              push si

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
;              pop  si
              pop  bx
              pop  ax
              ret


