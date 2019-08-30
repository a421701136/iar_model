#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char tmp[50];
    memset(tmp, 0, sizeof(tmp));
    printf("argc = %d\r\n", argc);
    if (argc > 1) {
        strcpy(tmp, argv[1]);
        printf("main: %s\n", tmp);
    } else {
        printf("input: exe argc\n");
    }
    
}