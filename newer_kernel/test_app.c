#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#define SIZE 100

#define ECE_RESET_W _IO('k', 1)
#define ECE_RESET_R _IO('k', 2)

int main()
{
    char input[SIZE]; 
    char output[SIZE];

    int fd = open("/dev/test_mod02", O_RDWR); //open returns -1 if there is an error. will return an integer representing the file descriptor. first argument is the path to the file we want to open, second argument is our flag for accessmode. (O_RDWR means open for read and write)

    if (fd == -1) //if open function failed
    {
        printf("ERROR\n");
        return -1;
    }

    

    printf("Start write:\n\n");

    


    //need to clear/initialize input
    int ret = write(fd, input, SIZE); //write returns -1 if there is an error. it returns the number of bytes actually written on success. first argument is the file descriptor integer, second argument is our input buffer which holds what we wrote, third argument is the number of bytes we want to write

    if(ret < 0) //if our write function fails
    {
        printf("WRITE ERROR, TRY AGAIN\n");

        ioctl(fd, ECE_RESET_W, NULL);
        

        return -1;
    }

    printf("Reading from device:\n");
    for (int i = 0; i < SIZE;i += 20)//increments in chunks of 10 because we output 10 bytes at a time
    {
        memset(output, 0, sizeof(output)); //first argument is where in the memory we want to write to(just the name of the char array gives us the address), second argument is what we are filling with (in our case 0 to clear old output), third argument is number of bytes to be filled in our case it is the amount of bytes of previous output
        
        ssize_t chk = read(fd, output, 20); //first argument is the file descriptor where we are reading from, second argument is where the read data is being stored, third argument is the maximum number of bytes to attempt reading into the buffer
        //read keeps track of where we are in the file/device with its file offset in the kernel drivers. returns -1 on error, 0 when no bytes are read and on success will return number of bytes read.

        if (chk <= 0)
        {
            printf("READ ERROR, TRY AGAIN\n");

            ioctl(fd, ECE_RESET_R, NULL);

            return -1;
        }

        printf("%s\n", output); //prints the current 20 bits
    }

    
    

    close(fd); //the close function deallocates a file descriptor. the argument is an integer representing the file descriptor
    return 0;
}