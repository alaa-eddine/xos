# X-OS Changelog

This document chronicles the development history of X-OS from its first boot in 2002 through the major releases of 2004, and the 2024 modernization.

## Version 0.3.4 (2025.09.07) - Build System Modernization (21 years later xD)
### Bug Fixes
- Fixed floppy driver and ls command

### Major Changes
- **Compiler Compatibility**: Updated build system for modern GCC (9.0+) and NASM (2.14+)
- **Cross-platform Support**: Enhanced Makefiles for various Linux distributions
- **Documentation**: Complete English documentation for all modules
- **GitHub Preparation**: Structured project for open-source publication

### Technical Improvements
- Fixed deprecated assembly syntax for modern NASM
- Resolved linker script compatibility issues
- Updated compiler flags for modern toolchains
- Maintained backward compatibility with original hardware

### Documentation Added
- Comprehensive README with project overview
- Detailed module documentation (Boot, Kernel, Memory, Drivers, Shell)
- Build system guide with troubleshooting
- Historical changelog (this document)

---

## Version 0.3.3 - Final Historical Release (2004)

### Features
- Console display bug fixes
- English character table bug fix (command parameter detection)
- **Dynamic Memory Allocator**: First functional version of xosmalloc
  - Memory allocation and deallocation
  - Free space optimization
  - Ready for integration with other modules

---

## Version 0.3.2-a - Development Snapshot (September 5, 2004)

### Memory Management Improvements
- Enhanced page memory allocation module
- New dynamic allocation functions
- **Large Memory Support**: Can now allocate memory blocks > 4096 bytes
- **free() Function**: Added memory deallocation capability

---

## Version 0.3.2 - Major System Update (May 2, 2004)

### Bootloader Enhancements
- **XoLo Bootloader**: Significant improvements to boot process
- Console display bug fixes
- **Internal Debugger**: Initial development of debugging tools

### Multitasking Foundation
- **Simple Scheduler**: Basic scheduler implementation for multitasking preparation
- 16-bit mode temporarily disabled for XoLo modifications
- **Memory Reorganization**: Complete memory layout restructuring
  - Kernel load addresses
  - GDT (Global Descriptor Table) placement
  - IDT (Interrupt Descriptor Table) placement  
  - TSS (Task State Segment) organization

---

## Version 0.3.1 - Floppy Driver Breakthrough (March 18, 2004)

### The Breakthrough Moment
> **"YOUPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"**
> 
> *"Today around 1:50 AM....... The floppy driver finally works!!!"*
> 
> *":-)) :-))))) :-))))))))))))))))))))))))))))))))"*
> 
> *"uhh... maybe I should go to sleep"*

This version marked a major milestone with the successful implementation of the floppy disk driver after months of development.

---

## Version 0.3.0 - Project Revival (March 7, 2004)

### Major Architecture Overhaul
*"It's getting serious!!"*

- **Makefile Rewrite**: Simplified installation process
- **Proper System Integration**: GDT, IDT, and PIC now properly activated in kernel
- **Flat Memory Model**: Adopted flat segmentation for simplicity
- **Paging System**: Three-level paging with 4KB pages
- **Dynamic Memory**: First malloc() and free() implementation (unstable but functional)
- **Keyboard Improvements**: Bug fixes in keyboard handling
- **AZERTY Support**: French keyboard layout implemented (32-bit mode only)

### Technical Notes
1. GDT and IDT loading functions inspired by BosoKernel project
2. Started development of protected mode floppy driver

---

## Version 0.2.0 - Protected Mode Transition (November 4, 2003)

### Build System Changes
- **Linux-Only Compilation**: No longer compiles under Windows (except with Cygwin)
- GCC now required for 32-bit mode compilation

### System Features
- **FAT12 Bug Fix**: Corrected filesystem handling issues
- **xos32 Command**: Added command to switch to protected mode
- **File Execution Security**: Added executable signature checking
  - .xss system files cannot be executed directly
  - Only files with executable signature can run
- **True 32-bit Shell**: First real 32-bit shell implementation
  - Loads own GDT and IDT
  - Initializes PIC, keyboard, and system timer
  - Three initial commands: 'cls', 'aide', 'ver'
- **32-bit Functions**: Extensive 32-bit function library
  - Display management
  - String comparison
  - Keyboard handling  
  - System clock management
