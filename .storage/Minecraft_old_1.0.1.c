#define _CRT_SECURE_NO_WARNING
#include <stdio.h>
#include <stdlib.h>
int esd1 () {
	system("cls");
	printf("即将启动\n");
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
	printf("退出？\n需要按任意键确认");
	system("pause");
	system("exit");
	return 0;
}

int qwq () {
	printf("输入的文本有问题哦‘qwq’\n");
	return esd2();
}

int main () {
	system("cls");
	int num;
	printf("输入1、2、3选择需要进行的操作\n");
	printf("“1”是启动服务器\n“2”是“=￣ω￣=”\n“3”是退出该程序\n");

//	system("pause");
	printf("请输入1、2、3以执行程序分命令\n");
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