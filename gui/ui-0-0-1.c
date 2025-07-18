#define UNICODE
#define _UNICODE
#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comdlg32.lib")

#define DEFAULT_JAR L"server.jar"
#define MIN_MEMORY L"512M"
#define MAX_MEMORY L"2048M"
#define ID_JAR_BROWSE 101
#define ID_START 102
#define ID_STOP 103
#define ID_MEM_MIN 104
#define ID_MEM_MAX 105
#define ID_STATUS 106
#define ID_CONSOLE 107
#define ID_CMD_INPUT 108
#define ID_CMD_SEND 109

typedef struct {
    PROCESS_INFORMATION pi;
    HANDLE hInputWrite;
    HANDLE hOutputRead;
    wchar_t jarPath[MAX_PATH];
    wchar_t minMem[32];
    wchar_t maxMem[32];
    HWND hWnd;
    HWND hConsole;
    HFONT hFont;
    bool running;
    bool isChoosingFile;  // 新增：防止重复选择标志
} ServerData;

ServerData server = {0};

// 修复的JAR文件选择函数
void BrowseJarFile(HWND hWnd) {
    if (server.isChoosingFile) return;  // 防止重复调用
    
    server.isChoosingFile = true;
    
    OPENFILENAME ofn = {0};
    wchar_t szFile[MAX_PATH] = {0};
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"JAR Files\0*.jar\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn)) {
        wcscpy(server.jarPath, szFile);
        SetDlgItemText(hWnd, ID_JAR_BROWSE, szFile);
    }
    
    server.isChoosingFile = false;  // 重置标志
}

DWORD WINAPI ReadOutputThread(LPVOID lpParam) {
    char buffer[4096];
    DWORD bytesRead;
    while (ReadFile(server.hOutputRead, buffer, sizeof(buffer), &bytesRead, NULL)) {
        buffer[bytesRead] = '\0';
        wchar_t wBuffer[4096];
        MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wBuffer, 4096);
        SendMessage(server.hConsole, EM_SETSEL, -1, -1);
        SendMessage(server.hConsole, EM_REPLACESEL, 0, (LPARAM)wBuffer);
    }
    return 0;
}

