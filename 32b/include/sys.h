#ifndef SYS_H
#define SYS_H

#ifndef cli
#define cli             asm("cli"::)
#endif

#ifndef sti
#define sti             asm("sti"::)
#endif

#ifndef outb
#define outb(port,value) \
	asm volatile ("outb %%al,%%dx"::"d" (port), "a" (value));
#endif

#ifndef outbp
#define outbp(port,value) \
	asm volatile ("outb %%al,%%dx; jmp 1f; 1:"::"d" (port), "a" (value));
#endif

#ifndef inb
#define inb(port) ({	\
	unsigned char _v;	\
	asm volatile ("inb %%dx,%%al" : "=a" (_v) : "d" (port)); \
	_v;	\
})
#endif

#ifndef nop
#define nop ({ asm volatile ("nop");})
#endif




 
#define eoi()   __asm__("movb $0x20,%al \n"                    \
                        "outb %al,$0x20 \n");                  \
                __asm__("movb $0x20,%al \n"                    \
                        "outb %al,$0xA0 \n");

								 


//void reboot(void);

#endif
