/*
	jwqe764241 작성
*/

#include <Sources/Utils/console.h>




console::ConsoleBuffer console::openConsole(void) {
	ConsoleBuffer fdBuff = new FILE*[3];

	if (AllocConsole()) {
		freopen_s(&fdBuff[0], "CONIN$", "rb", stdin);
		freopen_s(&fdBuff[1], "CONOUT$", "wb", stdout);
		freopen_s(&fdBuff[2], "CONOUT$", "wb", stderr);
	}

	return fdBuff;
}


int	console::closeConsole(
	_In_ ConsoleBuffer fdBuff
) {
	fclose(fdBuff[0]);
	fclose(fdBuff[1]);
	fclose(fdBuff[2]);

	return FreeConsole();
}


void console::changeColor(
	_In_ COLOR__ background,
	_In_ COLOR__ text
) {

	//TODO: 값 입력 제한둘것



	char command[10] = "Color ";

	strcat_s(command, sizeof(command), background);
	strcat_s(command, sizeof(command), text);

	system(command);
}




//TODO: 특정 공간에 윈도우 핸들값, 콘솔의 DC를 얻은 후
//TODO: pixelAt 다수콜 시 오버헤드 -> startDraw, endDraw로
LSTATUS console::Painter::startDraw(void) {
	m_hConWnd = ::GetConsoleWindow();
	m_hConDC  = ::GetDC(m_hConWnd);

	if (m_hConWnd == NULL) {
		return ERROR_CANNOT_GET_HWND;
	}

	if (m_hConDC == NULL) {
		return ERROR_CANNOT_GET_HDC;
	}

	return ERROR_OK;
}


LSTATUS console::Painter::endDraw(void) {
	ReleaseDC(m_hConWnd, m_hConDC);
	
	return ERROR_OK;
}


LSTATUS console::Painter::pixelAt(
	_In_ const int x,
	_In_ const int y,
	_In_ const COLORREF& color
) {
	return SetPixel(m_hConDC, x, y, color);
}


/*
//--Test
#ifndef NDEBUG
	void jwqe764241::console::pixel(
		_In_ const int x,
		_In_ const int y,
		_In_ const COLORREF& color
	) {
		HWND hWnd = ::GetConsoleWindow();
		HDC  hDC  = ::GetDC(hWnd);

		SetPixel(hDC, x, y, color);

		ReleaseDC(hWnd, hDC);
	}
#endif
//Test--
*/

LSTATUS console::Painter::drawBitmap(
	...
) {
	

	return NULL;
}
