#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

int InitApplication(HINSTANCE);
HWND InitWindow(HINSTANCE);

struct SCoordinate {
	int				x, y;
};

struct SCharacter {
	SCoordinate		Position;
};

struct SGame {
	SCharacter		Player;
};

int				updateGame(SGame& game) {
	++game.Player.Position.x;
	return 0;
}

const wchar_t*	g_ImageNames[] =
{	L"d:\\blah.bmp"
,	L"d:\\bleh.bmp"
,	L"d:\\blih.bmp"
,	L"d:\\bloh.bmp"
,	L"d:\\bluh.bmp"
};

struct SWinMainArgs {
	HINSTANCE	hInstance		;
	HINSTANCE	hPrevInstance	;
	LPSTR		lpCmdLine		; 
	int			nCmdShow		;
};

struct SApplication {
	SWinMainArgs	WinMainArgs	= {};
	HWND			hWnd		= NULL;
	int				ImageIndex	= 0;
	SGame			Game		= {};
};

SApplication		* g_App = nullptr;

VOID onPaint(HDC hdc)
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

	if(g_App) {
		SCoordinate			playerPos	= g_App ? g_App->Game.Player.Position : SCoordinate{400, 300};

		Gdiplus::Image		* image		= Gdiplus::Image::FromFile(g_ImageNames[g_App->ImageIndex], FALSE);
		int					offsetX		= playerPos.x;
		int					offsetY		= playerPos.y;
		int					width		= (int)image->GetWidth();
		int					height		= (int)image->GetHeight();
		Gdiplus::Rect		gdirect		= { offsetX - width / 2, offsetY - height / 2, width, height};
		graphics.DrawImage(image, gdirect);
	}
}

LRESULT CALLBACK mainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		onPaint(hdc);
		EndPaint(hWnd, &ps);
		return 0;
		//return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
int update(SApplication& application)
{
	printf("Updeteando\n");
	updateGame(application.Game);

	RECT				rect	= {};
	GetClientRect(application.hWnd, &rect);

	if (application.Game.Player.Position.x > rect.right - 100) {
		application.Game.Player.Position.x = 0;
		application.Game.Player.Position.y += 10;
		++application.ImageIndex;
		if (application.ImageIndex >= 3)
			application.ImageIndex = 0;
	}


	//UpdateWindow(hWnd);
	InvalidateRect(application.hWnd, NULL, TRUE);
	SendMessage(application.hWnd, WM_PAINT, NULL, NULL);

	MSG			msg			= {};
	int			gotMessage	= PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
	if (gotMessage) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(10);
	if (msg.message == WM_QUIT)
		return 1;

	return 0;
}

BOOL registerWindowClass(HINSTANCE hinstance)
{
	WNDCLASSEX wcx;

	// Fill in the window class structure with parameters that describe the main window. 
	wcx.cbSize			= sizeof(WNDCLASSEX);					// size of structure 
	wcx.style			= CS_HREDRAW | CS_VREDRAW;				// redraw if size changes 
	wcx.lpfnWndProc		= mainWndProc;							// points to window procedure 
	wcx.cbClsExtra		= 0;									// no extra class memory 
	wcx.cbWndExtra		= 0;									// no extra window memory 
	wcx.hInstance		= hinstance;							// handle to instance 
	wcx.hIcon			= LoadIcon(NULL, IDI_APPLICATION);		// predefined app. icon 
	wcx.hCursor			= LoadCursor(NULL, IDC_ARROW);			// predefined arrow 
	wcx.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	// white background brush 
	wcx.lpszMenuName	= "MainMenu";							// name of menu resource 
	wcx.lpszClassName	= "MainWClass";							// name of window class 
	wcx.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);			// small class icon 

	return RegisterClassEx(&wcx);		// Register the window class. 
}

int initWindow(SApplication& app) {
	if (!registerWindowClass(app.WinMainArgs.hInstance))
		return -1;

	app.hWnd					= CreateWindowEx(
		0,
		"MainWClass",        // name of window class 
		"Sample",            // title-bar string 
		WS_OVERLAPPEDWINDOW, // top-level window 
		CW_USEDEFAULT,       // default horizontal position 
		CW_USEDEFAULT,       // default vertical position 
		CW_USEDEFAULT,       // default width 
		CW_USEDEFAULT,       // default height 
		(HWND)NULL,         // no owner window 
		(HMENU)NULL,        // use class menu 
		app.WinMainArgs.hInstance,           // handle to application instance 
		(LPVOID)NULL);      // no window-creation data 

	if (!app.hWnd)
		return FALSE;

	// Show the window and send a WM_PAINT message to the window procedure. 
	ShowWindow(app.hWnd, app.WinMainArgs.nCmdShow);
	UpdateWindow(app.hWnd);
	return 0;
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	// Consola
	AllocConsole();
	FILE							* fp					= 0;
	freopen_s(&fp, "CONOUT$", "w+", stdout);

	// Graficos
	Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR						gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);	// Initialize GDI+.

	// Ventana
	SApplication					app;
	app.WinMainArgs.hInstance		= hinstance		;
	app.WinMainArgs.hPrevInstance	= hPrevInstance	;
	app.WinMainArgs.lpCmdLine		= lpCmdLine		;
	app.WinMainArgs.nCmdShow		= nCmdShow		;
	initWindow(app);

	g_App						= &app;
	app.Game.Player.Position	= { 10, 10 };
	
	bool							condition				= true;
	while (condition)
		condition					= (update(app) == 0);

	// Graficos
	Gdiplus::GdiplusShutdown(gdiplusToken);

	// consola
	FreeConsole();
	UNREFERENCED_PARAMETER(lpCmdLine);
	return 0;
}

