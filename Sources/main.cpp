#include <Windows.h>





#if defined(_MBCS)

	//For Multubyte

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR IpCmdLine, int nCmdShow) {

	///MessageBox(NULL, "YES!!", "TEST", MB_OK);

}

#elif defined(UNICODE) || defined(_UNICODE)

	//For Unicode	->	Use this

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {


}

#else

	///No Settings 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

}

#endif
