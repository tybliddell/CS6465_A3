#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    const char name[16] = "test";
    int open = 1;
    void** addr = 0;
    ringbuf(name, open, addr);

    return 0;
}