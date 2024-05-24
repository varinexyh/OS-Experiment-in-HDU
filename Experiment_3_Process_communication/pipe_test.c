#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>

#define BUFFER_SIZE 1024

int main()
{
    int pipe_fd[2];
    char buffer[BUFFER_SIZE];
    int bytes_written = 0;
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    fcntl(pipe_fd[1], F_SETFL, O_NONBLOCK);

    while(1)
    {
        int result = write(pipe_fd[1], buffer, BUFFER_SIZE);
        if (result == -1)
        {
            if (errno = EAGAIN)
            {
                printf("Pipe is full. Total bytes written: %d\n", bytes_written);
                break;
            }
            else
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        bytes_written += result;

    }
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    
    return 0;
}