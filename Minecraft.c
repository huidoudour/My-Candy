#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void safe_flush(FILE *stream) {
    int c;
    while ((c = getchar())!= '\n' && c!= EOF);
}

int esd1() {
    system("cls");
    printf("��������\n");
    system("java -server -Xms1024M -Xmx4096M -jar fabric-server-launch.jar -nogui");
    return 0;
}

int esd2() {
    system("cls");
    printf("qwq\n");
    return 0;
}

int esd3() {
    system("cls");
    printf("�˳���\n��Ҫ�������ȷ��");
    system("pause");
    system("exit");
    return 0;
}

int main() {
    int num;
    while (1) {
        system("cls");
        printf("����1��2��3ѡ����Ҫ���еĲ���\n");
        printf("��1��������������\n��2���ǡ�=���أ�=��\n��3�����˳��ó���\n");
        printf("������1��2��3��ִ�г��������\n");

        if (scanf("%d", &num)!= 1) {
            printf("����������������롣\n");
            safe_flush(stdin);
            system("pause");
            continue;
        }

        switch (num) {
            case 1:
                safe_flush(stdin);
                esd1();
                system("pause");
                break;
            case 2:
                safe_flush(stdin);
                esd2();
                system("pause");
                break;
            case 3:
                safe_flush(stdin);
                return esd3();
            default:
                printf("����������������롣\n");
                safe_flush(stdin);
                system("pause");
        }
    }
    return 0;
}