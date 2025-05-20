#include <stdio.h>
#include <stdlib.h>

typedef struct BlockHeader {
    struct BlockHeader *next;
    int size;
} BlockHeader;

char simulatedMemory[10000];
BlockHeader *freeListHead = (BlockHeader *)simulatedMemory;

void mergeAdjacentBlocks(BlockHeader *start) {
    BlockHeader *current = start;
    while (current && current->next) {
        void *currentEnd = (void *)current + sizeof(BlockHeader) + current->size;
        if (currentEnd == (void *)current->next) {
            current->size += current->next->size + sizeof(BlockHeader);
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

void deallocateBlock(void *blockPtr) {
    if (blockPtr == NULL) {
        printf("\nInvalid block");
        return;
    }

    BlockHeader *blockToFree = (BlockHeader *)blockPtr;
    BlockHeader *current = freeListHead, *previous = NULL;

    while (current && current < blockToFree) {
        previous = current;
        current = current->next;
    }

    blockToFree->next = current;
    if (previous == NULL) {
        freeListHead = blockToFree;
    } else {
        previous->next = blockToFree;
    }

    printf("\n\nBefore Merging:");
    BlockHeader *temp = freeListHead;
    while (temp) {
        printf("\nAddress: %p, Size: %d", (void *)temp, temp->size);
        temp = temp->next;
    }

    mergeAdjacentBlocks(freeListHead);

    printf("\n\nAfter Merging:");
    temp = freeListHead;
    while (temp) {
        printf("\nAddress: %p, Size: %d", (void *)temp, temp->size);
        temp = temp->next;
    }

    printf("\nBlock Deallocated");
}

void *allocateBlock(int requestedSize) {
    BlockHeader *current = freeListHead, *previous = NULL;

    while (current && current->size < requestedSize + sizeof(BlockHeader)) {
        previous = current;
        current = current->next;
    }

    if (!current) {
        printf("\nNo memory available");
        return NULL;
    }

    if (current->size >= requestedSize + sizeof(BlockHeader) + 1) {
        BlockHeader *newFreeBlock = (BlockHeader *)((char *)current + sizeof(BlockHeader) + requestedSize);
        newFreeBlock->size = current->size - requestedSize - sizeof(BlockHeader);
        newFreeBlock->next = current->next;

        current->size = requestedSize;
        current->next = NULL;

        if (previous) {
            previous->next = newFreeBlock;
        } else {
            freeListHead = newFreeBlock;
        }
    } else {
        if (previous) {
            previous->next = current->next;
        } else {
            freeListHead = current->next;
        }
        current->next = NULL;
    }

    printf("\n\nMemory Allocated (%d bytes)", requestedSize);
    return (void *)current;
}

int main() {
    freeListHead->next = NULL;
    freeListHead->size = sizeof(simulatedMemory) - sizeof(BlockHeader);

    printf("\nInitial Free List: Start = %p, Size = %d", (void *)freeListHead, freeListHead->size);

    void *block1 = allocateBlock(64);
    void *block2 = allocateBlock(128);
    void *block3 = allocateBlock(256);
    void *block4 = allocateBlock(32);
    void *block5 = allocateBlock(200);

    printf("\n\nRemaining Free Block Size: %d", freeListHead->size);

    printf("\n\nDeallocating Some Blocks...");
    deallocateBlock(block1);
    deallocateBlock(block4);
    deallocateBlock(block2);

    printf("\n\nAllocating Again Into Freed Chunks...");
    void *block6 = allocateBlock(100);

    BlockHeader *temp = freeListHead;
    while (temp != NULL) {
        printf("\nAddress: %p, Size: %d", (void *)temp, temp->size);
        temp = temp->next;
    }

    printf("\n\n");
    return 0;
}
