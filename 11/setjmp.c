#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>

void ssu_nested_func(int loc_var, int loc_volatile, int loc_register);

static jmp_buf glob_buffer;

int main(int argc, char const *argv[])
{
    register int loc_register; // 레지스터를 사용합니다.
    volatile int loc_volatile; // 메모리를 최적화 하지 않습니다.
    int loc_var;

    loc_var = 10;
    loc_volatile = 11;
    loc_register = 12;

    // 이 위치를 저장?
    if (setjmp(glob_buffer) != 0) 
    {
        printf("after longjmp, loc_var = %d, loc_volatile = %d, loc_register = %d\n", loc_var, loc_volatile, loc_register);
        // register는 레지스터 환경을 저장해 놓아서 그것으로 복구한다함
        // -O옵션을 주면, 로컬 변수도 레지스터를 사용해서 setjmp로 복구가됨
        exit(0);
    }

    loc_var = 80;
    loc_volatile = 81;
    loc_register = 82;

    ssu_nested_func(loc_var, loc_volatile, loc_register);

    return 0;
}

void ssu_nested_func(int loc_var, int loc_volatile, int loc_register)
{
    printf("before longjmp, loc_var = %d, loc_volatile = %d, loc_register = %d\n", loc_var, loc_volatile, loc_register);
    longjmp(glob_buffer, 1);
}
