#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define ID_BUTTON_BUILD 1
#define ID_EDIT_SIZE 2
#define ID_EDIT_FILENAME 3
#define ID_EDIT_REPORT 4
//gcc -o dd_interface dd_interface.c -mwindows
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void build_file(const char* filename, int size_mb, HWND hwndReport);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "DDInterfaceClass";
    
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0)); // Fundo amarelo

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "DD Interface",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hEditSize, hEditFilename, hEditReport, hButtonBuild;

    switch (uMsg) {
    case WM_CREATE:
        hEditSize = CreateWindow("EDIT", "100",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            20, 20, 200, 25,
            hwnd, (HMENU)ID_EDIT_SIZE, NULL, NULL);

        hEditFilename = CreateWindow("EDIT", "output.img",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            20, 60, 200, 25,
            hwnd, (HMENU)ID_EDIT_FILENAME, NULL, NULL);

        hEditReport = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
            20, 100, 440, 140,
            hwnd, (HMENU)ID_EDIT_REPORT, NULL, NULL);

        hButtonBuild = CreateWindow("BUTTON", "Build",
            WS_CHILD | WS_VISIBLE,
            250, 20, 100, 65,
            hwnd, (HMENU)ID_BUTTON_BUILD, NULL, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON_BUILD) {
            char sizeStr[256], filename[256];
            GetWindowText(hEditSize, sizeStr, sizeof(sizeStr));
            GetWindowText(hEditFilename, filename, sizeof(filename));

            int size_mb = atoi(sizeStr);
            if (size_mb <= 0) {
                MessageBox(hwnd, "Please enter a valid size in megabytes.", "Error", MB_OK | MB_ICONERROR);
            } else {
                build_file(filename, size_mb, hEditReport);
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void build_file(const char* filename, int size_mb, HWND hwndReport) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        MessageBox(NULL, "Failed to create file.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    char* buffer = (char*)calloc(1, 1024 * 1024); // 1MB buffer filled with zeros
    if (!buffer) {
        MessageBox(NULL, "Failed to allocate memory.", "Error", MB_OK | MB_ICONERROR);
        fclose(file);
        return;
    }

    for (int i = 0; i < size_mb; i++) {
        fwrite(buffer, 1, 1024 * 1024, file);

        char report[256];
        snprintf(report, sizeof(report), "Written %d MB of %d MB", i + 1, size_mb);
        SetWindowText(hwndReport, report);
    }

    free(buffer);
    fclose(file);

    MessageBox(NULL, "File created successfully.", "Success", MB_OK | MB_ICONINFORMATION);
}

