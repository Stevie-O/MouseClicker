// MouseJiggler.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MouseJiggler.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MOUSEJIGGLER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MOUSEJIGGLER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MOUSEJIGGLER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MOUSEJIGGLER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;
   RECT rcScreen, rcClient;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX),
      CW_USEDEFAULT, 100, CW_USEDEFAULT, 50, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   GetWindowRect(hWnd, &rcScreen);
   GetClientRect(hWnd, &rcClient);
   rcScreen.right += 200 - rcClient.right;
   rcScreen.bottom += 50 - rcClient.bottom;
   SetWindowPos(hWnd, NULL, 0, 0, rcScreen.right - rcScreen.left, rcScreen.bottom - rcScreen.top, SWP_NOMOVE|SWP_NOZORDER);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rc;
	HWND child;

	switch (message)
	{

	case WM_CREATE:
		GetClientRect(hWnd, &rc);
		child = CreateWindow(_T("BUTTON"), _T("Enabled"), WS_CHILD|BS_AUTOCHECKBOX|WS_VISIBLE|WS_TABSTOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hWnd, (HMENU)IDC_JIGGLE_ENABLED, NULL, NULL);
		SendDlgItemMessage(hWnd, IDC_JIGGLE_ENABLED, BM_SETCHECK, BST_CHECKED, 0);

		SetTimer(hWnd, 101, 1000, NULL);
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_SIZE:
		GetClientRect(hWnd, &rc);
		child = GetDlgItem(hWnd, IDC_JIGGLE_ENABLED);
		SetWindowPos(child, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_TIMER:
		if (wParam == 101) 
		{
			if (IsDlgButtonChecked(hWnd, IDC_JIGGLE_ENABLED)) 
			{
				// the following messes with menus
				//keybd_event(VK_F24, MapVirtualKey(VK_F24, MAPVK_VK_TO_VSC), 0, 0);
				//keybd_event(VK_F24, MapVirtualKey(VK_F24, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
				mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, 0);
				// SetTimer creates periodic timers
				// SetTimer(hWnd, 101, 1000, NULL);
			} else {
				// shut off the timer
				KillTimer(hWnd, wParam);
				OutputDebugString(_T("Deactivating timer\n"));
			}
			return 0;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDC_JIGGLE_ENABLED:
			if (wmEvent == BN_CLICKED && IsDlgButtonChecked(hWnd, IDC_JIGGLE_ENABLED)) {
				OutputDebugString(_T("Re-enabling timer\n"));
				SetTimer(hWnd, 101, 1000, NULL);
			}
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
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
