#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include<tchar.h>
#include <windows.h>
#include "Game.h"
#include <math.h>
#include<iostream>
#include<commctrl.h>

#define WIDTH 800
#define HEIGHT 600

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

HWND hTrack, hMain;

int AddControls(HWND);

int tickRate = 10;

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("BIGCHUNGUS");

int xPos = 0, yPos = 0;

Game* g;

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;             /* This is the handle for our window */
    MSG message;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = (HICON) LoadImageW (NULL, L"res/mc.ico", IMAGE_ICON, 48, 48, LR_LOADFROMFILE);
    wincl.hIconSm = (HICON) LoadImageW (NULL, L"res/mc.ico", IMAGE_ICON, 48, 48, LR_LOADFROMFILE);
    wincl.hCursor = LoadCursor (NULL, IDC_CROSS);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) BLACK_BRUSH;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Big Chungus 3: Revenge of the son"),       /* Title Text */
               WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,   /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               WIDTH,                 /* The programs width */
               HEIGHT,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    hMain = hwnd;

    int xPos = (GetSystemMetrics(SM_CXSCREEN) - WIDTH)/2;
    int yPos = (GetSystemMetrics(SM_CYSCREEN) - HEIGHT)/2;
    SetWindowPos( hwnd, 0, xPos, yPos, WIDTH, HEIGHT, SWP_NOZORDER | SWP_NOSIZE );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    g = new Game(hwnd, WIDTH - 5, HEIGHT - 30, 0, 0);
    g->setState(Game::GAME_STATE::RUNNING);

    DWORD last = GetTickCount();
    DWORD current;

    while (g->getState() == Game::GAME_STATE::RUNNING)
    {

        if (PeekMessage(&message, hwnd, 0, 0, PM_REMOVE))
            DispatchMessage(&message);

        current = GetTickCount();

        if (current - last >= (unsigned int) tickRate)
        {

            g->update(false);
            last = current;

        }

        else Sleep(current - last);

    }

    delete g;
    return 0;

}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)                  /* handle the messages */
    {
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;

    case WM_KEYDOWN:
        g->handleInput(wParam, lParam);
        break;

    case WM_CLOSE:
        if (MessageBoxW(hwnd, L"Really quit?", L"Quit Game", MB_OKCANCEL) == IDOK)
        {
            g->setState(Game::GAME_STATE::STOPPED);
        }
        return 0;
        break;

    default:                   /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

int AddControls(HWND hParent)
{

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC  = ICC_LISTVIEW_CLASSES;
    if (!InitCommonControlsEx(&icex)) return -1;
    hTrack = CreateWindowW(TRACKBAR_CLASSW, L"Track", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 30, 500, 100, 20, hParent, (HMENU) 3, NULL, NULL);

    SendMessage(hTrack, TBM_SETRANGE, TRUE, MAKELONG(20, 90));
    SendMessageW(hTrack, TBM_SETPAGESIZE, 0,  10);
    SendMessageW(hTrack, TBM_SETTICFREQ, 10, 0);
    SendMessageW(hTrack, TBM_SETPOS, FALSE, 45);

    return 0;
}
