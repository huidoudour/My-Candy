#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define DEFAULT_JAR "server.jar"
#define MIN_MEMORY "512M"
#define MAX_MEMORY "2048M"
#define BUFFER_SIZE 1024

typedef struct {
    PROCESS_INFORMATION pi;
    HANDLE hChildStd_IN_Rd;  // �ӽ��������ȡ��
    HANDLE hChildStd_IN_Wr;  // �ӽ�������д���
    HANDLE hChildStd_OUT_Rd; // �ӽ��������ȡ��
    HANDLE hChildStd_OUT_Wr; // �ӽ������д���
} ServerProcess;

static ServerProcess server = {0};

// �����ܵ��������ͨ��
bool create_pipes() {
    SECURITY_ATTRIBUTES sa = {
        sizeof(SECURITY_ATTRIBUTES),
        NULL,
        TRUE  // ����ɼ̳�
    };

    // �����ӽ�������ܵ�
    if (!CreatePipe(&server.hChildStd_IN_Rd, &server.hChildStd_IN_Wr, &sa, 0)) {
        return false;
    }

    // �����ӽ�������ܵ�
    if (!CreatePipe(&server.hChildStd_OUT_Rd, &server.hChildStd_OUT_Wr, &sa, 0)) {
        CloseHandle(server.hChildStd_IN_Rd);
        CloseHandle(server.hChildStd_IN_Wr);
        return false;
    }

    return true;
}

// �رչܵ�
void close_pipes() {
    if (server.hChildStd_IN_Rd) CloseHandle(server.hChildStd_IN_Rd);
    if (server.hChildStd_IN_Wr) CloseHandle(server.hChildStd_IN_Wr);
    if (server.hChildStd_OUT_Rd) CloseHandle(server.hChildStd_OUT_Rd);
    if (server.hChildStd_OUT_Wr) CloseHandle(server.hChildStd_OUT_Wr);

    server.hChildStd_IN_Rd = server.hChildStd_IN_Wr = NULL;
    server.hChildStd_OUT_Rd = server.hChildStd_OUT_Wr = NULL;
}

// ��������������
void start_server(const char *jar_file, const char *min_mem, const char *max_mem, bool nogui) {
    if (server.pi.hProcess) {
        printf("���������������� (PID: %d)\n", server.pi.dwProcessId);
        return;
    }

    char command[BUFFER_SIZE];
    snprintf(command, BUFFER_SIZE, "java -Xms%s -Xmx%s -jar %s %s",
             min_mem, max_mem, jar_file, nogui ? "nogui" : "");

    printf("��������: %s\n", command);

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    si.dwFlags = STARTF_USESTDHANDLES;

    // �����ܵ�
    if (!create_pipes()) {
        printf("�����ܵ�ʧ��! �������: %d\n", GetLastError());
        return;
    }

    si.hStdInput = server.hChildStd_IN_Rd;
    si.hStdOutput = server.hChildStd_OUT_Wr;
    si.hStdError = server.hChildStd_OUT_Wr;

    // ��������
    if (!CreateProcess(
        NULL,
        command,
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &server.pi))
    {
        printf("�޷�����������! �������: %d\n", GetLastError());
        close_pipes();
        return;
    }

    printf("������������ (PID: %d)\n", server.pi.dwProcessId);

    // �ر����ǲ�ʹ�õľ����
    CloseHandle(server.hChildStd_IN_Rd);
    CloseHandle(server.hChildStd_OUT_Wr);
    server.hChildStd_IN_Rd = server.hChildStd_OUT_Wr = NULL;
}

// ֹͣ����������
void stop_server() {
    if (!server.pi.hProcess) {
        printf("û�����еķ�����ʵ��\n");
        return;
    }

    printf("����ֹͣ������ (PID: %d)...\n", server.pi.dwProcessId);

    // �����������ֹͣ����
    DWORD bytesWritten;
    const char* stop_cmd = "stop\n";
    WriteFile(server.hChildStd_IN_Wr, stop_cmd, strlen(stop_cmd), &bytesWritten, NULL);

    // �ȴ������������ر�
    if (WaitForSingleObject(server.pi.hProcess, 10000) == WAIT_TIMEOUT) {
        printf("������δ��Ӧ��ǿ����ֹ...\n");
        TerminateProcess(server.pi.hProcess, 0);
    }

    // �رվ��
    CloseHandle(server.pi.hProcess);
    CloseHandle(server.pi.hThread);
    close_pipes();

    memset(&server, 0, sizeof(ServerProcess));
    printf("��������ֹͣ\n");
}

