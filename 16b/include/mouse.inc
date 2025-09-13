;***********************************************************
;*********** GESTION DE LA SOURIS **************************
;***********************************************************

P_get_mouseinfo:
;sortie bx= nombre de bouttons ou 0
    push ax
    mov ah, 0
    int 33h
    cmp ax, 0
    jne .end
    mov bx, ax
.end:
    ret

P_show_mouse:
    mov ah, 01h
    int 33h
    ret

P_hide_mouse:
    mov ah, 02h
    int 33h
    ret