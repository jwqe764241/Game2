#ifndef __GAMEWINDOW_H
#define __GAMEWINDOW_H

#include <Windows.h>

typedef struct _WINDOWSIZE 
{
	int width;
	int height;
} WINDOWSIZE, * LPWINDOWSIZE;


class CGameWindow
{
private:
	//--Window Val
	HWND		m_hWnd;
	HINSTANCE	m_hInstance;
	wchar_t *	m_pstrFrameTitle;
	wchar_t *	m_pstrWndClassName;
	int			m_inCmdShow;
	WINDOWSIZE  m_sizeWindow;
	//Window Val
private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT umessage, WPARAM wParam, LPARAM lParam);

public:
	void StartWindow();
	int  GetWidth();
	int  GetHeight();
	HWND GetHwnd();
public:
	CGameWindow() {};
	CGameWindow(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClass, int nCmdShow, int width, int height);
	~CGameWindow();
};

#endif