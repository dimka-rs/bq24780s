#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdint.h>
#include <sys/poll.h>

#define READ_TOUT_MS 100  //timeout for read poll


/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */
int8_t fd;

int8_t open_port(uint8_t * SerialPort)
{
    fd = open(SerialPort, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd == -1) {
        printf("open_port: Unable to open %s\r\n", (char *) SerialPort);
        perror("Error: ");
        return -1;
    } 
    //fcntl(fd, F_SETFL, 0); //This resets flags including O_NONBLOCK

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    options.c_cflag &= ~CSIZE; /* Mask the character size bits */
    options.c_cflag |= CS8;    /* Select 8 data bits */
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_lflag |= ICANON;

    tcsetattr(fd, TCSANOW, &options);
    return 0;
}

ssize_t write_port(const uint8_t * CommandLine)
{
    ssize_t wlen;
    
    wlen = write(fd, CommandLine, strlen(CommandLine));
    if (wlen == -1)
    {
        printf("write_port: write failed\r\n");
        perror("Error: ");
    }
    return wlen;
}


ssize_t read_port(uint8_t * Buffer, size_t Count)
{
    struct pollfd pfd;
    int8_t ret;
    ssize_t rlen;

    pfd.fd = fd;
    pfd.events = POLLIN;

    ret = poll(&pfd, 1, READ_TOUT_MS);

    if(ret == -1)
    {
        perror("poll");
        return -1;
    }

    if(!ret)
    {
        //printf("read timeout\n");
        return 0;
    }

    if(pfd.revents & POLLIN)
    {
        //printf("Reading up to %d bytes\r\n", (int) Count);
        rlen = read(fd, Buffer, Count);
        if (rlen == -1)
        {
            printf("read_port: failed to read \r\n");
            perror("Error: ");
        } else {
            Buffer[rlen] = 0; //terminate string
        }
        return rlen;
    }
}


void close_port()
{
    close(fd);
}
