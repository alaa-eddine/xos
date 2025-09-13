;************************************************************;

MSG_MO:          DB ' Mo',0 ;**
MSG_KO:          DB ' Ko',0

MSG_version:     DB 'X-OS version ',0;**
MSG_lastcompil:  DB 'd‚rniŠre compilation le ',0;**
MSG_errcomm:     DB "XS16 :: Cette commande n'‚xiste pas.",0
MSG_noxosexe   DB "XS16 :: Ce fichier n'est pas un executable XS16.",0


MSG_mem:         DB 'taille de la memoire conventionelle libre : ',0
MSG_err_cmm:     DB 'commande inconnue',0
MSG_inf_mem:     DB 'la memoire disponible est : ',0

MSG_cluster:     DB 'Premier cluster = ',0


;************************************************************;

CM_help:
                 DB 10,13,'Aide de X-OS',10,13
                 DB 'ATTENTION, X-OS fait la difference entre les MAJUSCULES et les minuscules ',13,10

                 DB "cls    : Efface l'‚cran",10,13
                 DB "mem    : Affiche la taille de memoire conventionelle libre",10,13
                 DB "reboot : Redemmare l'ordinateur ",10,13
                 DB 'ver    : Affiche le numero de version du kernel ',10,13
                 DB "aide   : Affiche cet ‚cran d'aide",10,13
                 DB 'ls     : Affiche la liste des fichiers et repertoires',10,13
                 DB 'xos32   : Passage en mode 32bits ',10,13
                 DB 'VESADEMO : affiche le logo XOS en mode VESA 600x800x256 (exp‚rimental)',10,13,0
                 DB "int21h : fait appelle … l'interruption 21h detourn‚e par X-OS (experimental)",10,13
                 DB "         cette interruption va etre utilis‚e dans les prochainnes",10,13
                 DB "         version de X-OS pour faire appelle … des routines systŠme",10,13
                 DB "         ces routines deviendront disponnibles pour tout les ",10,13
                 DB "         programmes execut‚s par X-OS si vous executez cette commande",10,13
                 DB 0

found:    db 'found!!',0
notfound: db 'not found !!!',0

;************************************************************;

OS_prompt:       DB '[Floppy]# : ',0
promptlg:        DB 12

pix:             DB '.',0 ;**
slash:           DB '/',0 ;**
space:           DB ' ',0
;************************************************************;