void StartServer(HWND hWnd) {
    if (server.running) {
        MessageBox(hWnd, L"服务器已在运行中!", L"提示", MB_OK | MB_ICONINFORMATION);
        return;
    }

    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    HANDLE hInputRead, hInputWrite;
    HANDLE hOutputRead, hOutputWrite;
    
    if (!CreatePipe(&hInputRead, &hInputWrite, &sa, 0) || 
        !CreatePipe(&hOutputRead, &hOutputWrite, &sa, 0)) {
        MessageBox(hWnd, L"无法创建管道!", L"错误", MB_OK | MB_ICONERROR);
        return;
    }

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.hStdInput = hInputRead;
    si.hStdOutput = hOutputWrite;
    si.hStdError = hOutputWrite;
    si.wShowWindow = SW_HIDE;

    wchar_t command[1024];
    _snwprintf(command, 1024, L"java -Xms%s -Xmx%s -jar \"%s\" nogui",
             server.minMem, server.maxMem, 
             wcslen(server.jarPath) ? server.jarPath : DEFAULT_JAR);

    PROCESS_INFORMATION pi;
    if (CreateProcess(NULL, command, NULL, NULL, TRUE, 
                     CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        server.pi = pi;
        server.running = true;
        server.hInputWrite = hInputWrite;
        server.hOutputRead = hOutputRead;
        CloseHandle(hInputRead);
        CloseHandle(hOutputWrite);
        CreateThread(NULL, 0, ReadOutputThread, NULL, 0, NULL);
        SetDlgItemText(hWnd, ID_STATUS, L"状态: 运行中");
    } else {
        CloseHandle(hInputRead);
        CloseHandle(hInputWrite);
        CloseHandle(hOutputRead);
        CloseHandle(hOutputWrite);
        MessageBox(hWnd, L"启动服务器失败!", L"错误", MB_OK | MB_ICONERROR);
    }
}

void StopServer(HWND hWnd) {
    if (!server.running) {
        MessageBox(hWnd, L"没有运行的服务器实例!", L"提示", MB_OK | MB_ICONINFORMATION);
        return;
    }

    DWORD bytesWritten;
    WriteFile(server.hInputWrite, "stop\n", 5, &bytesWritten, NULL);
    Sleep(1000);
    TerminateProcess(server.pi.hProcess, 0);
    CloseHandle(server.pi.hProcess);
    CloseHandle(server.pi.hThread);
    CloseHandle(server.hInputWrite);
    CloseHandle(server.hOutputRead);
    server.running = false;
    SetDlgItemText(hWnd, ID_STATUS, L"状态: 已停止");
}

// 优化后的命令发送函数
void SendCommandToServer(HWND hWnd) {
    if (!server.running) {
        MessageBox(hWnd, L"服务器未运行!", L"错误", MB_OK | MB_ICONERROR);
        return;
    }

    wchar_t cmd[256];
    GetDlgItemText(hWnd, ID_CMD_INPUT, cmd, 256);
    if (wcslen(cmd) == 0) return;

    // 自动添加/前缀
    if (cmd[0] != L'/') {
        wchar_t temp[256];
        swprintf(temp, 256, L"/%s", cmd);
        wcscpy(cmd, temp);
    }

    // 转换为UTF-8
    char utf8Cmd[512];
    WideCharToMultiByte(CP_UTF8, 0, cmd, -1, utf8Cmd, 512, NULL, NULL);

    DWORD bytesWritten;
    WriteFile(server.hInputWrite, utf8Cmd, strlen(utf8Cmd), &bytesWritten, NULL);
    WriteFile(server.hInputWrite, "\n", 1, &bytesWritten, NULL);
    SetDlgItemText(hWnd, ID_CMD_INPUT, L"");
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
            server.hWnd = hWnd;
            server.hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"微软雅黑");
            
            CreateWindow(L"STATIC", L"JAR 文件:", WS_VISIBLE | WS_CHILD, 10, 10, 80, 25, hWnd, NULL, NULL, NULL);
            CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 10, 250, 25, hWnd, (HMENU)ID_JAR_BROWSE, NULL, NULL);
            CreateWindow(L"BUTTON", L"浏览...", WS_VISIBLE | WS_CHILD, 360, 10, 80, 25, hWnd, (HMENU)ID_JAR_BROWSE, NULL, NULL);
            
            CreateWindow(L"STATIC", L"最小内存:", WS_VISIBLE | WS_CHILD, 10, 45, 80, 25, hWnd, NULL, NULL, NULL);
            CreateWindow(L"EDIT", MIN_MEMORY, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 45, 100, 25, hWnd, (HMENU)ID_MEM_MIN, NULL, NULL);
            
            CreateWindow(L"STATIC", L"最大内存:", WS_VISIBLE | WS_CHILD, 210, 45, 80, 25, hWnd, NULL, NULL, NULL);
            CreateWindow(L"EDIT", MAX_MEMORY, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 300, 45, 100, 25, hWnd, (HMENU)ID_MEM_MAX, NULL, NULL);
            
            CreateWindow(L"BUTTON", L"启动服务器", WS_VISIBLE | WS_CHILD, 10, 80, 120, 30, hWnd, (HMENU)ID_START, NULL, NULL);
            CreateWindow(L"BUTTON", L"停止服务器", WS_VISIBLE | WS_CHILD, 140, 80, 120, 30, hWnd, (HMENU)ID_STOP, NULL, NULL);
            
            CreateWindow(L"STATIC", L"状态: 已停止", WS_VISIBLE | WS_CHILD, 270, 85, 200, 25, hWnd, (HMENU)ID_STATUS, NULL, NULL);
            
            server.hConsole = CreateWindow(L"EDIT", L"", 
                WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                10, 120, 430, 200, hWnd, (HMENU)ID_CONSOLE, NULL, NULL);
            
            CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 330, 300, 25, hWnd, (HMENU)ID_CMD_INPUT, NULL, NULL);
            CreateWindow(L"BUTTON", L"发送命令", WS_VISIBLE | WS_CHILD, 320, 330, 100, 25, hWnd, (HMENU)ID_CMD_SEND, NULL, NULL);
            
            HWND hChild = GetWindow(hWnd, GW_CHILD);
            while (hChild) {
                SendMessage(hChild, WM_SETFONT, (WPARAM)server.hFont, TRUE);
                hChild = GetWindow(hChild, GW_HWNDNEXT);
            }
            break;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case ID_JAR_BROWSE: BrowseJarFile(hWnd); break;
                case ID_START: StartServer(hWnd); break;
                case ID_STOP: StopServer(hWnd); break;
                case ID_CMD_SEND: SendCommandToServer(hWnd); break;
            }
            break;
        }
        case WM_DESTROY:
            if (server.running) StopServer(hWnd);
            if (server.hFont) DeleteObject(server.hFont);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    wcscpy(server.minMem, MIN_MEMORY);
    wcscpy(server.maxMem, MAX_MEMORY);
    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MinecraftServerManager";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    if (!RegisterClass(&wc)) return 1;

    HWND hWnd = CreateWindow(wc.lpszClassName, L"Minecraft 服务器管理器",
                            WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
                            CW_USEDEFAULT, CW_USEDEFAULT, 470, 400, 
                            NULL, NULL, hInstance, NULL);
    if (!hWnd) return 1;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}