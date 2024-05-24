#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

#define SHM_NAME "/my_shared_memory"
#define SEM_NAME_SENDER "/sem_sender"
#define SEM_NAME_RECEIVER "/sem_receiver"
#define SHM_SIZE 256

int calculate_expression(const char *expr)
{
	int num1, num2;
	sscanf(expr, "%d+%d", &num1, &num2);
	return num1 + num2;
}

int main()
{
	int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
	char* shm_ptr = (char *) mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	sem_t* sem_sender = sem_open(SEM_NAME_SENDER, 0);
	sem_t* sem_receiver = sem_open(SEM_NAME_RECEIVER, 0);
	while(1)
	{
		sem_wait(sem_receiver);
		if (strcmp(shm_ptr, "end") == 0)
		{
			sprintf(shm_ptr, "over");
			sem_post(sem_sender);
			break;
		}

		printf("Received expression: %s\n", shm_ptr);
		int result = calculate_expression(shm_ptr);
                sprintf(shm_ptr, "%d", result);
		sem_post(sem_sender);
	}

	sem_close(sem_sender);
	sem_close(sem_receiver);
	munmap(shm_ptr, SHM_SIZE);
	close(shm_fd);

	return 0;
}
