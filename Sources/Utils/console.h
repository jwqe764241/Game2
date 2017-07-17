/*
	jwqe764241 작성
*/

#ifndef __CONSOLE_H
#define __CONSOLE_H


#include <Sources/GameApp.h>
#include <string>


namespace console {

	//--typedef
	typedef FILE**				ConsoleBuffer;
	typedef const char* const	COLOR__;
	//typedef--
	
	ConsoleBuffer openConsole		(void);


	int		closeConsole	(
		_In_ ConsoleBuffer fdBuff
	);


	void	changeColor		(
		_In_ COLOR__ background,
		_In_ COLOR__ text
	);

	/*
	#ifndef NDEBUG
	void	pixel(
		_In_ const int x,
		_In_ const int y,
		_In_ const COLORREF& color
	);
	#endif
	*/

	class Painter {
		private:
			HWND m_hConWnd = NULL;
			HDC  m_hConDC  = NULL;

		public:
			LSTATUS startDraw(void);
			LSTATUS endDraw(void);
			LSTATUS	pixelAt(
				_In_ const int x,
				_In_ const int y,
				_In_ const COLORREF& color
			);
			LSTATUS drawBitmap(...);
	};




	namespace color {
		//TODO: 나중에 다른 형식으로 수정 할것
		///--CONSTANT
			COLOR__ BLACK		= "0";
			COLOR__ BLUE		= "1";
			COLOR__ GREEN		= "2";
			COLOR__ CYAN		= "3";
			COLOR__ RED			= "4";
			COLOR__ PLUM		= "5";
			COLOR__ YELLOW		= "6";
			COLOR__ WHITE		= "7";
			COLOR__ GRAY		= "8";
			COLOR__ LIGHTBLUE	= "9";
			COLOR__ LIGHTGREEN	= "A";
			COLOR__ LIGHTCYAN	= "B";
			COLOR__ LIGHTRED	= "C";
			COLOR__ LIGHTPLUM	= "D";
			COLOR__ LIGHTYELLOW = "E";
			COLOR__	LIGHTWHITE	= "F";
		///CONSTANT--
	};

	enum CONSOLE_ERR {
		ERROR_OK = 1,
		ERROR_NULL_HWND,
		ERROR_NULL_HDC,
		ERROR_CANNOT_GET_HWND,
		ERROR_CANNOT_GET_HDC
	};

}

#endif