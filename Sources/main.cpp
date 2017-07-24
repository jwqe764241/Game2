#include <Sources/GameApp.h>
#include <Sources/Utils/console.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK,__FILE__, __LINE__)
#endif


#if defined(_MBCS)

	//For Multubyte

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR IpCmdLine, int nCmdShow) {

	///MessageBox(NULL, "YES!!", "TEST", MB_OK);

}

#elif defined(UNICODE) || defined(_UNICODE)

	//For Unicode	->	Use this

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) 
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_REPORT_FLAG);

	//디버그 모드일 때 콘솔 열기
	#if defined(_DEBUG)
		console::ConsoleBuffer consoleBuff = console::openConsole();
	#endif

	CGameApp * pGameApp = new CGameApp(hInstance, L"TEST", L"WND_CLASS_TEST", nCmdShow, 800, 600);
	pGameApp->Launch();

	delete pGameApp;

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
