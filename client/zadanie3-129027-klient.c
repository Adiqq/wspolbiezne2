#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main(int argc, char** argv) {
    assert(argc > 1);
    char serverName[] = "/home/adiq/c/serverFifo";
    char clientName[] = "/home/adiq/c/clientFifo";
    char buffer[512];
    int fd;
    char input[256];
    strcpy(input,argv[1]);
    for(int i = 2; i < argc; i++){
        strcat(input, " ");
        strcat(input, argv[i]);
    }
    if (mkfifo(serverName, S_IRUSR | S_IWUSR) == -1) {
        puts("server fifo already exists");
        //exit(1);
    }
    if ((fd = open(serverName, O_WRONLY)) == -1) {
        perror("error on opening server file");
        exit(1);
    }
    int clientFd;
    if (mkfifo(clientName, S_IRUSR | S_IWUSR) == -1) {
        puts("client fifo already exists");
        //exit(1);
    }
    char command[512];
    command[0] = (char)strlen(clientName);
    command[1] = '\0';
    strcat(command, clientName);
    int commandLength = (char)strlen(command);
    command[commandLength] = (char)strlen(input);
    command[commandLength + 1] = '\0';
    strcat(command, input);

    size_t length = strlen(command);
    if(write(fd, command, length) == -1){
        perror("tx error");
        exit(3);
    }
    if ((clientFd = open(clientName, O_RDONLY)) == -1) {
        perror("error on opening client file");
        exit(1);
    }
    if (read(clientFd, buffer, sizeof buffer) == -1){
        exit(2);
    }
    puts(buffer);
    close(clientFd);
    close(fd);

    return 0;
}