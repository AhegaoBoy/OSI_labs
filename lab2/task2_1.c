#include<stdio.h>
#include<unistd.h>
void main()
{
    int pid = fork();
    printf("%d\n", pid);
}
//В данной программе наблюдается вывод номеров потоков. Первым выводится номер потока потомка