#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 清除输入缓冲区
void safe_flush(FILE *stream) {
    int c;
    while ((c = getchar())!= '\n' && c!= EOF);
}

int startServer() {
    system("cls");
    printf("即将启动\n");
    system("java -server -Xms1024M -Xmx4096M -jar fabric-server-launch.jar -nogui");
    return 0;
}

int doSomethingElse() {
    system("cls");
    printf("qwq\n");
    return 0;
}

int exitProgram() {
    system("cls");
    printf("退出？\n需要按任意键确认");
    system("pause");
    system("exit");
    return 0;
}

int main() {
    char input[10];
    while (1) {
        system("cls");
        printf("输入1、2、3选择需要进行的操作\n");
        printf("“1”是启动服务器\n“2”是“=￣ω￣=”\n“3”是退出该程序\n");
        printf("请输入1或2或3以执行程序分命令，或者输入‘exit’退出\n");

        if (scanf("%9s", input)!= 1) {
            printf("输入错误，请重新输入。\n");
            safe_flush(stdin);
            system("pause");
            continue;
        }

        if (strcmp(input, "1") == 0) {
            safe_flush(stdin);
            startServer();
            system("pause");
        } else if (strcmp(input, "2") == 0) {
            safe_flush(stdin);
            doSomethingElse();
            system("pause");
        } else if (strcmp(input, "3") == 0 || strcmp(input, "exit") == 0) {
            return exitProgram();
        } else {
            printf("输入错误，请重新输入。\n");
            safe_flush(stdin);
            system("pause");
        }
    }
    return 0;
}