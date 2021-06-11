/*
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *statloc);
pid_t waitpid(pid_t pid, int *statloc, int options);

리턴 값 : 성공 시 프로세스 ID, 에러 시 -1 (waitpid()의 경우, WNOHANG 옵션으로 실행되었고 자식 프로세스가 종료되지 않았다면, 0을 리턴)

프로세스의 종료 상태를 회수하는 시스템 호출 함수
자식프로세스의 종료가 언제 발생할지 부모 프로세스는 알 수 없으므로 커널은 시그널을 통해 부모 프로세스에게 알림
 - 자식 프로세스가 종료했을 때, 커널은 부모 프로세스에게 SIGCHLD 시그널을 보냄.
 - 부모 프로세스는 이 시그널을 무시할 수도 있고, 핸들러 함수를 등록하여 핸들러에게 규저오딘 동작을 할 수 있음.

statloc 인자의 종료 상태 값
1. 자식 프로세스가 exit()를 호출
 - 하위 8bits : exit()의 인자
 - 상위 8bits : 0x00
2. 자식 프로세스가 시그널에 의해 종료
 - 하위 8bits : 0x00
 - 중간 1bits : Coreflag
 - 상위 7bits : Signal No.
3. 자식 프로세스가 SIG_STP, SIG_STOP에 의해 일시중단 됨
 - 하위 8bits : Signal No.
 - 상위 8bits : 0x7f

pid 인자에 따른 용도
1. pid == -1 : 임의의 프로세스를 기다림 wait()함수와 동일하다.
2. pid > 0 : 프로세스 ID와 pid인 프로세스를 기다림
3. pid == 0 : 프로세스 그룸 ID가 호출한 프로세스와 동일한 임의의 자식 프로세스를 기다림.
4. pid < 0 : 프로세스 그룸 ID가 pid의 절대값과 같은 임의의 자식 프로세스를 기다림.
ex.) pid = -10이면, 프로세스 그룹 ID가 10인 자식 프로세스를 기다린다.

options 인자에 사용할수 있는 옵션들

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void ssu_echo_exit(int status);

int main(int argc, char const *argv[])
{
    pid_t pid;
    int status;

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid == 0)
        exit(7);
    if (wait(&status) != pid)
    {
        fprintf(stderr, "wait error\n");
        exit(1);
    }
    ssu_echo_exit(status);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid == 0)
        abort();
    if (wait(&status) != pid)
    {
        fprintf(stderr, "wait error\n");
        exit(1);
    }
    ssu_echo_exit(status);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid == 0)
        status /= 0;
    if (wait(&status) != pid)
    {
        fprintf(stderr, "wait error\n");
        exit(1);
    }
    ssu_echo_exit(status);
    exit(0);
}

void ssu_echo_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d %s\n", WTERMSIG(status),
#ifdef WCOREDUMP
    WCOREDUMP(status) ? "(core file generated)" : "");
#else
    "");
#endif
    else if (WIFSTOPPED(status))
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
}