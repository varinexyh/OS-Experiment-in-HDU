#include "filesystem.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//Importing the libs
int main()
{
    //Loading the file system first.
    my_loadsys();
    //Set two char arrays to store the command and operation objects.
    char command[MAX_PATH];
    char arg[MAX_PATH];
    while(1)
    {
        printf(">");
        scanf("%s", command);
         if (strcmp("my_format", command) == 0)
         {
            my_format();
         }
         else if (strcmp("my_mkdir", command) == 0)
         {
            scanf("%s", arg);
            my_mkdir(arg);
         }
         else if (strcmp("my_rmdir", command) == 0)
         {
            scanf("%s", arg);
            my_rmdir(arg);
         }
         else if (strcmp("my_ls", command) == 0)
         {
            my_ls();
         }
         else if (strcmp("my_cd", command) == 0)
         {
            scanf("%s", arg);
            my_cd(arg);
         }
         else if (strcmp("my_create", command) == 0)
         {
            scanf("%s", arg);
            my_create(arg);
         }
         else if (strcmp("my_rm", command) == 0)
         {
            scanf("%s", arg);
            my_rm(arg);
         }
         else if (strcmp("my_open", command) == 0)
         {
            scanf("%s", arg);
            my_open(arg);
         }
         else if (strcmp("my_close", command) == 0)
         {
            my_close();
         }
         else if (strcmp("my_write", command) == 0)
         {
            getchar(); //Offset single character.
            fgets(arg, MAX_PATH, stdin);
            arg[strcspn(arg, "\n")] = 0; //Removing the another line characters.
            my_write(arg);
         }
         else if (strcmp("my_read", command) == 0)
         {
            my_read();
         }
         else if (strcmp("my_exitsys", command) == 0)
         {
            my_exitsys();
            break;
         }
         else
         {
             //Robustness operations.
            printf("Unknown command.\n");
         }
    }
    return 0;
}
