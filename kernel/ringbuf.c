#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"
#include "memlayout.h"

#define MAX_RINGBUFS 10
#define RINGBUF_SIZE 16
#define SINGLE_RINGBUF_SIZE (PGSIZE * (RINGBUF_SIZE * 2 + 1))

struct ringbuf
{
    int refcount; // 0 for empty slot
    char name[16];
    void *buf[RINGBUF_SIZE]; // physical addresses of pages that comprise the ring buffer
    void *book;
};

struct book
{
    uint64 read_done, write_done;
};

struct spinlock ringbuf_lock;
struct ringbuf ringbufs[MAX_RINGBUFS];

// Return the index of the ringbuf with a matching name if it exists, else -1
int ringbuf_exists(const char *name)
{
    for (int i = 0; i < MAX_RINGBUFS; i++)
        if (!strncmp(ringbufs[i].name, name, sizeof(name) / sizeof(char)))
            return i;

    return -1;
}

// Return index for an open ringbuf spot, else -1 if there are none available
int find_open_ringbuf()
{
    for (int i = 0; i < MAX_RINGBUFS; i++)
        if (ringbufs[i].refcount == 0)
            return i;
    return -1;
}

int alloc_ringbuf(int index)
{
    for (int i = 0; i < RINGBUF_SIZE + 1; i++)
    {
        void *temp = kalloc();
        printf("Allocated index %d: %p\n", i, (uint64)temp);

        if (!temp)
        {
            printf("Error allocing on index %d\n", i);
            for (int j = 0; j < i; j++)
                kfree(ringbufs[index].buf[j]);
            return -1;
        }
        if (i == RINGBUF_SIZE)
            ringbufs[index].book = temp; // bookkeeping page
        else
            ringbufs[index].buf[i] = temp; // ringbuf page
    }
    return 0;
}

int map_ringbuf(int index)
{
    uint64 start_address = PHYSTOP - ((index + 1) * SINGLE_RINGBUF_SIZE);
    for (int i = 0; i < RINGBUF_SIZE * 2 + 1; i++)
    {
        uint64 current_address = start_address + i * PGSIZE;
        struct proc *user_proc = myproc();
        int map_success;
        if (i == 0)
        {
            // map book
            map_success = mappages(user_proc->pagetable, current_address, PGSIZE, (uint64)ringbufs[index].book, PTE_R | PTE_W | PTE_U);
            printf("Mapping book page to address %p\n", (i - 1) % RINGBUF_SIZE, current_address);
        }
        else
        {
            // map the buf pages
            map_success = mappages(user_proc->pagetable, current_address, PGSIZE, (uint64)ringbufs[index].buf[(i - 1) % RINGBUF_SIZE], PTE_R | PTE_W | PTE_U);
            printf("Mapping page %d, address %p to address %p\n", (i - 1) % RINGBUF_SIZE, ringbufs[index].buf[(i - 1) % RINGBUF_SIZE], current_address);
        }
        if (map_success == -1)
        {
            // TODO: handle error Mckay
            printf("Error mapping, need to unmap mapped pages and free all pages\n");
            return -1;
        }
    }
    return 0;
}

int create_ringbuf(const char *name)
{
    int index = ringbuf_exists(name); // if the ringbuf exists, it will return the index of the existing ringbuf
    if (index != -1)
    {
        ringbufs[index].refcount++;
        printf("ringbuf %s already exists\n", name);
        return 0;
    }

    printf("ringbuf %s does not exist, creating\n", name);
    // does not exist, need an open spot
    int open_spot = find_open_ringbuf();
    if (open_spot == -1)
    {
        printf("Error! No open spots\n");
        return -1;
    }

    int alloc_success = alloc_ringbuf(open_spot);
    if (alloc_success == -1)
    {
        printf("Error! Unsuccessful alloc\n");
        return -1;
    }

    // ringbufs is now allocated, map them
    int map_sucess = map_ringbuf(open_spot);
    if (map_sucess == -1)
    {
        printf("Error! Unsuccessful mapping\n");
        return -1;
    }

    // TODO create an empty book
    ringbufs[open_spot].refcount++;
    strncpy(ringbufs[open_spot].name, name, sizeof(name) / sizeof(char));

    // TODO set addr to correct address
    return 0;
}

int close_ringbuf(const char *name)
{
    int index = ringbuf_exists(name); // if the ringbuf exists, it will return the index of the existing ringbuf
    if (index == -1)
    {
        printf("Error! Cannot close ringbuf %s since it doesn't exist\n", name);
        return -1;
    }

    return 0;
}

int ringbuf(const char *name, int open, void **addr)
{
    if (!name)
    { // Invalid name
        printf("Error! Invalid name\n");
        return -1;
    }

    // TODO acquire lock and unacquire
    if (open)
    {
        create_ringbuf(name);
    }
    else
    {
        close_ringbuf(name);
    }

    return 0;
}