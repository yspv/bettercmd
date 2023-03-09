#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "inodata.h"

static void print_help()
{
    printf( "-f \t-- file type\n"
            "-p \t-- permissions\n"
            "-a \t-- last access time\n"
            "-m \t-- last modified time\n"
            "-i \t-- inode\n"
            "-s \t-- file size\n"
            "-l \t-- long list\n");
}

static char *get_filetype(const struct stat *st)
{
    char *filetype;
    switch((*st).st_mode & S_IFMT) {
    case S_IFREG:
        filetype = "regular";
        break;
    case S_IFLNK:
        filetype = "symbolic link";
        break;
    case S_IFDIR:
        filetype = "directory";
        break;
    default:
        filetype = "unknown";
    }
    return filetype;
}

static void print_perms(const struct stat *st)
{
    int ugo[3], i, p, n;
    char perms[9];
    ugo[0] = (*st).st_mode & S_IRWXU;         /* user permissons  */
    ugo[1] = (*st).st_mode & S_IRWXG;         /* group permissons */
    ugo[2] = (*st).st_mode & S_IRWXO;         /* other permissons */
    i = 0;
    p = 0;
    n = 100;
    for(; p < 3; p++, i += 3, n /= 10) {
        perms[i] = ugo[p] & (n * 4) ? 'r' : '_';
        perms[i+1] = ugo[p] & (n * 2) ? 'w' : '_';
        perms[i+2] = ugo[p] & (n * 1) ? 'x' : '_';
    } 
    perms[i] = '\0';
    printf("%s\n", perms);
}

int main(int argc, char **argv)
{
    struct stat st;
    char *options;
    if(argc < 2) {
        fprintf(stderr, "usage:  inodata [file_name]\n"
                "\tinodata [-optionsions] [filename]\n"
                "\ntype -h for help\n");
        exit(1);
    }

    options = argv[1];

    if(argc > 2) {
        if(lstat(argv[2], &st) < 0 ) {
            perror(argv[2]);
            exit(2);
        }
        options = argv[1];
    } 
    else if(strcmp(options, "-h") != 0) {
        if(lstat(argv[1], &st) < 0) {
            perror(argv[1]);
            exit(2);
        }
        options = "-l";
    }


    for(char *opt = ++options; *opt != '\0'; opt++) {
        switch(*opt) {
        case 'h':                               /* help */
            print_help();
            break;
        case 'l':                               /* long list */
            opt = "-fisnpacm";
            break;
        case 'a':                               /* last access */
            printf("%s", ctime(&st.st_atime));
            break;    
        case 'c':                               /* status change */
            printf("%s", ctime(&st.st_ctime));
            break;
        case 'm':                               /* last modification */
            printf("%s", ctime(&st.st_mtime));
            break;
        case 'i':                               /* inode */
            printf("inode: %llu\n", st.st_ino);
            break;
        case 's':                               /* file size on bytes */
            printf("size: %lld bytes\n", st.st_size);
            break;
        case 'f':                               /* file type */
            printf("file type: %s\n", get_filetype(&st));
            break;
        case 'n':                               /* hard links */
            printf("hard links: %d\n", st.st_nlink);
            break;
        case 'p':                               /* permissions  */
            printf("permissions: ");
            print_perms(&st);
            break;
        default:
            fprintf(stderr, "-%c: no such options.\n"
                    "type -h for help\n", *opt);
            exit(3);
        }
    }
    
    return 0;
}
