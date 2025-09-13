# Third Party Licenses

This project includes code from third-party sources under different licenses:

## GNU C Library Components

The following files are from the GNU C Library and are licensed under the GNU Lesser General Public License (LGPL) v2.1:

- `32b/lib/ctype.h` - Character handling functions
- `32b/lib/limits.h` - Integer type size limits

## Linux Kernel Components

The following files contain code derived from the Linux kernel and are subject to GPL v2 license:

- `32b/include/floppy.h` - Hardware register definitions from Linux kernel's `include/linux/fdreg.h`

## KOS Project Components

The following files contain code derived from the KOS operating system project (http://kos.enix.org):

- `32b/lib/printk.c` - Printf implementation adapted from KOS project (license unknown, likely GPL/BSD)

### LGPL v2.1 License

These files remain under their original LGPL v2.1 license. The LGPL allows linking with MIT-licensed code.

**Copyright:** Copyright (C) 1991-2002 Free Software Foundation, Inc.

**License:** GNU Lesser General Public License v2.1  
**Full License Text:** https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html

### GPL v2 License

The Linux kernel-derived code is licensed under GPL v2.

**Copyright:** Copyright (C) 1991-2003 Linus Torvalds and contributors

**License:** GNU General Public License v2  
**Full License Text:** https://www.gnu.org/licenses/old-licenses/gpl-2.0.html

### KOS Project License

The KOS project components have unknown original license (likely GPL or BSD from that era).

**Copyright:** Unknown (KOS project contributors)
**License:** Unknown (assumed GPL/BSD compatible)
**Source:** http://kos.enix.org

### License Compatibility

- The **LGPL v2.1** is compatible with the MIT license used for the rest of this project
- The **GPL v2** components require that any derivative work be GPL-compatible  
- The **KOS components** have unknown license but are assumed to be open source compatible
- Since this is an educational/hobby project being distributed as source code, license compliance is maintained through proper attribution

## Project License

All other files in this project are licensed under the MIT License. See the main [LICENSE](LICENSE) file for details.
