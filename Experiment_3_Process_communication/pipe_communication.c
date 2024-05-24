#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>
#include<pthread.h>
#include<semaphore.h>

#define NUM_CHILDREN 3
#define BUFFER_SIZE 256

sem_t mutex;
void *child_process(char *arg)
{
    //Use the pipe created below.
      int pipe_fd = *((int *) arg);
    //Initialize the message and limit the length of the message(the content of the massage is no longer than 256 bits.)
      char message[BUFFER_SIZE];
      int length;
      srand(getpid());
      length = rand() % BUFFER_SIZE;
      for (int i = 0; i < length; i++)
      {
        message[i] = 'A' + (rand() % 26);
      }
      message[length] = '\0';
      //write the message into the mutex semaphore.
      sem_wait(&mutex);
      write(pipe_fd, message, strlen(message) + 1);
      sem_post(&mutex);

      printf("Children process %d sent the message: %s\n", getpid(), message);
      return NULL;
}

int main()
{
    //Initialize the pipe and the children process.
    int pipe_fd[2];
    pthread_t children[NUM_CHILDREN];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    //Initialize the mutex semaphore.
    sem_init(&mutex, 0, 1);
    //Create the children process seperately through the pthread_create() function in pthread.h. 
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if (pthread_create(&children[i], NULL, child_process, &pipe_fd[1]) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }

    }
    //Make the children processes wait.
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pthread_join(children[i], NULL);
    }
    //Read the meassage from child process through buffer area, and the message is conveyed by pipe that we created before(by the 0 side).
    char buffer[BUFFER_SIZE];
    int bytes_read;
    while((bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE)) > 0)
    {
        printf("Parent received message: %s\n", buffer);
    }
    //Make sure the robustness.
    if (bytes_read == -1)
    {
        perror("read");

    }
    //Recycle the pipes that we create above.
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    //Recycle the mutex semaphore.
    sem_destroy(&mutex);

    return 0;
}  