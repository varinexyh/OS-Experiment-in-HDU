#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#define DISK_SIZE 1024 * 1024 //Total disk size: 1MB
#define BLOCK_SIZE 512 //We divided the visual storage into 512 blocks.
#define MAX_FILES 128 //Maximum files stored in single directory.
#define MAX_FILENAME 16 //Maximum file name length(16 characters).
#define MAX_PATH 256 //Maximum file path length.
typedef struct 
{
    char name[MAX_FILENAME];
    int size;
    int start_block; //To record the start block of single file in physical disk storage.
    int is_directory; //Mark the file whether a directory.
} FileEntry;
typedef struct {
    char name[MAX_FILENAME];
    int file_count; //File number in single directory.
    FileEntry files[MAX_FILES];
} Directory;
typedef struct 
{
    char data[BLOCK_SIZE]; //Physical data stored in disk(read and write operations only).
} Block;
typedef struct
{
    Block blocks[DISK_SIZE / BLOCK_SIZE]; //Divide the whole storage in to equal blocks.
    Directory root; //Root directory of file system.
} FileSystem;
extern FileSystem fs;
extern Directory *current_dir;
extern FileEntry *opened_file;
//Below are operations within the file system.
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
