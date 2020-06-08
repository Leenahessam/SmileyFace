// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WindowsProject1.h"
#include <tchar.h>
#include <windows.h>
#include <math.h>
#include <algorithm>
#define MAX_LOADSTRING 100

using namespace std;
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int numPoints = 0;
int Xc1, Yc1, Xc2, Yc2, Xc3, Yc3, Xp1, Yp1, Xp2, Yp2, Xp3, Yp3, r1, r2, r3, Xp4, Yp4, Xp5, Yp5, Xp6, Yp6, Xp7, Yp7;
int xls1, yls1, xle1, yle1, xls2, yls2, xle2, yle2, xls3, yls3, xle3, yle3;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
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

COLORREF SetColor(int x, int y)
{
	if (numPoints == 1)
		return RGB(0, 0, 0);
	int distance = (int)sqrt(pow((x - Xc1), 2) + pow((y - Yc1), 2));
	if (distance < r1)
		return RGB(0, 0, 255);
	return RGB(255, 0, 0);
}
float getPt(int p1, int p2, float prec)
{
	int diff = p2 - p1;
	return p1 + (diff * prec);
}

void QuadCurve(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3) {
	for (float i = 0; i < 1; i += 0.0001)
	{
		float xa = getPt(x1, x2, i);
	    float ya = getPt(y1, y2, i);
		float xb = getPt(x2, x3, i);
		float yb = getPt(y2, y3, i);
		int x =  getPt(xa, xb, i);
		int y =  getPt(ya, yb, i);
		SetPixel(hdc,x, y, SetColor(x,y));
	}
}
void DrawLine(HDC hdc, int xs, int ys, int xe, int ye)
{
	int dx = xe - xs;
	int dy = ye - ys;
	  
	if (abs(dy) <= abs(dx))
	{
		double slope = (double)dy / dx;
		if (xs > xe)
		{
			swap(xs, xe);
			swap(ys, ye);
		}
		for (int x = xs; x <= xe; x++)
		{
			int y = round(ys + (x - xs)*slope);
			SetPixel(hdc, x, y, SetColor(x,y));
		}
	}
	else
	{
		double islope = (double)dx / dy;
		if (ys > ye)
		{
			swap(xs, xe);
			swap(ys, ye);
		}
		for (int y = ys; y <= ye; y++)
		{
			int x = round(xs + (y - ys)*islope);
			SetPixel(hdc, x, y, SetColor(x,y));
		}
	}
}
void Draw8Points(HDC hdc, int xc, int yc, int a, int b)
{
	SetPixel(hdc, xc + a, yc + b, SetColor(xc + a, yc + b));
	SetPixel(hdc, xc - a, yc + b, SetColor(xc - a, yc + b));
	SetPixel(hdc, xc - a, yc - b, SetColor(xc - a, yc - b));
	SetPixel(hdc, xc + a, yc - b, SetColor(xc + a, yc - b));
	SetPixel(hdc, xc + b, yc + a, SetColor(xc + b, yc + a));
	SetPixel(hdc, xc - b, yc + a, SetColor(xc - b, yc + a));
	SetPixel(hdc, xc - b, yc - a, SetColor(xc - b, yc - a));
	SetPixel(hdc, xc + b, yc - a, SetColor(xc + b, yc - a));
}

void CircleBresenham(HDC hdc, int xc, int yc, int R)
{
	int x = 0, y = R;
	int d = 1 - R;
	Draw8Points(hdc, xc, yc, x, y);
	while (x < y)
	{
		if (d < 0)
			d += 2 * x + 2;
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		Draw8Points(hdc, xc, yc, x, y);
	}
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = GetDC(hWnd);
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_LBUTTONUP:
		if (numPoints == 0) 
		{
			Xc1 = LOWORD(lParam);
			Yc1 = HIWORD(lParam);
			numPoints++;
		}
		else if (numPoints == 1)
		{
			Xp1 = LOWORD(lParam);
			Yp1 = HIWORD(lParam);
			r1 = (int)sqrt(pow((Xc1 - Xp1), 2) + pow((Yc1 - Yp1), 2));
			CircleBresenham(hdc, Xc1, Yc1, r1);
			numPoints++;
		}
		else if (numPoints == 2)
		{
			Xc2 = LOWORD(lParam);
			Yc2 = HIWORD(lParam);
			numPoints++;
		}
		else if (numPoints == 3)
		{
			Xp2 = LOWORD(lParam);
			Yp2 = HIWORD(lParam);
			r2 = (int)sqrt(pow((Xc2 - Xp2), 2) + pow((Yc2 - Yp2), 2));
			CircleBresenham(hdc, Xc2, Yc2, r2);
			numPoints++;
		}
		else if (numPoints == 4)
		{
			Xc3 = LOWORD(lParam);
			Yc3 = HIWORD(lParam);
			numPoints++;
		}
		else if (numPoints == 5)
		{
			Xp3 = LOWORD(lParam);
			Yp3 = HIWORD(lParam);
			r3 = (int)sqrt(pow((Xc3 - Xp3), 2) + pow((Yc3 - Yp3), 2));
			CircleBresenham(hdc, Xc3, Yc3, r3);
			numPoints++;
		}
		else if (numPoints == 6)
		{
			Xp4 = LOWORD(lParam);
			Yp4 = HIWORD(lParam);
			numPoints++;
		}
		else if (numPoints == 7)
		{
			Xp5 = LOWORD(lParam);
			Yp5 = HIWORD(lParam);
			numPoints++;
		}
		else if (numPoints == 8)
		{
			Xp6 = LOWORD(lParam);
			Yp6 = HIWORD(lParam);
			QuadCurve(hdc, Xp4, Yp4, Xp5, Yp5, Xp6, Yp6);
			numPoints++;
		}
		else if (numPoints == 9)
		{
			xls1 = LOWORD(lParam);
			yls1 = HIWORD(lParam);
			numPoints++;
		}
		else if (numPoints == 10)
		{
			xle1 = LOWORD(lParam);
			yle1 = HIWORD(lParam);
			DrawLine(hdc, xls1, yls1, xle1, yle1);
			numPoints++;
		}
		else if (numPoints == 11)
		{
			xls2 = LOWORD(lParam);
			yls2 = HIWORD(lParam);
			numPoints++;
		}
		else if (numPoints == 12)
		{
			xle2 = LOWORD(lParam);
			yle2 = HIWORD(lParam);
			DrawLine(hdc, xls2, yls2, xle2, yle2);
			numPoints++;
		}
		else if (numPoints == 13)
		{
			xls3 = LOWORD(lParam);
			yls3 = HIWORD(lParam);
			numPoints++;
		}
		else if (numPoints == 14)
		{
			xle3 = LOWORD(lParam);
			yle3 = HIWORD(lParam);
			DrawLine(hdc, xls3, yls3, xle3, yle3);
			numPoints = 0;
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