- **Configuration System**: Kernel reads config file specifying default shell

---

## Version 0.1.6 - Project Resumption (October 25, 2003)

*"After 8 months.... I decided to resume the project"*

- **32-bit Shell Rewrite**: Rewrote SHL32.COM in C for easier development

---

## Version 0.1.4 - Protected Mode Achievement (February 20, 2003)

### Historic Milestone
*"Finally wrote a program that enables protected mode transition"*

- **switch32.com**: Program that loads GDT, enters protected mode, and launches shell (SHL32.COM)

---

## **Real Mode Era Ends**
*"Up to this point, X-OS operated only in real mode"*

---

## Version 0.1.3 - Modular Architecture (February 20, 2003)

### Program Loading
- **load Function**: Added ability to load programs/code into memory
- **Program Execution**: X-OS can now launch programs

### System Architecture
Division of Xosker into 3 parts:

1. **xosker.asm** - System kernel
   - Basic operations (initialization, hardware detection)
   - Interrupt installation

2. **shlX.asm** - Command interpreter (shell)
   - Receives keyboard/mouse commands
   - Executes user commands
   - Kernel loads and launches shell
   - Allows multiple specialized shells

3. **Logo System** - Boot graphics
   - BMP file displayed at system startup

### Optimizations
- Boot sector code optimization

---

## Version 0.1.0 - System Interrupts (January 26, 2003)

### Interrupt System
- **Interrupt 21h Hijacking**: X-OS now has custom interrupts
- Future programs will use X-OS-specific interrupts
- **Bug Fixes**: Corrected all known bugs

---

## Version 0.0.6 - Graphics Support (January 16, 2003)

### VESA Graphics
- **VESA Mode**: 640x480 256-color support functions
- **Bug Discovery**: System crash when pressing keys during 'ls' command execution

---

## Development Hiatus
*"A little break and we resume the project ;)"*

---

## Version 0.0.3 - VGA Graphics (September 25, 2002)

### Graphics Implementation
- **VGA Mode**: 320x200 256-color graphics functions
- **Command System**: Resolved function addition problems using pointer structures
- **Bug Fixes**: Various stability improvements
- **New Bug**: System crash on special keys (F1-F12)
- **Additional Commands**: Multiple new commands added

---

## Version 0.0.2 - Basic Functions (July 20, 2002)

### Initial Commands
- **Basic Functions**: mem, reboot, ver, aide commands
- **Bug Detection**: Several bugs discovered
- **Development Issues**: Problems adding new functions

---

## Version 0.0.1 - First Official Release (June 25, 2002)

### Major Achievements
- **Non-contiguous Kernel Loading**: Boot sector can find and load kernel even if fragmented
- **FAT12 Compatibility**: X-OS floppy disks work with other operating systems
- **Filesystem Integration**: Full compatibility with standard FAT12 filesystem

---

## Version 0.0.0 - First Boot (May 1, 2002)

### The Beginning
*"The first boot of X-OS, a test version:"*

- **Initial Boot**: Kernel loading, logo display (text mode only), then... nothing
- **Technical Limitations**: 
  - Kernel had to be located in first data sectors
  - Kernel had to be contiguous
  - Very basic functionality

---

## Development Statistics

### Timeline
- **Start Date**: May 1, 2002 (age 16)
- **Active Development**: 2002-2004 (2 years)
- **Modernization**: 2024 (age 41)

### Major Milestones
1. **First Boot** (May 2002) - Basic kernel loading
2. **FAT12 Support** (June 2002) - Filesystem compatibility  
3. **Graphics Support** (September 2002) - VGA graphics
4. **Protected Mode** (February 2003) - 32-bit transition
5. **Modular Design** (February 2003) - Kernel/shell separation
6. **Memory Management** (March 2004) - Paging and allocation
7. **Floppy Driver** (March 2004) - Storage device support
8. **Modern Build** (December 2024) - Contemporary toolchain

### Technical Evolution
- **Assembly → C**: Transition from pure assembly to C programming
- **Real → Protected Mode**: 16-bit to 32-bit architecture evolution
- **Monolithic → Modular**: From single program to modular system
- **Basic → Advanced**: Simple commands to complex memory management

This changelog represents 4 years of intensive learning and development, documenting the journey from a simple boot sector to a functional operating system with advanced features like memory management, multitasking, and hardware drivers.
