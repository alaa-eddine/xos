/*
@Auteur : Alaa-eddine KADDOURI
@Projet : X Operating System

@Description :



*/

#include <types.h>
#include <sys.h>
#include <pic.h>

/* PIC interrupt mask */
u8 ll_PIC_master_mask = 0xFE;
u8 ll_PIC_slave_mask = 0xFE;




void PIC_init(void)
{
    outbp(ICW1_M,0x11);
    outbp(ICW2_M,PIC1_BASE);
    outbp(ICW3_M,0x04);
    outbp(ICW4_M,0x01);
    outbp(OCW1_M,0xFF);

    outbp(ICW1_S,0x11);
    outbp(ICW2_S,PIC2_BASE);
    outbp(ICW3_S,0x02);
    outbp(ICW4_S,0x01);
    outbp(OCW1_S,0xFF);
}

void PIC_end(void)
{
    outbp(ICW1_M,0x11);
    outbp(ICW2_M,0x08);
    outbp(ICW3_M,0x04);
    outbp(ICW4_M,0x01);
    outbp(OCW1_M,0xFF);

    outbp(ICW1_S,0x11);
    outbp(ICW2_S,0x70);
    outbp(ICW3_S,0x02);
    outbp(ICW4_S,0x01);
    outbp(OCW1_S,0xFF);
}

void irq_mask(u16 irqno)
{
    /* Le Timer ne peut pas etre masqué */
    if (irqno == 0) return;
    /* L'interruption est dans le PIC maitre ?*/
    if (irqno < 8) {
	bit_on(ll_PIC_master_mask,irqno);
	outbp(0x21,ll_PIC_master_mask);
    } else if (irqno < 16) {
	/* L'interruption est dans le PIC esclave ?*/
	bit_on(ll_PIC_slave_mask,irqno-8);
	outbp(0xA1,ll_PIC_slave_mask);
	/* Si toutes les interruptions du PIC esclave sont désactivées   */
	/* On désactive ce dernier du PIC maitre via IRQ 2*/
	if (ll_PIC_slave_mask == 0xFF && !(ll_PIC_master_mask & 0x04)) {
	    bit_on(ll_PIC_master_mask,2);
	    outbp(0x21,ll_PIC_master_mask);
	}
    }
}

void irq_unmask(u16 irqno)
{
    /* Le Timer ne peut pas etre demasqué */
    if (irqno == 0) return;
    /* PIC maitre */
    if (irqno < 8) {
	bit_off(ll_PIC_master_mask,irqno);
	outbp(0x21,ll_PIC_master_mask);
    } else if (irqno < 16) {
	/* PIC esclave */
	bit_off(ll_PIC_slave_mask,irqno-8);
	outbp(0xA1,ll_PIC_slave_mask);
	/* Activation du PIC esclave si necessaire */
	if (ll_PIC_master_mask & 0x04) {
	    bit_off(ll_PIC_master_mask,2);
	    outbp(0x21,ll_PIC_master_mask);
	}
    }
}
