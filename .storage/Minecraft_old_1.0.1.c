#define _CRT_SECURE_NO_WARNING
#include <stdio.h>
#include <stdlib.h>
int esd1 () {
	system("cls");
	printf("��������\n");
//	system("cd C:\\1.21.1");
//	system(""C:\\App\\java\\jdk-21\\bin\\java.exe" -server -Xms1024M -Xmx4096M -jar fabric-server-launch.jar -nogui");
	system("java -server -Xms1024M -Xmx4096M -jar fabric-server-launch.jar -nogui");
	return 0;
}

int esd2 () {
	system("cls");
	printf("qwq\n");
	return 0;
}

int esd3 () {
	system("cls");
	printf("�˳���\n��Ҫ�������ȷ��");
	system("pause");
	system("exit");
	return 0;
}

int qwq () {
	printf("������ı�������Ŷ��qwq��\n");
	return esd2();
}

int main () {
	system("cls");
	int num;
	printf("����1��2��3ѡ����Ҫ���еĲ���\n");
	printf("��1��������������\n��2���ǡ�=���أ�=��\n��3�����˳��ó���\n");

//	system("pause");
	printf("������1��2��3��ִ�г��������\n");
	scanf("%d",&num);
	if (num > 0 )
	{
		if (num > 3 )
		{
			printf("Error\n");
			
			esd2();
			
		}
		if (num == 1 )
		{
			esd1();
		}
		if (num == 2 )
		{
			esd2();
		}
		if (num == 3 )
		{
			esd3();
			return 1;
		}
	}



	system("pause");
	return main ();
	system("pause");

}