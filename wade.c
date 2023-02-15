#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <limits.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXLINE 4096
#define MAXARGS 128
#define MAXJOBS 16
#define MAXJID 1<<16

#define LISTENQ 1024
#define MAXBUF 8192

#define SA struct sockaddr

#define SIOCGIFCONF 0x8912
#define SIOCGIFFLAGS 0x8913
#define SIOCGIFADDR 0x8915
#define SIOCGIFNETMASK 0x891b

//mon premier programme github avancee

int main(int argc, char **argv)
{
    int i;
    char **ptr;
    extern char **environ;
   
    printf("hello world from process ID %d \n", (int)getpid());
    printf("argc = %d \n", argc);

    for (i = 0; i < argc; i++) /* echo all command-line args */
        printf("argv[%d]: %s \n", i, argv[i]);

    for (ptr = environ; *ptr != 0; ptr++) /* and all env strings */
        printf("%s \n", *ptr);

    exit(0);
}