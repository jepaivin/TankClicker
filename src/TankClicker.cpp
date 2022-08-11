// TankClicker.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Game.h"
#include "View.h"
#include "TankClicker.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

Game GameInstance;
View ViewInstance;

float LastFPS = 0;

UINT_PTR TIMER_REPAINT = 1;
UINT_PTR TIMER_DEBUGTEXT = 2;

DWORD LastTickCount = GetTickCount();

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                TickApplication();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TANKCLICKER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TANKCLICKER));

    MSG msg;
    
    // Main message loop:
    while (true)
	{
        GameInstance.LockMutex();

        if (PeekMessage(&msg, nullptr, 0, 0, 0))
        {
            bool bOK = GetMessage(&msg, nullptr, 0, 0);
            if (!bOK)
            {
                break;
            }
            
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
            
        }
        
		TickApplication();

        GameInstance.ReleaseMutex();
    }
    
    GameInstance.Shutdown();

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TANKCLICKER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TANKCLICKER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
      
   GameInstance.Initialize();
   ViewInstance.Initialize();

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   ViewInstance.OnWindowChanged(hWnd);
   
   SetTimer(hWnd, TIMER_REPAINT, 10, (TIMERPROC)nullptr);
   SetTimer(hWnd, TIMER_DEBUGTEXT, 1000, (TIMERPROC)nullptr);

   return TRUE;
}

void TickApplication()
{
    DWORD Ticks = GetTickCount();
    
    DWORD DeltaTicks = Ticks - LastTickCount;

    GameInstance.Tick(DeltaTicks / 1000.0f);    
    ViewInstance.Draw(GameInstance);

	LastTickCount = Ticks;
    MainFrameCount++;
}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_LBUTTONDOWN:
	    {
            POINT Point;
            Point.x = GET_X_LPARAM(lParam);
            Point.y = GET_Y_LPARAM(lParam);
            GameInstance.OnMouseDown(Point, EMouseButton::Left);
	    }
	    break;
	case WM_LBUTTONUP:
	{
		POINT Point;
		Point.x = GET_X_LPARAM(lParam);
		Point.y = GET_Y_LPARAM(lParam);
		GameInstance.OnMouseUp(Point, EMouseButton::Left);
	}
	break;
     
     case WM_RBUTTONDOWN:
	     {
            POINT Point;
            Point.x = GET_X_LPARAM(lParam);
            Point.y = GET_Y_LPARAM(lParam);
            GameInstance.OnMouseDown(Point, EMouseButton::Right);
	     }
        break;

	 case WM_RBUTTONUP:
	 {
		 POINT Point;
		 Point.x = GET_X_LPARAM(lParam);
		 Point.y = GET_Y_LPARAM(lParam);
		 GameInstance.OnMouseUp(Point, EMouseButton::Right);
	 }

    case WM_TIMER:
        if (wParam == TIMER_REPAINT)
        {
            // Refresh screen
            InvalidateRect(hWnd, nullptr, false);
        }
        else if (wParam == TIMER_DEBUGTEXT)
        {
			char Title[256];
			sprintf_s<256>(Title, "%d / %d FPS", MainFrameCount, AIWorkerFrameCount);
            SetWindowText(hWnd, CA2CT(Title));
            
            AIWorkerFrameCount = 0;
            MainFrameCount = 0;
        }
        break;

    case WM_ERASEBKGND:
        return 1;
    
    case WM_SIZE:
        ViewInstance.OnWindowChanged(hWnd);
        break;

    case WM_PAINT:
        {            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            ViewInstance.SwapBuffers(hdc);            
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
