#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(fork() == 0)
    { // Child
        void **addr = 0;
        if(ringbuf("magicbuf", 1, addr) == 0)
        {
            printf("Child has opened ringbuf magicbuf\n");
        }

        sleep(8);
        ringbuf("magicbuf", 0, addr);
        printf("Child has closed ringbuf magicbuf\n");
        return 0;
    }
    else
    { // Parent
        void **addr = 0;
        sleep(2);
        if(ringbuf("magicbuf", 1, addr) == 0)
        {
            printf("Parent has opened ringbuf magicbuf\n");
        }
        sleep(2);
        ringbuf("magicbuf", 0, addr);
        printf("Parent has closed ringbuf magicbuf\n");
        wait(0);
        return 0;
    }

//   int fds[2];

//   if(pipe(fds) < 0) {
//     printf("Error creating pipe\n");
//     exit(1);
//   }

//   char send_array[1024]; // 1KB
//   for(int i = 0; i < 1024; i++) {
//     send_array[i] = i;
//   }

//   if(fork() == 0) { 
//     // Child
//     close(fds[0]); // close read
//     for(int i = 0; i < 10 * 1024; i++) { // Send 1KB 10K times
//       write(fds[1], send_array, sizeof(send_array));
//     }
//     close(fds[1]);
//     exit(0);
//   }
//   else { 
//     // Parent
//     close(fds[1]); // close write
//     int start = uptime();
//     char buf[1024];
//     char current_expected = 0;
//     while(1) {
//       int num_read = read(fds[0], buf, sizeof(buf)); // Read 1KB
//       if(!num_read) { // Done reading from pipe
//         break;
//       }
//       for(int i = 0; i < num_read; i++) { // Check data
//         if(buf[i] != current_expected) {
//           printf("Error! Got 0x%x, expected 0x%x from child\n", buf[i], current_expected);
//         }
//         current_expected++;
//       }
//     }

//     int end = uptime();
//     printf("Reading took time: %d\n", end-start);

//     int status;
//     wait(&status);
//     exit(0);
//   }

//   return 0;
}