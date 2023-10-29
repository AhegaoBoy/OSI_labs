#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid();
    printf("Идентификатор текущего процесса: %d\n", pid);

    pid_t ppid = getppid();
    printf("Идентификатор родительского процесса: %d\n", ppid);

    gid_t pgrg = getpgrp();
    printf("Идентификатор группы вызывающего процесса: %d\n", pgrg);

    uid_t uid = getuid();
    printf("Реальный идентификатор владельца: %d\n", uid);

    gid_t gid = getgid();
    printf("Реальный идентификатор группы владельца: %d\n", gid);

    uid_t euid = geteuid();
    printf("Эффективный идентификатор владельца: %d\n", euid);

    gid_t egid = getegid();
    printf("Эффективный идентификатор группы владельца: %d\n", egid);

    return 0;
}