#include "FullScreen.h"
#include "stdafx.h"
#include "ScrnCapWnd.h"

#define FILE_MENU_FULLSCREEN 1
#define FILE_MENU_EXIT 2
#define FILE_MENU_ABOUT 3
#define FILE_MENU_SCRENCAPTURE 4
// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("ScreenCapture Tool");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HMENU hMenu;



void AddMenu(HWND);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 500,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


    switch (message)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case FILE_MENU_EXIT:
            DestroyWindow(hWnd);
            break;
        case FILE_MENU_ABOUT:
            MessageBox(NULL, L"Written 2022 By Hugo", L"About", MB_ICONINFORMATION);
            break;
        case FILE_MENU_FULLSCREEN:
            ShowWindow(hWnd, SW_HIDE);
            Sleep(300);
            PlaySound(TEXT("sound/shutter.wav"), NULL, SND_ASYNC);
            CaptureScreenToFile(L"C:/Users/hugow/Pictures/screen.png", L"image/png");
            ShowWindow(hWnd, SW_SHOW);
            break;
        case FILE_MENU_SCRENCAPTURE:
            ShowWindow(hWnd, SW_HIDE);
            Sleep(1000);
            CWndImpl::SetInstance(hInst);
            CScrnCapWnd* pScrnCap = new CScrnCapWnd;
            pScrnCap->Create(NULL, _T("ScrnCap"), WS_POPUP | WS_VISIBLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW, SCREEN_RC);
            UpdateWindow(pScrnCap->GetSafeHwnd());
            CWndImpl::MsgLoop();
            ShowWindow(hWnd, SW_SHOW);
            break;

        }
    case WM_CREATE:
        AddMenu(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}

void AddMenu(HWND hWnd)
{
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hSubMEnu = CreateMenu();

    AppendMenu(hSubMEnu, MF_STRING, FILE_MENU_FULLSCREEN, L"FullScreen");
    AppendMenu(hSubMEnu, MF_STRING, FILE_MENU_SCRENCAPTURE, L"ScreenShot");

    AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMEnu, L"Picture");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");


    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu(hMenu, MF_POPUP, FILE_MENU_ABOUT, L"About");

    SetMenu(hWnd, hMenu);
}