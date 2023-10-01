#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/ringbuf.h"
#include "kernel/riscv.h"

int g_seed = 0;

void fast_srand(int seed) {
    g_seed = seed;
}

int fast_rand(void) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

int main(int argc, char *argv[])
{
    if (fork() == 0)
    { // Child
        int fd = create_ringbuf("magic!");
        sleep(10);

        // hello world test
        while (1)
        {
            int can_read;
            char *addr;
            ringbuf_start_read(fd, &addr, &can_read);
            // printf("Current can read: %d, current addr: %p\n", can_read, addr);

            if (can_read)
            {
                char data[can_read];
                memmove(data, addr, can_read);
                printf("Got our %d bytes of data: %s\n", can_read, data);
                ringbuf_finish_read(fd, can_read);
            }
            // printf("Still nothing to read!\n");
        }

        // test 2
        //  while(1) {

        // }

        free_ringbuf(fd);
    }
    else
    { // Parent
        sleep(5);
        int fd = create_ringbuf("magic!");
        int can_write;
        char *addr;
        ringbuf_start_write(fd, &addr, &can_write);
        memmove(addr, "this is number one loser peepee haha", strlen("this is number one loser peepee haha"));
        ringbuf_finish_write(fd, strlen("this is number one loser peepee haha"));
        sleep(5);

        ringbuf_start_write(fd, &addr, &can_write);
        memmove(addr, "mckay has itty bitty penis", strlen("mckay has itty bitty penis"));
        ringbuf_finish_write(fd, strlen("mckay has itty bitty penis"));

        sleep(5);
        ringbuf_start_write(fd, &addr, &can_write);
        printf("can write %d bytes\n", can_write);
        for(int i = 0; i < 3577; i++) {
            char val = 'a';
            int ran = fast_rand() % 26;
            val += ran;
            // printf("val: %c, ran: %d\n", val, ran);
            // printf("moving\n");
            memmove(addr + i, (void*)&val, 1);
        }
        printf("piss\n");
        ringbuf_finish_write(fd, 3577);
        

        free_ringbuf(fd);
        wait(0);
    }

    return 0;
}