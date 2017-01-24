#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main() {
    char name[] = "/tmp/serverFifo";
    char buffer[512];
    int fd;
    int clientFd;
    if (mkfifo(name, S_IRUSR | S_IWUSR) == -1) {
        perror("fifo already exists");
    }
    if ((fd = open(name, O_RDONLY)) == -1) {
        perror("error on opening server file");
        exit(1);
    }
    while(1){
        ssize_t readBytes = read(fd, buffer, sizeof buffer);
        if (readBytes == -1) {
            perror("Could not read server fifo to buffer");
            exit(5);
        }
        if (readBytes == 0){
            close(fd);
            if ((fd = open(name, O_RDONLY)) == -1) {
                perror("error on opening server file");
                exit(1);
            }
            continue;
        }
        size_t nameLength = (size_t)buffer[0];
        char clientFifoName[256];
        strncpy(clientFifoName, buffer + 1, nameLength);
        size_t commandLength = (size_t)buffer[nameLength + 1];
        char command[256];
        strncpy(command, buffer + nameLength + 2, commandLength);
        command[commandLength] = '\0';

        pid_t pid = fork();

        if(pid == 0){
            if ((clientFd = open(clientFifoName, O_WRONLY)) == -1) {
                perror("error on opening client file");
            } else {
                dup2(clientFd, 1);

                char *arg = strtok(command, " ");
                int i = 0;
                char *args[32];
                args[0] = arg;
                while (arg != NULL)
                {
                    //remove EOF char
                    char *pos;
                    if ((pos=strchr(arg, '\n')) != NULL)
                        *pos = '\0';

                    args[i++] = arg;
                    arg = strtok (NULL, " ");

                }
                //add terminal character to array
                args[i] = NULL;

                if(execvp(args[0], args) == -1){
                    perror("Error");
                }
            }
            exit(0);
        } else if(pid != -1){
            //parent
        } else{
            perror("Child could not be created");
            exit(4);
        }

    }
    return 0;
}