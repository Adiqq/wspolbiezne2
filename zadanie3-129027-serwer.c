#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char name[] = "myfifo";
    int fd;
    if (mkfifo(name, S_IRUSR | S_IWUSR) == -1) {
        perror("fifo already exists");
        //exit(1);
    }
    if ((fd = open(name, O_RDONLY)) == -1) {
        perror("error on opening file");
        exit(1);
    }
    while(1){

    }
    return 0;
}