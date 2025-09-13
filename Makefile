###########################################################################
# X-OS
#
#
###########################################################################
# X-OS Makefile by Alaa-eddine KADDOURI
# https://xos.eurekaa.org
###########################################################################

#options de la compilation
#
# -DDEBUG : active le debugage
# -DXOS_PAGING : active la pagination
# -DEGGS : active des procedures et des fonctions qui ne servent à rien (donc très utiles ;-)

#COMPIL_OPT= -DDEBUG -DEGGS



OBJ=help
BINDIR=build/obj
IMAGEBINDIR=build/image
ARCHDIR=targz

XOSDIR=xosdev

FLOPPY=/mnt/floppy
FLOPPYDEV=/dev/fd0
MKDOSFS=mkdosfs


DD=dd
all: $(OBJ)

help:
	@echo  '**************************************'	
	@echo  '*     X-OS Installation Script       *'
	@echo  '**************************************'
	@echo
	@echo  'make install    : Build and install X-OS on floppy disk'
	@echo  'make build      : Build X-OS without installing on floppy disk'
	@echo  'make boot       : Build boot sector only and install on floppy disk'
	@echo  'make clean      : Remove temporary files'
	@echo  'make image      : Create X-OS floppy disk image, requires mkdosfs'
	@echo  "                  the created file is called xos.img"
	@echo  "make runimage   : run the image created by make image with qemu"
	@echo 

install: build
	@echo ===================[INSTALLATION]===================
	@echo Mounting drive...
	@if !(grep 'floppy' /etc/mtab > /dev/null) then mount /mnt/floppy ; fi
	@echo
	@echo Installing boot sector ...	
	@cat $(BINDIR)/boot > ${FLOPPYDEV}
	@echo [OK] 
	@echo Copying system files ...
	cp $(BINDIR)/xosker32.xss ${FLOPPY}
	cp $(BINDIR)/xosker16.xss ${FLOPPY}
	cp $(BINDIR)/*.xss ${FLOPPY}
	cp $(BINDIR)/*.com $(BINDIR)/*.xss ${FLOPPY}
	cp config/* ${FLOPPY}
	cp logo/xoslogo.bmp ${FLOPPY}

	@echo
	@echo Unmounting drive...
	@if (grep 'floppy' /etc/mtab > /dev/null) then umount /mnt/floppy ; fi
	@echo
	@echo ===============[INSTALLATION COMPLETED]=============

package: clean
	@echo Generating xossrc package...
	cd .. && tar -czf  $(XOSDIR)/$(ARCHDIR)/xossrc_$(shell date +%d)$(shell date +%m)$(shell date +%Y).tar.gz $(XOSDIR) --exclude=$(XOSDIR)/targz/* && cd $(XOSDIR)
	@echo [OK]
	@echo Package is located in $(ARCHDIR) directory


boot: bootsect
	cat $(BINDIR)/boot > ${FLOPPYDEV}

build: buildmsg builddate bootsect kernel shell16 kernel32 buildendmsg

buildmsg:
	@echo ----------------------------------------------------
	@echo --  Building X-OS                                 
	@echo ----------------------------------------------------
	@echo -- By Alaa-eddine KADDOURI                        
	@echo -- https://xos.eurekaa.org                    
	@echo ----------------------------------------------------
	@echo
	@echo ==========[Starting build process]=================
	@echo

buildendmsg:
	@echo ==============[Build process completed]=============
	@echo
	@echo

kernel:
	@echo Building 16-bit kernel...
	make -s -C 16b/kernel > /dev/null 
	@echo [OK]
	@echo

shell16:
	@echo Building 16-bit shell ...
	make -s -C 16b/shl16 > /dev/null
	@echo [OK]
	@echo

bootsect:
	@echo Building boot sector ...
	make -s -C boot > /dev/null
	@echo [OK]
	@echo

kernel32:
	@echo Building 32-bit kernel ...
#	export COMPIL_OPT='$(COMPIL_OPT)'
	make -s -C 32b/kernel32 > /dev/null
	@echo [OK]
	@echo

builddate:
	@echo Updating build date ...
	@echo compilday DB $(shell date +%d) > ./16b/include/compdate.inc
	@echo compilmonth DB $(shell date +%m) >> ./16b/include/compdate.inc
	@echo compilyear DW $(shell date +%Y) >> ./16b/include/compdate.inc 

	@echo \#define compilday \"$(shell date +%d)\" > ./32b/include/kerinfo.h
	@echo \#define compilmonth \"$(shell date +%m)\" >> ./32b/include/kerinfo.h
	@echo \#define compilyear \"$(shell date +%Y)\" >> ./32b/include/kerinfo.h

	@echo Completed 
	@echo


image:build
	@echo "===============[CREATING IMAGE]====================="
	@echo "Creating floppy disk image"
	@rm -f $(IMAGEBINDIR)/xos.img
	@$(MKDOSFS) -C -F 12 -S 512 -s 1 -R 1 -f 2 -r 224 -v $(IMAGEBINDIR)/xos.img 1440
	@echo 'Copying boot sector'
	@$(DD) if="$(BINDIR)"/boot of=$(IMAGEBINDIR)/xos.img conv=notrunc
	@echo '[OK]'
	@echo Copying system files ...
	mcopy -i $(IMAGEBINDIR)/xos.img $(BINDIR)/*.com ::
	mcopy -i $(IMAGEBINDIR)/xos.img $(BINDIR)/*.xss ::
	mcopy -i $(IMAGEBINDIR)/xos.img config/* ::
	mcopy -i $(IMAGEBINDIR)/xos.img logo/xoslogo.bmp ::
	@echo
	@echo =================[IMAGE CREATION COMPLETED]=========
runimage:
	qemu-system-i386 -fda $(IMAGEBINDIR)/xos.img
clean:
	@echo =============================================
	@echo Cleaning directories $(BINDIR) and $(ARCHDIR)
	@rm -f $(BINDIR)/*.o 
	@rm -f $(BINDIR)/*.xss
	@rm -f $(BINDIR)/*.com
	@rm -f $(BINDIR)/*.img
	@rm -f $(BINDIR)/*.elf
	@rm -f $(BINDIR)/boot	
	@echo [ok]
	@echo