// ��������״̬
void check_server_status() {
    if (server.pi.hProcess) {
        DWORD exitCode;
        GetExitCodeProcess(server.pi.hProcess, &exitCode);

        if (exitCode == STILL_ACTIVE) {
            printf("�������������� (PID: %d)\n", server.pi.dwProcessId);

            // ��ȡ���������
            char buffer[BUFFER_SIZE];
            DWORD bytesRead;
            PeekNamedPipe(server.hChildStd_OUT_Rd, NULL, 0, NULL, &bytesRead, NULL);
            if (bytesRead > 0) {
                ReadFile(server.hChildStd_OUT_Rd, buffer, BUFFER_SIZE-1, &bytesRead, NULL);
                buffer[bytesRead] = '\0';
                printf("�������������:\n%s\n", buffer);
            }
        } else {
            printf("��������ֹͣ (�˳�����: %d)\n", exitCode);
            CloseHandle(server.pi.hProcess);
            CloseHandle(server.pi.hThread);
            close_pipes();
            memset(&server, 0, sizeof(ServerProcess));
        }
    } else {
        printf("������δ����\n");
    }
}

// ���������������
void send_server_command(const char* cmd) {
    if (!server.pi.hProcess) {
        printf("������δ���У��޷���������\n");
        return;
    }

    DWORD bytesWritten;
    char full_cmd[BUFFER_SIZE];
    snprintf(full_cmd, BUFFER_SIZE, "%s\n", cmd);

    if (!WriteFile(server.hChildStd_IN_Wr, full_cmd, strlen(full_cmd), &bytesWritten, NULL)) {
        printf("��������ʧ��! �������: %d\n", GetLastError());
    } else {
        printf("�ѷ�������: %s", full_cmd);
    }
}

// ��ʾ������Ϣ
void show_help() {
    printf("\nMinecraft ������������ (Windows �汾)\n");
    printf("��������:\n");
    printf("  start [jar�ļ�] [��С�ڴ�] [����ڴ�] - ����������\n");
    printf("  stop                                  - ֹͣ������\n");
    printf("  status                                - �鿴������״̬\n");
    printf("  cmd <����������>                      - ���������������\n");
    printf("  help                                  - ��ʾ������Ϣ\n");
    printf("  exit                                  - �˳�������\n");
    printf("\nĬ��ֵ:\n");
    printf("  jar�ļ�: %s\n", DEFAULT_JAR);
    printf("  ��С�ڴ�: %s\n", MIN_MEMORY);
    printf("  ����ڴ�: %s\n", MAX_MEMORY);
}

int main() {
    char input[BUFFER_SIZE];
    char *command;
    char *arg1, *arg2, *arg3;

    printf("Minecraft ������������ (Windows) - ���� 'help' ��ȡ����\n");

    while (1) {
        printf("> ");
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) break;
        input[strcspn(input, "\n")] = '\0'; // �Ƴ����з�

        command = strtok(input, " ");
        if (command == NULL) continue;

        if (strcmp(command, "start") == 0) {
            // ��������
            arg1 = strtok(NULL, " ");
            arg2 = strtok(NULL, " ");
            arg3 = strtok(NULL, " ");

            const char *jar = arg1 ? arg1 : DEFAULT_JAR;
            const char *min_mem = arg2 ? arg2 : MIN_MEMORY;
            const char *max_mem = arg3 ? arg3 : MAX_MEMORY;

            start_server(jar, min_mem, max_mem, true);
        }
        else if (strcmp(command, "stop") == 0) {
            stop_server();
        }
        else if (strcmp(command, "status") == 0) {
            check_server_status();
        }
        else if (strcmp(command, "cmd") == 0) {
            // ��ȡʣ�ಿ����Ϊ����
            char *server_cmd = strtok(NULL, "");
            if (server_cmd) {
                send_server_command(server_cmd);
            } else {
                printf("������Ҫ���͵ķ���������\n");
            }
        }
        else if (strcmp(command, "help") == 0) {
            show_help();
        }
        else if (strcmp(command, "exit") == 0) {
            if (server.pi.hProcess) {
                printf("����: ��������������! �Ƿ�Ҫֹͣ������? (y/n) ");
                fgets(input, BUFFER_SIZE, stdin);
                if (input[0] == 'y' || input[0] == 'Y') {
                    stop_server();
                }
            }
            printf("�˳�������\n");
            break;
        }
        else {
            printf("δ֪����: %s\n", command);
            printf("���� 'help' ��ȡ����\n");
        }
    }

    // ������Դ
    if (server.pi.hProcess) {
        stop_server();
    }

    return 0;
}