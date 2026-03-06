#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define ID_EDIT1 1
#define ID_EDIT2 2
#define ID_BTN_ADD 3
#define ID_BTN_SUB 4
#define ID_BTN_MUL 5
#define ID_BTN_DIV 6

HWND hEdit1, hEdit2;
HBRUSH hBrushGreen;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch(Message) {
        case WM_CREATE:

            CreateWindow("STATIC", "Please input two numbers",
                         WS_VISIBLE | WS_CHILD | SS_CENTER,
                         25, 15, 190, 20,
                         hwnd, NULL, NULL, NULL);

            hEdit1 = CreateWindow("EDIT", "",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
                                  50, 45, 140, 20,
                                  hwnd, (HMENU)ID_EDIT1, NULL, NULL);

            hEdit2 = CreateWindow("EDIT", "",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
                                  50, 75, 140, 20,
                                  hwnd, (HMENU)ID_EDIT2, NULL, NULL);

            CreateWindow("BUTTON", "+", WS_VISIBLE | WS_CHILD, 45, 110, 30, 25, hwnd, (HMENU)ID_BTN_ADD, NULL, NULL);
            CreateWindow("BUTTON", "-", WS_VISIBLE | WS_CHILD, 85, 110, 30, 25, hwnd, (HMENU)ID_BTN_SUB, NULL, NULL);
            CreateWindow("BUTTON", "*", WS_VISIBLE | WS_CHILD, 125, 110, 30, 25, hwnd, (HMENU)ID_BTN_MUL, NULL, NULL);
            CreateWindow("BUTTON", "/", WS_VISIBLE | WS_CHILD, 165, 110, 30, 25, hwnd, (HMENU)ID_BTN_DIV, NULL, NULL);
            break;

        case WM_CTLCOLORSTATIC: {

            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(255, 255, 255));
            SetBkMode(hdcStatic, TRANSPARENT);
            return (INT_PTR)hBrushGreen;
        }

        case WM_COMMAND: {

            if (HIWORD(wParam) == BN_CLICKED) {
                char buf1[100], buf2[100];

                GetWindowText(hEdit1, buf1, 100);
                GetWindowText(hEdit2, buf2, 100);

                double num1 = atof(buf1);
                double num2 = atof(buf2);
                double result = 0;


                int id = LOWORD(wParam);

                if (id == ID_BTN_ADD) {
                    result = num1 + num2;
                } else if (id == ID_BTN_SUB) {
                    result = num1 - num2;
                } else if (id == ID_BTN_MUL) {
                    result = num1 * num2;
                } else if (id == ID_BTN_DIV) {
                    if (num2 == 0) {
                        MessageBox(hwnd, "Cannot divide by zero!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    result = num1 / num2;
                } else {
                    break;
                }


                char t[100];
                sprintf(t, "%f", result);

                MessageBox(hwnd, t, "Result", MB_OK);
            }
            break;
        }

        case WM_DESTROY:

            DeleteObject(hBrushGreen);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;


    hBrushGreen = CreateSolidBrush(RGB(0, 128, 0));

    memset(&wc,0,sizeof(wc));
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = hBrushGreen;
    wc.lpszClassName = "WindowClass";
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

       hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "WindowClass",
        "My Calculator",
        WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        250, 200,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
