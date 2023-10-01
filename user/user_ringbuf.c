#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    const char name[16] = "test";
    int open = 1;
    void* addr;
    printf("%d", ringbuf(name, open, &addr));

    // printf("returned from ringbuf syscall\n");
    // ringbuf("two", open, addr);    

    // printf("exiting user program\n");

    // ringbuf("two", 0, addr);
    // printf("about to unmap test now\n");
    ringbuf("test", 1, addr);
    ringbuf("test", 0, addr);

    // TODO close the ringbufs before exiting
    return 0;
}