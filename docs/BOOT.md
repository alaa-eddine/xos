# Boot Process in X-OS

The X-OS boot process is a multi-stage system that transitions from BIOS boot to a fully functional 32-bit protected mode kernel.

## Overview

The boot process consists of three main stages:

1. **Boot Sector** (`boot/boot.asm`) - Initial BIOS boot loader
2. **Setup Program** (`boot/setup.asm`) - Protected mode transition
3. **Kernel Initialization** - 32-bit kernel startup

## Stage 1: Boot Sector (boot.asm)

### Purpose
The boot sector is loaded by the BIOS at address 0x7C00 and is responsible for:
- Loading the setup program and kernel from floppy disk
- Implementing FAT12 filesystem support
- Preparing the system for the next stage

### Key Features

#### FAT12 Compatibility
The boot sector includes a proper BIOS Parameter Block (BPB):

```assembly
_OEM_NAME__________    db 'XoLo3',10,13,0
_bytes_per_sector__    dw 512
_sect_per_cluster__    db 1
_reserved_sectors__    dw 1
_number_of_FATs____    db 2
_number_of_entries_    dw 224
_nb_total_sectors__    dw 2880
_media_descriptor__    db 0xF0
_sectors_per_FAT___    dw 9
_sector_per_track__    dw 18
_number_of_heads___    dw 2
```

This makes X-OS floppy disks readable by other operating systems.

#### File Loading Process
The bootloader can load files from the FAT12 filesystem:

1. **Read FAT**: Load File Allocation Table into memory
2. **Search Directory**: Find files by name in root directory
3. **Follow Clusters**: Use FAT to follow cluster chains
4. **Load Files**: Read file data into specified memory locations

#### Memory Layout During Boot
```
0x0500 - 0x7BFF   Available for file loading
0x7C00 - 0x7DFF   Boot sector code
0x7E00 - 0x7FFF   Stack space
```

### Boot Process Flow

```
BIOS → Boot Sector (0x7C00)
  ↓
Load Setup Program → Memory (0x1000)
  ↓
Load Kernel → Memory (0x1000 + setup_size)
  ↓
Jump to Setup Program
```

## Stage 2: Setup Program (setup.asm)

### Purpose
The setup program handles the critical transition from 16-bit real mode to 32-bit protected mode:

- CPU compatibility checking (386+ required)
- A20 line activation
- GDT (Global Descriptor Table) setup
- Protected mode activation
- Kernel relocation to high memory

### CPU Detection
Verifies 386+ processor presence:

```assembly
C386:
    ; Test for 386 processor by checking flag register behavior
    ; If not 386+, display error and halt
```

### A20 Line Activation
Enables access to memory above 1MB:

```assembly
EnableA20:
    ; Use keyboard controller to enable A20 line
    ; Required for protected mode memory access
```

The A20 line historically was disabled to maintain compatibility with 8086 processors. Enabling it allows access to the full 32-bit address space.

### GDT Setup
Creates a basic Global Descriptor Table:

```assembly
gdt:
    db 0,0,0,0,0,0,0,0        ; Null descriptor
gdt_cs:
    db 0xFF,0xFF,0x0,0x0,0x0,10011011b,11011111b,0x0  ; Code segment
gdt_ds:
    db 0xFF,0xFF,0x0,0x0,0x0,10010011b,11011111b,0x0  ; Data segment
```

This creates:
- **Null Descriptor**: Required by Intel architecture
- **Code Segment**: 4GB flat model, executable
- **Data Segment**: 4GB flat model, read/write

### Protected Mode Transition

```assembly
cli                    ; Disable interrupts
lgdt [gdtptr]         ; Load GDT
mov eax,cr0
or ax,1               ; Set PE bit
mov cr0,eax           ; Enter protected mode
jmp next              ; Clear prefetch queue
```

### Kernel Relocation
Moves the kernel from low memory (0x1000) to high memory (0x100000):

```assembly
mov esi, 0x1000       ; Source: temporary location
mov ebx, 0x100000     ; Destination: 1MB mark
; Copy kernel to high memory
```

This relocation is necessary because:
- The kernel was initially loaded in low memory due to real mode limitations
- Protected mode allows access to memory above 1MB
- Placing the kernel at 1MB follows standard conventions

## Stage 3: Kernel Initialization

### Entry Point
After setup completes, control transfers to the 32-bit kernel at 0x100000.

### Initial Kernel Tasks
1. **Hardware Detection**: Identify installed hardware
2. **Memory Management**: Initialize paging and allocation
3. **Interrupt Setup**: Configure IDT and PIC
4. **Driver Loading**: Initialize device drivers
5. **Shell Launch**: Start the command interpreter

## Memory Map During Boot Process

### Real Mode (Stages 1-2)
```
0x00000 - 0x003FF   Interrupt Vector Table
0x00400 - 0x004FF   BIOS Data Area
0x00500 - 0x07BFF   Available (file loading area)
0x07C00 - 0x07DFF   Boot sector
0x07E00 - 0x09FFF   Available
0x0A000 - 0x0FFFF   Video memory, BIOS ROM
```

### Protected Mode (Stage 3+)
```
0x00000000 - 0x000FFFFF   Low memory (real mode compatible)
0x00100000 - 0x????????   Kernel and available RAM
```

## Error Handling

### Boot Sector Errors
- **File not found**: Display error message and halt
- **Disk read error**: Retry operation, then halt
- **Invalid filesystem**: Boot fails

### Setup Program Errors
- **No 386 processor**: Display "ERR : Processeur 386 non detecté" and halt
- **A20 activation failure**: Continue with limited functionality
- **Memory errors**: May cause system instability

## Configuration

### Boot Configuration
The system reads configuration from `config/bootconf.cfg`:
- Default shell to load
- Boot options
- Hardware settings

### Customization Options
- **Logo display**: Controlled by `DO_SPLASH` compile flag
- **Debug output**: Additional diagnostic information
- **Memory layout**: Adjustable load addresses

## Technical Details

### Sector Layout
- **Boot sector**: 512 bytes, ends with 0x55AA signature
- **Setup program**: Variable size, loaded after boot sector
- **Kernel**: Variable size, initially loaded after setup

### File System Integration
The bootloader integrates seamlessly with FAT12:
- Preserves filesystem structure
- Allows normal file operations from other OS
- Supports standard DOS utilities

### Performance Considerations
- **Minimal size**: Boot sector must fit in 512 bytes
- **Fast loading**: Optimized disk I/O routines
- **Error recovery**: Robust error handling for unreliable floppy disks

## Debugging Features

### Boot Messages
The system displays progress messages during boot:
- File loading status
- Hardware detection results
- Error conditions

### Diagnostic Tools
- Memory dumps available in debug builds
- Hardware detection verbose output
- Boot timing information

## Historical Context

This bootloader was developed when:
- Floppy disks were the primary boot medium
- 1.44MB was a significant storage constraint
- FAT12 was the standard filesystem for removable media
- BIOS was the only firmware interface available

The design reflects these constraints while providing a solid foundation for the operating system.
