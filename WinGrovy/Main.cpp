#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

int InitApplication(HINSTANCE);
HWND InitWindow(HINSTANCE);

VOID OnPaint(HDC hdc)
{
	LPCTSTR				lpchText	= "Texto";
	int					nCount		= strlen(lpchText);
	RECT				rect		= { 10, 10, 210, 110 };
	UINT				uFormat		= 0;
	DrawText(hdc, lpchText, nCount, &rect, uFormat);

	Gdiplus::Graphics	graphics	(hdc);
	Gdiplus::Pen		pen			(Gdiplus::Color(255, 1, 1, 1));
	if( Gdiplus::Status::Ok != graphics.DrawLine(&pen, 1, 1, 200, 100) )
		printf("error!");

	Gdiplus::Point		points[4]	= {{0,0},{50,30},{30,60},{7,8}};
	int					count		= 3;
	graphics.DrawPolygon(&pen, points, count);

	Gdiplus::Image*		image		= Gdiplus::Image::FromFile(L"d:\\bleh.bmp", FALSE);
	int					offsetX		= 400;
	int					offsetY		= 300;
	int					width		= (int)image->GetWidth();
	int					height		= (int)image->GetHeight();
	Gdiplus::Rect		gdirect		= { offsetX - width / 2, offsetY - height / 2, width, height};
	graphics.DrawImage(image, gdirect);
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
int update(HWND hWnd)
{
	printf("Updeteando\n");
	MSG			msg			= {};
	int			gotMessage	= GetMessage(&msg, hWnd, 0, 0);
	if (gotMessage == -1)
		return -1;

	if (gotMessage != 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	else
		return 1;

	return 0;
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	// Consola
	AllocConsole();
	FILE* fp = 0;
	freopen_s(&fp, "CONOUT$", "w+", stdout);

	// Graficos
	Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR						gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);	// Initialize GDI+.

	// Ventana
	if (!InitApplication(hinstance))
		return FALSE;

	HWND hWnd = InitWindow(hinstance);
	if (!hWnd)
		return FALSE;

	// Show the window and send a WM_PAINT message to the window procedure. 
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	bool condition = true;
	while (condition) {
		condition = (update(hWnd) == 0);
	}

	// Graficos
	Gdiplus::GdiplusShutdown(gdiplusToken);

	// consola
	FreeConsole();
	UNREFERENCED_PARAMETER(lpCmdLine);
	return 0;
}

BOOL InitApplication(HINSTANCE hinstance) 
{ 
    WNDCLASSEX wcx; 
 
    // Fill in the window class structure with parameters that describe the main window. 
    wcx.cbSize			= sizeof(WNDCLASSEX);					// size of structure 
    wcx.style			= CS_HREDRAW | CS_VREDRAW;				// redraw if size changes 
    wcx.lpfnWndProc		= MainWndProc;							// points to window procedure 
    wcx.cbClsExtra		= 0;									// no extra class memory 
    wcx.cbWndExtra		= 0;									// no extra window memory 
    wcx.hInstance		= hinstance;							// handle to instance 
    wcx.hIcon			= LoadIcon(NULL,IDI_APPLICATION);		// predefined app. icon 
    wcx.hCursor			= LoadCursor(NULL,IDC_ARROW);			// predefined arrow 
    wcx.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	// white background brush 
    wcx.lpszMenuName	= "MainMenu";							// name of menu resource 
    wcx.lpszClassName	= "MainWClass";							// name of window class 
    wcx.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);			// small class icon 

	return RegisterClassEx(&wcx);  // Register the window class. 

} 
 
HWND InitWindow(HINSTANCE hinstance)
{ 
    HWND hwnd; 
 
	// Create the main window. 
	hwnd = CreateWindowEx(
		0,
        "MainWClass",        // name of window class 
        "Sample",            // title-bar string 
        WS_OVERLAPPEDWINDOW, // top-level window 
        CW_USEDEFAULT,       // default horizontal position 
        CW_USEDEFAULT,       // default vertical position 
        CW_USEDEFAULT,       // default width 
        CW_USEDEFAULT,       // default height 
        (HWND) NULL,         // no owner window 
        (HMENU) NULL,        // use class menu 
        hinstance,           // handle to application instance 
        (LPVOID) NULL);      // no window-creation data 
 
    if (!hwnd) 
        return NULL; 
 
    return hwnd;
 
} 