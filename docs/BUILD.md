# Build System and Installation Guide

This guide covers building, installing, and running X-OS on modern systems after the 2024 modernization updates.

## Prerequisites

### Required Software
- **Linux system** (tested on Ubuntu 20.04+, Debian 11+, Fedora 35+)
- **NASM** (Netwide Assembler) version 2.14+
- **GCC** (GNU Compiler Collection) version 9.0+
- **GNU Make** version 4.0+
- **mkdosfs** (from dosfstools package)

### Optional Software
- **Bochs** emulator for testing (recommended)
- **QEMU** as alternative emulator
- **Physical floppy drive** for real hardware testing

### Installing Prerequisites

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install nasm gcc make dosfstools bochs bochs-x
```

#### Fedora/CentOS/RHEL
```bash
sudo dnf install nasm gcc make dosfstools bochs
```

#### Arch Linux
```bash
sudo pacman -S nasm gcc make dosfstools bochs
```

## Build System Overview

### Makefile Structure
The build system uses a hierarchical Makefile structure:

```
Makefile                 # Main build control
├── boot/Makefile       # Bootloader compilation
├── 16b/kernel/Makefile # 16-bit kernel
├── 16b/shl16/Makefile  # 16-bit shell
└── 32b/kernel32/Makefile # 32-bit kernel and components
```

### Build Targets

#### Primary Targets
```bash
make help           # Display available targets
make compil         # Compile entire system
make install        # Compile and install to floppy
make image          # Create floppy disk image
make clean          # Remove build artifacts
```

#### Specialized Targets
```bash
make boot           # Compile and install bootloader only
make kernel         # Compile 16-bit kernel
make kernel32       # Compile 32-bit kernel
make shell16        # Compile 16-bit shell
make runimage       # Run disk image in Bochs
```

## Compilation Process

### Build Order
The system builds components in this order:

1. **Date/time generation**: Updates compilation timestamps
2. **Bootloader**: Creates boot sector (boot/boot.asm)
3. **16-bit kernel**: Assembles real mode kernel
4. **16-bit shell**: Assembles real mode shell
5. **32-bit kernel**: Compiles protected mode kernel and all C components

### Compilation Options

#### Debug Build
Enable debugging features:
```bash
# Edit Makefile and uncomment:
COMPIL_OPT= -DDEBUG -DEGGS

# Then build:
make compil
```

Debug features include:
- Memory allocation debugging
- Hardware status monitoring
- Extended diagnostic commands
- Verbose error reporting

#### Standard Build
```bash
make compil
```

### Build Configuration

#### Compiler Flags
The build system uses these flags:
- **-m32**: Generate 32-bit code
- **-nostdlib**: Don't link standard library
- **-nostartfiles**: Don't use standard startup files
- **-fno-builtin**: Disable built-in functions
- **-fno-stack-protector**: Disable stack protection

#### Linker Configuration
Custom linker script (`32b/kernel32/linker.ld`) defines:
- **Entry point**: Kernel start address
- **Section layout**: Code and data organization
- **Memory layout**: Virtual address mapping

## Installation Methods

### Method 1: Floppy Disk Image (Recommended)

#### Create Disk Image
```bash
make image
```

This creates `xos.img` (1.44MB floppy image) containing:
- Boot sector with X-OS bootloader
- 16-bit and 32-bit kernels
- Shell programs
- Configuration files
- Boot logo

#### Test in Emulator
```bash
make runimage
```

Launches Bochs emulator with the disk image.

### Method 2: Physical Floppy Disk

#### Prerequisites
- Physical floppy drive (/dev/fd0)
- Blank 1.44MB floppy disk
- Proper permissions for floppy access

#### Installation Steps
```bash
# Insert blank floppy disk
make install
```

This process:
1. Compiles the entire system
2. Mounts the floppy disk
3. Installs boot sector
4. Copies system files
5. Unmounts the disk

#### Manual Installation
```bash
# Compile first
make compil

# Mount floppy
sudo mount /dev/fd0 /mnt/floppy

# Install boot sector
sudo dd if=obj/boot of=/dev/fd0 bs=512 count=1

