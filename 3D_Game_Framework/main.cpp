#include <Windows.h>

#if defined(_MBCS)

	//For Multubyte

#elif defined(UNICODE) || defined(_UNICODE)

	//For Unicode

#else

	///No Settings 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lCmdLine, int nCmdShow) {

	

}

#endif