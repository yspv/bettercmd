/*    
 *    Program for redirecting output stream
 *   
 *    'w'  == '>'
 *    'wa' == '>>'
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


/* Checking for an argument has a redirect symbol  */
int is_rdt(const char *str)
{
    if(!(strcmp(str, "w")) || !(strcmp(str, "wa"))) {
        return 1;
    }
    return 0;
}

void rewrite(int *fd, const char *filename)
{
    *fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0666);
    if(*fd == -1) {
        perror(filename);
        exit(3);
    }
}

void write_append(int *fd, const char *filename)
{
    *fd = open(filename, O_CREAT|O_WRONLY|O_APPEND, 0666);
    if(*fd == -1) {
        perror(filename);
        exit(4);
    }
}

int main(int argc, char **argv)
{
    int i, pid, fd, status;
    char **args;

    if(argc < 4) {
        fprintf(stderr, \
                "Error: Arguments should be greater then two\n");
        exit(1);
    }
    
    args = malloc(argc * sizeof(char *));
    
    for(i = 1; (i < argc) && !(is_rdt(argv[i])); i++) {
        args[i-1] = argv[i];
    }

    args[i-1] = NULL;

    if(i == argc) {
        fprintf(stderr, \
                "Error: Arguments should include 'w' or 'rw'.\n");
        exit(2);
    }

    if(!(strcmp(argv[i], "w"))) {
        rewrite(&fd, argv[i+1]);
    } 
    if(!(strcmp(argv[i], "wa"))) {
        write_append(&fd, argv[i+1]);
    }

    pid = fork();
    if(pid == -1) {
        perror("fork()");
        exit(5);
    }
    if(pid == 0) {
        dup2(fd, 1);                             /* change std output */
        close(fd);
        execvp(argv[1], args);
        perror(argv[1]);
        exit(6);
    }

    close(fd);
    wait(&status);

    return 0;
}
