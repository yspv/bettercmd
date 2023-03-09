#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

enum {
    bufsize = 255
};

int main(int argc, char **argv)
{
    char buf[bufsize + 1];
    int read_byte;

    if(argc < 2) {
        fprintf(stderr, "Please input symbolic link.");
        exit(1);
    }
    for(int i = 1; i < argc; i++) {
        read_byte = readlink(argv[i], buf, bufsize);
        if(read_byte < 0) {
            perror(argv[1]);
            exit(2);
        }
        buf[read_byte] = '\0';
        printf("%s -> %s\n", argv[i], buf);
    }
    
    return 0;
}
