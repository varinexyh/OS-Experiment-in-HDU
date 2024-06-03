#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#define DISK_SIZE 1024 * 1024 //Total disk size: 1MB
#define BLOCK_SIZE 512
#define MAX_FILES 128
#define MAX_FILENAME 16
#define MAX_PATH 256
typedef struct 
{
    char name[MAX_FILENAME];
    int size;
    int start_block;
    int is_directory;
} FileEntry;
typedef struct {
    char name[MAX_FILENAME];
    int file_count;
    FileEntry files[MAX_FILES];
} Directory;
typedef struct 
{
    char data[BLOCK_SIZE];
} Block;
typedef struct
{
    Block blocks[DISK_SIZE / BLOCK_SIZE];
    Directory root;
} FileSystem;
extern FileSystem fs;
extern Directory *current_dir;
extern FileEntry *opened_file;
void my_format();
void my_mkdir(char *name);
void my_rmdir(char *name);
void my_ls();
void my_cd(char *name);
void my_create(char *name);
void my_rm(char *name);
void my_open(char *name);
void my_close();
void my_write(char *data);
void my_read();
void my_exitsys();
void my_loadsys();
#endif