#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<time.h>
#include<semaphore.h>

#define SHM_NAME "/my_shared_memory"
#define SEM_NAME_SENDER "/sem_sender"
#define SEM_NAME_RECEIVER "/sem_receiver"
#define SHM_SIZE 256

void generate_expression(char *expr)
{
	int num1 = rand() % 100;
	int num2 = rand() % 100;
	sprintf(expr, "%d+%d", num1, num2);
}

int main()
{
	srand(time(NULL));
	int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SHM_SIZE);
	char* shm_ptr = (char*) mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        sem_t* sem_sender = sem_open(SEM_NAME_SENDER, O_CREAT, 0666, 1);
	sem_t* sem_receiver = sem_open(SEM_NAME_RECEIVER, O_CREAT, 0666, 0);
	for (int i = 0; i < 10; i++)
	{
		char expression[50];
		generate_expression(expression);

		sem_wait(sem_sender);
		sprintf(shm_ptr, "%s", expression);
		sem_post(sem_receiver);

		sem_wait(sem_sender);
		printf("Result: %s\n", shm_ptr);
	}

	sem_wait(sem_sender);
	sprintf(shm_ptr, "end");
	sem_post(sem_receiver);

	sem_wait(sem_sender);
	printf("Received: %s\n", shm_ptr);
	
	//clear the memory and the posix semaphore sources that used above
	
	sem_close(sem_sender);
	sem_close(sem_receiver);
	sem_unlink(SEM_NAME_SENDER);
	sem_unlink(SEM_NAME_RECEIVER);
	munmap(shm_ptr, SHM_SIZE);
	close(shm_fd);
	shm_unlink(SHM_NAME);

	return 0;
}
