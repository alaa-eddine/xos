# Memory Management in X-OS

X-OS implements a memory management system with both paging and dynamic allocation.

## Overview

The memory management subsystem consists of:
- **Page frame allocation**: Bitmap-based physical memory tracking
- **Paging system**: 3-level page tables with 4KB pages
- **Dynamic allocation**: Custom malloc/free implementation (xosmalloc)
- **Memory protection**: Separate kernel and user address spaces

## Physical Memory Layout

```
0x00000000 - 0x000003FF  Real Mode IVT (protected)
0x00000400 - 0x000007FF  BIOS Data Area
0x00000800 - 0x00007FFF  Available
0x00008000 - 0x0008FFFF  System structures (GDT, IDT, TSS)
0x00090000 - 0x0009FFFF  Available
0x000A0000 - 0x000FFFFF  Video RAM + BIOS ROM (protected)
0x00100000 - 0x????????  Kernel + Available RAM
```

## Page Frame Allocator

Located in `32b/mm/mm.c`, this module manages physical memory pages.

### Key Functions

```c
int init_mem(void)              // Initialize memory bitmap
int setused(unsigned int pos)   // Mark page as used
int setfree(unsigned int pos)   // Mark page as free
unsigned long alloc_npages(unsigned int npages)  // Allocate consecutive pages
```

### Implementation Details

- Uses a bitmap where each bit represents a 4KB page
- Supports allocation of consecutive page ranges
- Automatically protects system memory areas
- Tracks free page count for memory statistics

## Paging System

### Page Directory Structure
- Located at physical address `_PD_ADDR_` (0x80000)
- Contains 1024 entries, each covering 4MB of virtual address space
- First entry points to the initial page table

### Page Tables
- First page table at `_PT_ADDR_` (0x81000)
- Maps first 4MB using identity mapping
- Each entry covers 4KB of memory
- Present bit, read/write bit, and user/supervisor bit supported

### Initialization Process
```c
int set_paging(void)
{
    // Set up page directory and first page table
    // Enable identity mapping for first 4MB
    // Activate paging by setting CR0.PG bit
}
```

## Dynamic Memory Allocator (xosmalloc)

A custom implementation of malloc/free using linked lists.

### Design Philosophy
From the original comments:
> "I wrote this memory allocation manager to be as simple as possible. xosmalloc is certainly not one of the most performant allocators due to linked list traversals, but I hope it's the simplest to understand."

### Architecture

```
┌─────────────────┐     ┌─────────────────┐
│   Free Blocks   │     │   Used Blocks   │
│   Linked List   │     │   Linked List   │
└─────────────────┘     └─────────────────┘
         │                       │
         └───────┬───────────────┘
                 │
        ┌─────────────────┐
        │  Cache Pages    │
        │  (Metadata)     │
        └─────────────────┘
```

### Key Data Structures

```c
typedef struct _cache_element {
    unsigned long base;           // Block base address
    unsigned long size;           // Block size
    struct _cache_element *next;  // Next element in list
} cache_element;
```

### Core Functions

```c
void init_cache(void)                    // Initialize allocator
void* alloc(unsigned long size)          // Allocate memory block
int free(unsigned long base)             // Free memory block
```

### Allocation Strategy

1. **Search free list**: Find first block >= requested size
2. **Split if necessary**: If block is larger, split and return remainder to free list
3. **Coalesce on free**: Merge adjacent free blocks to reduce fragmentation
4. **Expand heap**: Allocate new pages when no suitable blocks found

## Memory Protection

### Kernel Space (Ring 0)
- Addresses 0x00000000 - 0x00400000
- Full access to all memory
- Can execute privileged instructions

### User Space (Ring 3)  
- Addresses 0x00400000+
- Limited access via page table permissions
- Cannot access kernel memory directly

## Debugging Features

The system includes several debugging utilities:

```c
int print_bit_mem(void)      // Display memory bitmap
int print_mapped_mem(void)   // Show memory regions
void dump_mem(char *addr)    // Hex dump memory contents
```

## Performance Characteristics

### Page Allocation
- **Time Complexity**: O(n) for finding consecutive pages
- **Space Complexity**: O(1) - fixed bitmap size
- **Fragmentation**: External fragmentation possible

### Dynamic Allocation
- **Time Complexity**: O(n) for allocation/deallocation
- **Space Complexity**: O(n) - metadata overhead
- **Fragmentation**: Internal fragmentation minimized by splitting

## Configuration

Key constants defined in `32b/mm/mm.h`:

```c
#define _PAGE_SIZE      4096        // Page size in bytes
#define MAPSIZE         128         // Bitmap size (4MB coverage)
#define INITIAL_PAGES_NB 1024      // Initial free page count
#define _PD_ADDR_       0x80000     // Page directory address
#define _PT_ADDR_       0x81000     // Page table address
```

## Future Improvements

Potential enhancements identified:
- Buddy system allocator for better performance
- Copy-on-write page sharing
- Swapping support for virtual memory
- NUMA-aware allocation strategies
