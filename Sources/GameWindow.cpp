/*
	사용안함
	GameApp 클래스로 병합됨
*/

//#include <Sources/GameWindow.h>
//
//CGameWindow::CGameWindow(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClassName, int nCmdShow, int width, int height)
//	:m_hWnd(NULL), m_hInstance(NULL), m_pstrFrameTitle(frameTitle), m_pstrWndClassName(wndClassName),
//	m_inCmdShow(nCmdShow), m_sizeWindow(WindowSize{width, height})
//{
//	WNDCLASSEX wndClass;	
//	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
//	wndClass.cbSize = sizeof(WNDCLASSEX);
//	wndClass.style = CS_HREDRAW | CS_VREDRAW;
//	wndClass.lpfnWndProc = CGameWindow::MainProc;
//	wndClass.cbClsExtra = 0;
//	wndClass.cbClsExtra = sizeof(CGameWindow*);
//	wndClass.hInstance = m_hInstance;
//	wndClass.lpszMenuName = nullptr;
//	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
//	wndClass.lpszClassName = m_pstrWndClassName;
//	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//	RegisterClassEx(&wndClass);
//
//	if (width == 0 || height == 0) {
//		m_sizeWindow = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
//	}
//
//	m_hWnd = CreateWindowEx(
//		WS_EX_APPWINDOW,
//		m_pstrWndClassName,
//		m_pstrFrameTitle,
//		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
//		CW_USEDEFAULT,
//		CW_USEDEFAULT,
//		m_sizeWindow.width,
//		m_sizeWindow.height,
//		NULL,
//		NULL,
//		m_hInstance,
//		this);
//}
//
//CGameWindow::~CGameWindow()
//{
//	CloseWindow(m_hWnd);
//}
//
//
//void CGameWindow::StartWindow() 
//{
//	ShowWindow(m_hWnd, SW_SHOW);
//	SetForegroundWindow(m_hWnd);
//	SetFocus(m_hWnd);
//	ShowCursor(false);
//	UpdateWindow(m_hWnd);
//}
//
//int CGameWindow::GetWidth() 
//{
//	return m_sizeWindow.width;
//}
//
//int CGameWindow::GetHeight() 
//{
//	return m_sizeWindow.height;
//}
//
//HWND CGameWindow::GetHwnd()
//{
//	return m_hWnd;
//}
//
//
//LRESULT CALLBACK CGameWindow::MainProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
//{
//	switch (umessage){
//		case WM_LBUTTONUP:
//			PostQuitMessage(0);
//			//MessageBox(hwnd, L"dsfdsf", L"dsfdsf", MB_OK);
//			break;
//	}
//
//	return DefWindowProc(hwnd, umessage, wparam, lparam);
//}
////temp--