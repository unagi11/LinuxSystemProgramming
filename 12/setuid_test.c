#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char line[256];
    FILE *fp;
    int uid;
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s file_name\n", argv[0]);
        exit(1);
    }

    // 처음에 그냥 파일을 열어서 내용물을 출력한다.

    printf("initially uid = %d and euid = %d\n", getuid(), geteuid());
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        fprintf(stderr, "first open error for %s\n", argv[1]);
        exit(1);
    }
    else
    {
        printf("first open successful: \n");
        while (fgets(line, 256, fp) != NULL)
            fputs(line, stdout);
        fclose(fp);
    }

    // 본래 나의 uid로 변경하여, 내용물을 출력해본다.
    // 본래 나의 uid가 파일을 열 권한이 없다면, 내용물을 볼 수 없을것이다.

    setuid(uid = getuid());
    printf("after setuid(%d):\n uid = %d and euid = %d\n ", uid, getuid(), geteuid());

    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        fprintf(stderr, "second open error for %s\n", argv[1]);
        exit(1);
    }
    else
    {
        printf("second open succeful: \n");
        while (fgets(line, 255, fp) != NULL)
            fputs(line, stdout);
        fclose(fp);
    }
    exit(0);
}