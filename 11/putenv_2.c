#include <stdio.h>
#include <stdlib.h>

void ssu_printenv(char* label, char*** envpp);

extern char** environ;

int main(int argc, char *argv[], char *envp[])
{
    ssu_printenv("Initially", &envp);
    putenv("TZ=PST8PDT");
    // TZ를 배정하고 나면, environ의 주소가 바뀐다. (현 프로세스 메모리 공간으로 옮겨오는듯)
    // 하지만, 안에 존재하는 char* 는 변하지 않는다.
    ssu_printenv("After changeing TZ", &envp);
    putenv("WARNING=Don't use argv after putenv()");
    ssu_printenv("After setting a new variable", &envp);
    printf("value of WARNING is %s\n", getenv("WARNING"));

    return 0;
}

void ssu_printenv(char* label, char*** envpp)
{
    char **ptr;

    printf("--- %s ---\n", label);
    printf("envp    is at %12o and contains %12o\n", envpp, *envpp);
    printf("environ is at %12o and contains %12o\n", &environ, environ);
    printf("My enviroment variable are:\n");
    for (ptr = environ; *ptr; ptr++)
        printf("(%8o) = %8o -> %s\n", ptr, *ptr, *ptr);
    printf("(%8o) = %8o\n", ptr, *ptr);    
}