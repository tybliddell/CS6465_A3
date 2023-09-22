#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"

#define MAX_RINGBUFS 10
#define RINGBUF_SIZE 16

struct ringbuf
{
    int refcount; // 0 for empty slot
    char name[16];
    void *buf[RINGBUF_SIZE]; // physical addresses of pages that comprise the ring buffer
    void *book;
};

struct spinlock ringbuf_lock;
struct ringbuf ringbufs[MAX_RINGBUFS];

int create_ringbuf(const char* name, int open) 
{
    // TODO: check if name is valid first
    // go through ringbufs, check if name exists
    for(int i = 0; i < MAX_RINGBUFS; i++) {
        if(ringbufs[i].refcount == 0)
        {
            // Empty slot
            printf("this is an empty slot\n");
        }

        printf("existing name: %s, name: %s\n", ringbufs[i].name, name);
        // ringbuf exists
        if(!strncmp(ringbufs[i].name, name, sizeof(name)/sizeof(char))) {
            ringbufs[i].refcount++;
            printf("ringbuf exists\n");
        }
        // ringbuf doesn't exist, create it
        else {
            printf("ringbuf does not exist\n");
            ringbufs[i].refcount++;
            strncpy(ringbufs[i].name, name, sizeof(name)/sizeof(char));

            // map pages of ring buffer + 1 bookkeeping page
            
            
            break;
        }
    }

    return 0;
}

// Create a new process, copying the parent.
// Sets up child kernel stack to return as if from fork() system call.
void
ringbuf(const char *name, int open, void **addr)
{
  printf("calling inside from next\n");
  printf("name: %s, open: %d, addr: %x\n", name, open, addr);

  create_ringbuf(name, open);
  
  // create ring buffer if it doesnt exist (increasing ref count, etc.)

  // allocate ringbug_size pages of physical memory + bookkeeping page

  // set addr to point to address of ring buffer
}