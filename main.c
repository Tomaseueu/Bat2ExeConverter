
#include <windows.h>
#include <commdlg.h>

#define IDC_BTN_BROWSE 101
#define IDC_BTN_ICON   102
#define IDC_BTN_BUILD  103
#define IDC_CHK_ADMIN  104

char batFilePath[MAX_PATH] = "";
char iconFilePath[MAX_PATH] = "";
BOOL runAsAdmin = FALSE;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void BrowseForFile(HWND hwnd, char *pathBuffer, const char *filter) {
    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = pathBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = filter;
    ofn.Flags = OFN_FILEMUSTEXIST;
    GetOpenFileName(&ofn);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "Bat2ExeWin";

    RegisterClass(&wc);

    HWND hwnd = CreateWindow("Bat2ExeWin", "Bat to Exe Converter", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

void GenerateEXE(HWND hwnd) {
    MessageBox(hwnd, "Tu bude logika pre vytvorenie EXE súboru z .bat", "TODO", MB_OK);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            CreateWindow("BUTTON", "Select .bat", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                20, 20, 120, 30, hwnd, (HMENU)IDC_BTN_BROWSE, NULL, NULL);

            CreateWindow("BUTTON", "Select Icon", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                20, 60, 120, 30, hwnd, (HMENU)IDC_BTN_ICON, NULL, NULL);

            CreateWindow("BUTTON", "Create .exe", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                20, 140, 120, 30, hwnd, (HMENU)IDC_BTN_BUILD, NULL, NULL);

            CreateWindow("BUTTON", "Administrator needed", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                20, 100, 200, 20, hwnd, (HMENU)IDC_CHK_ADMIN, NULL, NULL);
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDC_BTN_BROWSE:
                    BrowseForFile(hwnd, batFilePath, "Batch Files\0*.bat\0");
                    break;
                case IDC_BTN_ICON:
                    BrowseForFile(hwnd, iconFilePath, "Icons\0*.ico\0");
                    break;
                case IDC_CHK_ADMIN:
                    runAsAdmin = !runAsAdmin;
                    break;
                case IDC_BTN_BUILD:
                    GenerateEXE(hwnd);
                    break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
