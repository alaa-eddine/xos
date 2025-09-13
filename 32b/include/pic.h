#ifndef _PIC_H_
#define _PIC_H_


#define ICW1_M 	0x020 /* Master PIC (8259)*/
#define ICW2_M  0x021
#define ICW3_M  0x021
#define ICW4_M  0x021
#define OCW1_M  0x021
#define OCW2_M  0x020
#define OCW3_M  0x020

#define ICW1_S  0x0A0 /* Slave PIC*/
#define ICW2_S  0x0A1
#define ICW3_S  0x0A1
#define ICW4_S  0x0A1
#define OCW1_S  0x0A1
#define OCW2_S  0x0A0
#define OCW3_S  0x0A0

#define PIC1_BASE 0x020 /* Interrupt base for each PIC in HARTIK 3.0 */
#define PIC2_BASE 0x028
#define EOI       0x020 /* End Of Interrupt code for PIC! */

#define bit_on(v,b)     ((v) |= (1 << (b)))
#define bit_off(v,b)    ((v) &= ~(1 << (b)))





void PIC_init(void);
void PIC_end(void);
void irq_mask(u16 irqno);
void irq_unmask(u16 irqno);
#endif
