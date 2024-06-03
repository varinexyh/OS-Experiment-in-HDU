#include "filesystem.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//Import some libs above.
FileSystem fs;
Directory *current_dir;
FileEntry *opened_file = NULL;

void my_format()
{
    //Initializing the file system using the memset() function.
    memset(&fs, 0, sizeof(FileSystem));
    strcpy(fs.root.name, "/");
    current_dir = &fs.root;
    printf("System has been formatted.\n");
}

void my_mkdir(char *name)
{
    //Check the number of files in single directory.
    if (current_dir -> file_count >= MAX_FILES)
    {
        printf("The current directory is full.\n");
        return;
    }
    //Creating a new directory.
    FileEntry *new_dir = &current_dir -> files[current_dir -> file_count++];
    strcpy(new_dir ->name, name);
    new_dir -> is_directory = 1;
    printf("Directory %s is created.\n", name);
}
void my_rmdir(char *name)
{
    //Check the corresponding directory by routing the files in single directory.
    for (int i = 0; i < current_dir -> file_count; i++)
    {
        if (strcmp(current_dir -> files[i].name, name) == 0 && current_dir -> files[i].is_directory)
        {
            for (int j = i; j < current_dir -> file_count - 1; j++)
            {
                current_dir -> files[j] = current_dir -> files[j + 1];
            }
            current_dir -> file_count --;
            printf("Directory %s is removed.\n", name);
            return;
        }
    }
    printf("Directory %s is not found.\n", name);
}

void my_cd(char *name)
{
    //Referring to root directory. 
    if (strcmp(name, "..") == 0)
    {
        current_dir = &fs.root;
        printf("Changed to root directory.\n");
        return;
    }
    //Routing the whole directory to find.
    for (int i = 0; i < current_dir -> file_count; i++)
    {
        if (strcmp(current_dir -> files[i].name, name) == 0 && current_dir -> files[i].is_directory)
        {
            current_dir = (Directory *) &current_dir -> files[i];
            printf("Changed to directory: %s\n", name);
            return;
        }
    }
    printf("Directory %s is not found.\n", name);
}

void my_ls()
{
    //Output the files in same directory.
    for (int i = 0; i < current_dir -> file_count; i++)
    {
        printf("%s%s\n", current_dir -> files[i].name, current_dir -> files[i].is_directory ? "/" : "");

    }
}
void my_create(char *name)
{
    //CHeck whether the directory is full status.
    if (current_dir -> file_count >= MAX_FILES)
    {
        printf("The directory is full.\n");
        return;
    }
    //Add up the file_count and give the new file name.
    FileEntry *new_file = &current_dir -> files[current_dir -> file_count++];
    strcpy(new_file -> name, name);
    new_file -> is_directory = 0;
    printf("The file %s is successfully created.\n", name);
}
void my_rm(char *name)
{
    for (int i = 0; i < current_dir -> file_count; i++)
    {
        if (strcmp(current_dir -> files[i].name, name) == 0 && !current_dir -> files[i].is_directory)
        {
            for (int j = i; j < current_dir -> file_count - 1 ;j++)
            {
                //Removing single file by covering its place by other files(NOT PHYSICALLY DELETE!)
                current_dir -> files[j] = current_dir -> files[j + 1];
            }
            current_dir -> file_count --;
            printf("File %s is removed.\n", name);
            return;
        }
    }
    printf("The file %s is not found in the directory.\n");
}

void my_open(char *name)
{
    for (int i = 0; i < current_dir -> file_count; i++)
    {
        if (strcmp(current_dir -> files[i].name, name) == 0 && !current_dir ->files[i].is_directory)
        {
            //Make sure file is open.
            opened_file = &current_dir -> files[i];
            printf("File %s is opened.\n", name);
            return;
        }
    }
    printf("File '%s' is not found in the directory.\n", name);
}

void my_close()
{
    //Check if there's an opening file.
    if (opened_file)
    {
        printf("The opened file '%s' is closed.\n", opened_file ->name);
        opened_file = NULL;
    }
    else
    {
        printf("There is no existing opening file.\n");
    }
}

void my_write(char *data)
{
    if(!opened_file)
    {
        printf("There is no opening file.\n");
        return;
    }
    //write the characters in data array into fs by memcpy() function physically.
    int size = strlen(data);
    opened_file -> size = size;
    int start_block = opened_file -> start_block;
    memcpy(fs.blocks[start_block].data, data, size);
    printf("Successfully written to file '%s'.\n", opened_file -> name);
}

void my_read()
{
    //Make sure there is an opening file.
    if (!opened_file)
    {
        printf("There is no opening file.\n");
        return;
    }
    int start_block = opened_file -> start_block;
    printf("%s\n", fs.blocks[start_block].data);
}

void my_exitsys()
{
    //Set up a dat file the store a filesystem.
    FILE *fp = fopen("filesystem.dat", "wb");
    if (fp)
    {
        //Using fwrite() function to wirte the data of file system(In convenience of next visit).
        fwrite(&fs, sizeof(FileSystem), 1, fp);
        fclose(fp);
        printf("File system saved.\n");
    }
    else
    {
        //Robustness
        printf("Error saving file system.\n");
    }
}

void my_loadsys()
{
    FILE *fp = fopen("filesystem.dat", "rb");
    if (fp)
    {
        //Using fread() function to visit the file system stored before in "filesystem.dat"
        fread(&fs, sizeof(FileSystem), 1, fp);
        fclose(fp);
        //Make sure the pointer of file system is pointed to root directory.
        current_dir = &fs.root;
        printf("File system loaded.\n");
    }
    else
    {
        //If using the file system in first time, must initializing the storage form.
        my_format();
    }
}
