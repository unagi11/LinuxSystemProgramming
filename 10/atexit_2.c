#include <stdio.h>
#include <stdlib.h>

static void ssu_exit1(void);
static void ssu_exit2(void);

int main(int argc, char const *argv[])
{
    // ssu_exit2등록
    if (atexit(ssu_exit2) != 0) {
        fprintf(stderr, "ateixt error for ssu_exit2");
        exit(1);
    }
    // ssu_exit1 두번 등록
    if (atexit(ssu_exit1) != 0) {
        fprintf(stderr, "ateixt error for ssu_exit2");
        exit(1);
    }
    if (atexit(ssu_exit1) != 0) {
        fprintf(stderr, "ateixt error for ssu_exit2");
        exit(1);
    }

    printf("done\n");

    // 등록한 atexit의 역순으로 호출이 된다.
    return 0;
}

static void ssu_exit1(void) 
{
    printf("ssu_exit1 handler\n");
}

static void ssu_exit2(void)
{
    printf("ssu_exit2 handler\n");
}