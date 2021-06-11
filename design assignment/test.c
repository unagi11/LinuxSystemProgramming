#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t pid;
    
    FILE* fp = fopen("test.txt", "w+");

    char str[10];

    for (int i = 0; i < 10; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            fclose(fp);
            fp = fopen("test.txt", "r+");
            fseek(fp, i * sizeof(str) * 10, SEEK_SET);

            for (int j = 0; j < 10; j++)
            {
                memset(str, '0' + i, sizeof(str)-1);
                str[sizeof(str) -1] = '\n';
                str[sizeof(str)] = 0;

                fwrite(str, sizeof(str), 1, fp);
            }
            exit(1);
        }
    }

    sleep(1);

    return 0;
}
