This project is a simple implementation of a custom memory allocator in C, managing memory from a fixed-size static array. It mimics a basic heap allocator, supporting allocate and deallocate operations along with free list management and block merging.


1) How It Works:-

- All memory is managed from a single static buffer (`simulatedMemory`).
- Each block of memory has a header (`BlockHeader`) that stores its size and a pointer to the next free block.
- When a block is deallocated, it is reinserted in the sorted free list.
- Adjacent free blocks are merged to reduce fragmentation and keep the free list compact.

2) Functions Overview:_

`void *allocateBlock(int size)`
Finds a suitable chunk from the free list, splits it if necessary, and returns a pointer to the allocated block.

 `void deallocateBlock(void *ptr)`
Inserts the block back into the free list and attempts to merge it with adjacent free blocks.
