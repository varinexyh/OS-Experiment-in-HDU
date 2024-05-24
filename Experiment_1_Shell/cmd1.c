#include <stdio.h>
#include <stdlib.h>

void copyFile(const char *source, const char *destination) {
    FILE *src = fopen(source, "r");
    if (src == NULL) {
        perror("Error opening source file");
        return;
    }

    FILE *dest = fopen(destination, "w");
    if (dest == NULL) {
        perror("Error opening destination file");
        fclose(src);
        return;
    }

    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytesRead, dest);
    }

    fclose(src);
    fclose(dest);
    printf("File copied successfully.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: cmd1 <source> <destination>\n");
        return 1;
    }

    copyFile(argv[1], argv[2]);
    return 0;
}