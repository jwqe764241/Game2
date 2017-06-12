#include <Sources/GameApp.h>
#include <Sources/Utils/console.h>

#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK,__FILE__,__LINE)
#endif


#if defined(_MBCS)

	//For Multubyte

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR IpCmdLine, int nCmdShow) {

	///MessageBox(NULL, "YES!!", "TEST", MB_OK);

}

#elif defined(UNICODE) || defined(_UNICODE)

	//For Unicode	->	Use this

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	#if defined(_DEBUG)
		console::STDBUFF consoleBuff = console::openConsole();
	#endif

	CGameApp pGameApp(hInstance, L"TEST", L"WND_CLASS_TEST", nCmdShow, 0, 0);
	pGameApp.Launch();

	#if defined(_DEBUG)
		system("pause");
		console::closeConsole(consoleBuff);
	#endif


}

#else

	///No Settings 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

}

#endif
