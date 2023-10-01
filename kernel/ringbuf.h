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