# Copy system files
sudo cp obj/*.xss /mnt/floppy/
sudo cp config/* /mnt/floppy/
sudo cp logo/xoslogo.bmp /mnt/floppy/

# Unmount
sudo umount /mnt/floppy
```

## Build Artifacts

### Generated Files
After compilation, the `obj/` directory contains:

```
obj/
├── boot                # Boot sector (512 bytes)
├── xosker16.xss       # 16-bit kernel
├── xosker32.xss       # 32-bit kernel  
├── shl16.xss          # 16-bit shell
├── shl32.xss          # 32-bit shell
└── sw32.xss           # Mode switcher
```

### File Extensions
- **.xss**: X-OS System files (executable)
- **.com**: DOS-compatible executables
- **.bmp**: Bitmap graphics (boot logo)
- **.cfg**: Configuration files

## Testing and Debugging

### Emulator Testing

#### Bochs Configuration
The included `bochsrc.txt` configures:
- CPU: Pentium-class processor
- Memory: 32MB RAM
- Floppy: xos.img as drive A:
- Display: VGA with VESA support

#### QEMU Alternative
```bash
qemu-system-i386 -fda xos.img -boot a
```

### Hardware Testing

#### Requirements
- 386+ processor (486 recommended)
- 4MB+ RAM (8MB recommended)
- VGA-compatible display
- PS/2 keyboard
- 3.5" floppy drive

#### Boot Process
1. Insert X-OS floppy disk
2. Boot from floppy (set in BIOS)
3. System displays boot messages
4. Loads into 32-bit shell

### Troubleshooting

#### Build Errors

**NASM not found**
```bash
# Install NASM
sudo apt install nasm
```

**GCC errors**
```bash
# Install 32-bit development libraries
sudo apt install gcc-multilib
```

**Permission denied on floppy**
```bash
# Add user to floppy group
sudo usermod -a -G floppy $USER
# Logout and login again
```

#### Runtime Issues

**Boot hangs**
- Check CPU compatibility (386+ required)
- Verify floppy disk integrity
- Try different emulator settings

**Keyboard not working**
- Ensure PS/2 keyboard (not USB)
- Check keyboard controller initialization
- Try different keyboard layout (setkeymap en/fr)

**Display problems**
- Verify VGA compatibility
- Check video mode initialization
- Try text mode only (disable graphics)

## Advanced Build Options

### Cross-compilation
For building on non-x86 hosts:
```bash
# Install cross-compiler
sudo apt install gcc-i686-linux-gnu

# Modify Makefile to use cross-compiler
CC=i686-linux-gnu-gcc
```

### Custom Configuration

#### Memory Layout
Edit `32b/include/mem.h` to adjust:
- Kernel load address
- Page directory location
- Memory map boundaries

#### Hardware Support
Modify driver compilation in `32b/kernel32/Makefile`:
- Enable/disable specific drivers
- Adjust hardware detection
- Configure device parameters

### Packaging

#### Source Distribution
```bash
make package
```
Creates `targz/xossrc_DDMMYYYY.tar.gz` with complete source code.

#### Binary Distribution
```bash
make winpackage
```
Creates `targz/xosbin_DDMMYYYY.tar.gz` with compiled binaries.

## Modernization Changes (2024)

### Compiler Compatibility
- Updated for GCC 9.0+ compatibility
- Fixed deprecated assembly syntax
- Resolved linker script issues
- Added modern build flags

### Build System Improvements
- Parallel compilation support
- Better dependency tracking
- Improved error reporting
- Cross-platform compatibility

### Legacy Support
- Maintained original functionality
- Preserved historical code structure
- Compatible with original hardware
- Backward-compatible file formats

## Performance Optimization

### Build Speed
- Use parallel make: `make -j$(nproc) compil`
- Enable ccache for faster rebuilds
- Use SSD storage for build directory

### Runtime Performance
- Compile with optimizations: `-O2`
- Enable specific CPU features: `-march=i486`
- Optimize for size: `-Os` for embedded use

This build system provides a robust foundation for compiling and deploying X-OS while maintaining compatibility with both modern development environments and vintage hardware.
