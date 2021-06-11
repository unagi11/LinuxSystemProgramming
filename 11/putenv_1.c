/*
#include <stdlib.h>

char* getenv(const char* name); 
- 환경 변수 리스트중에서 name의 값을 찾아 포인터를 리턴해준다. 찾지 못한다면 NULL 리턴

int putenv(char* str); 
- 환경 변수의 값을 변경하거나 새로운 환경 변수를 등록한다. str에 "name=value"식으로 등록.
- 성공시 0, 에러 시 0이 아닌값을 리턴하고 errno를 설정

int setenv(const char* name, const char* value, int rewrite); 
- name과 value를 따로 등록 putenv과 비슷하지만, rewrite를 0로하면 덮어쓰지 않는다.
- 성공시 0, 에러 시 0이 아닌값을 리턴하고 errno를 설정

int unsetenv(const char* name);
- 환경변수 이름에 대한 정의를 초기화할 때 사용
- 만약 환경 변수 이름을 찾을 수 없는 경우에는 -1리턴하고 errno를 설정
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void ssu_addone(void);

extern char** environ;
char glob_var[] = "HOBBY=swimming";

int main(void)
{
    for (int i = 0; environ[i] != NULL; i++)
        printf("environ[%d] : %s\n", i, environ[i]);

    putenv(glob_var);

    ssu_addone();

    printf("My hobby is %s\n", getenv("HOBBY"));
    printf("MY lober is %s\n", getenv("LOVER"));

    // environ이 char**형이고, glob_var를 변경하면 HOBBY가 변경되는걸 보아
    // 환경변수 등록을하면 char*를 포인터로 참조하고있을 뿐인거 같다.
    strcpy(glob_var + 6, "fishing");

    for (int i = 0; environ[i] != NULL; i++)
        printf("environ[%d] : %s\n", i, environ[i]);

    return 0; 
}

void ssu_addone(void)
{
    char auto_var[10];
    strcpy(auto_var, "LOVER=js");
    putenv(auto_var);
    // char* 를 연결해주기 때문에 지역 메모리를 환경변수로 배정하면 함수가 끝날때 사라진다.
}