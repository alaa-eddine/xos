###########################################################################
# X-OS
#
#
###########################################################################
# X-OS Makefile by Alaa-eddine KADDOURI
# https://xos.eurekaa.org
###########################################################################

#
# "32b/shl32/Makefile"
#

OBJ=xos

CFLAGS=-I../include/ -DDEBUG -DEGGS -DXOS_PAGING -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -Wall -m32 -fno-pie -fno-stack-protector
CC=gcc

AFLAGS= -felf32
ASM=nasm


BINDIR=../../build/obj
INCDIR=../include
SHLDIR=../shl32/
DRVDIR=../drivers/
MMDIR=../mm/
DBGDIR=../xosdbg/
FSDIR=../fs/
LIBDIR=../lib/
VIDDIR=../video/

KERNELOBJS=xosker32.o\
	panic.o\
	scheduler.o\
	gdt.o\
	idt.o\
	int.o\
	interrupt.o\
	mem_ops.o\
	pic.o\
	task.o\
	conio.o\
	tools.o\
	utils.o\
	dbgutils.o\
	$(LIBDIR)printk.o\
	$(SHLDIR)shl32.o\
	$(SHLDIR)shlutils.o\
	$(LIBDIR)mem.o\
	$(FSDIR)fat/fat.o\
	$(DRVDIR)kbd.o\
	$(DRVDIR)timer.o\
	$(DRVDIR)block/floppy.o\
	$(LIBDIR)string.o\
	$(LIBDIR)atoi.o\
	$(MMDIR)mm.o\
	$(MMDIR)xosmalloc.o\
	$(DBGDIR)xosdbg.o\
	$(VIDDIR)vesa.o\





all: $(OBJ)


xos: $(KERNELOBJS) 
	ld -m elf_i386 -T linker.ld $^ -o xosker32.elf
	objcopy -O binary xosker32.elf xosker32.xss
	mv $^ *.xss *.elf $(BINDIR)


loader.o : loader.s
	as --32 -o loader.o loader.s

tools.o : tools.asm
	$(ASM) $(AFLAGS) $^ -o $@

int.o: int.asm
	$(ASM) $(AFLAGS) $^ -o $@


utils.o: ../mm/utils.asm
	$(ASM) $(AFLAGS) $^ -o $@
dbgutils.o: ../xosdbg/dbgutils.asm
	$(ASM) $(AFLAGS) $^ -o $@


%.o: %.c
	$(CC) ${CFLAGS} -c $< -o $@ 


