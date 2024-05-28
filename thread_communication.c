#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#define send_type 1
#define recv_type 2
#define send1_to_recv 1
#define send2_to_recv 2
#define recv_to_send1 3
#define recv_to_send2 4

void *send_thread_1(void *arg);
void *send_thread_2(void *arg);
void *recv_thread(void *arg);
sem_t send_psx, recv_psx, final_recv_1, final_recv_2;
pthread_t send_pid_1, send_pid_2, recv_pid;

int send1_over = 0;
int send2_over = 0;

struct msgbuf
{
    long mtype;
    char mtext[256];
    int mdir;
};

int msgid;

void *send_thread_1(void *arg)
{
     char info[256];
     struct msgbuf s_msg;
     s_msg.mtype = send_type;
     s_msg.mdir = send1_to_recv;
     while(1)
     {
        sem_wait(&send_psx);
        printf("Sender1 send: ");
        scanf("%s", info);
        if (strcmp(info, "exit") == 0)
        {
            strcpy(s_msg.mtext, "end1");
            msgsnd(msgid, &s_msg, sizeof(struct msgbuf), 0);
            sem_post(&send_psx);
            break;
        }
        strcpy(s_msg.mtext, info);
        msgsnd(msgid, &s_msg, sizeof(struct msgbuf), 0);
        sem_post(&recv_psx);

     }
     sem_wait(&final_recv_1);
     msgrcv(msgid, &s_msg, sizeof(struct msgbuf), recv_type, 0);
     printf("Sender1 receive: %s\n", s_msg.mtext);
     sem_post(&send_psx);
     if (send1_over && send2_over)
     {
        msgctl(msgid, IPC_RMID, 0);
     }
     pthread_exit(NULL);
}

void *send_thread_2(void *arg)
{
    char info[256];
    struct msgbuf s_msg;
    s_msg.mtype = send_type;
    s_msg.mdir = send2_to_recv;
    while (1)
    {
        sem_wait(&send_psx);
        printf("Sender2 send: ");
        scanf("%s", info);
        if (strcmp(info, "exit") == 0)
        {
             strcpy(s_msg.mtext, "end2");
             msgsnd(msgid, &s_msg, sizeof(struct msgbuf), 0);
             sem_post(&recv_psx);
             break;
        }
        strcpy(s_msg.mtext, info);
        msgsnd(msgid, &s_msg, sizeof(struct msgbuf), 0);
        sem_post(&recv_psx);

    }
    sem_wait(&final_recv_2);
    msgrcv(msgid, &s_msg, sizeof(struct msgbuf), recv_type, 0);
    printf("Sender2 receive: %s\n", s_msg.mtext);
    sem_post(&send_psx);
    if (send1_over && send2_over)
    {
        msgctl(msgid, IPC_RMID, 0);
    }
    pthread_exit(NULL);
}

void *recv_thread(void *arg)
{
    struct msgbuf r_msg;
    while(1)
    {
        sem_wait(&recv_psx);
        msgrcv(msgid, &r_msg, sizeof(struct msgbuf), send_type, 0);
        if (r_msg.mdir == send1_to_recv)
        {
            if (strcmp(r_msg.mtext, "end1") == 0)
            {
                strcpy(r_msg.mtext, "over1");
                r_msg.mtype = recv_type;
                r_msg.mdir = recv_to_send1;

                msgsnd(msgid, &r_msg, sizeof(struct msgbuf), 0);
                printf("Receive thread has received end1, returning 'over1'...\n");
                sem_post(&final_recv_1);
                send1_over = 1;
            }
            else
            {
                printf("Receive thread recieved message from sender1: %s\n", r_msg.mtext);
                sem_post(&send_psx);
            }
        }
        else if (r_msg.mdir = send2_to_recv)
        {
            if (strcmp(r_msg.mtext, "end2") == 0)
            {
                strcpy(r_msg.mtext, "over2");
                r_msg.mtype = recv_type;
                r_msg.mdir = recv_to_send2;

                msgsnd(msgid, &r_msg, sizeof(struct msgbuf), 0);
                printf("Receive thread has received end2, returning 'over2'...\n");
                sem_post(&final_recv_2);
                send2_over = 1;
            }
            else
            {
                printf("Receive thread received message from sender2: %s\n", r_msg.mtext);
                sem_post(&send_psx);
            }
        }
        if (send1_over && send2_over)
        {
            break;
        }

    }
    pthread_exit(NULL);
}

int main()
{
    sem_init(&send_psx, 0,1);
    sem_init(&recv_psx, 0, 0);
    sem_init(&final_recv_1, 0, 0);
    sem_init(&final_recv_2, 0, 0);
    msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if (msgid < 0)
    {
        printf("Error: msgget() function return error\n");
        exit(1);
    }
    pthread_create(&send_pid_1, NULL, send_thread_1, NULL);
    pthread_create(&send_pid_2, NULL, send_thread_2, NULL);
    pthread_create(&recv_pid, NULL, recv_thread, NULL);
    pthread_join(send_pid_1, NULL);
    pthread_join(send_pid_2, NULL);
    pthread_join(recv_pid, NULL);
    
    return 0;
}