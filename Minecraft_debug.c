#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ������뻺����
void safe_flush(FILE *stream) {
    int c;
    while ((c = getchar())!= '\n' && c!= EOF);
}

int startServer() {
    system("cls");
    printf("��������\n");
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
    printf("�˳���\n��Ҫ�������ȷ��");
    system("pause");
    system("exit");
    return 0;
}

int main() {
    char input[10];
    while (1) {
        system("cls");
        printf("����1��2��3ѡ����Ҫ���еĲ���\n");
        printf("��1��������������\n��2���ǡ�=���أ�=��\n��3�����˳��ó���\n");
        printf("������1��2��3��ִ�г��������������롮exit���˳�\n");

        if (scanf("%9s", input)!= 1) {
            printf("����������������롣\n");
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
            printf("����������������롣\n");
            safe_flush(stdin);
            system("pause");
        }
    }
    return 0;
}