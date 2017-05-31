/*
	jwqe764241 작성
*/

//TODO: C, C++ 둘다 사용 가능? __cplusplus

#ifndef ERROR_H
#define ERROR_H


#include <Sources/GameApp.h>

//디버그 모드일때
#ifndef NDEBUG
	#define ERROR_TEST_ENABLE
#endif


typedef LONG ERROR__;

void __cdecl ErrorHandler(
	_In_	char const * const	fileName,
	_In_	char const * const	funcName,
	_In_	const unsigned		lineNumber,
	_In_	char const * const	condition,
	_In_	char const * const	message
);

#define HR(expression)														\
		{																	\
			HRESULT hr = (expression);										\
			if (FAILED(hr)) {												\
				abort();														\
			}																\
		}																	\

#define CUSTOM_ASSERT(condition, message)														\
	if (!(condition)) {																			\
		ErrorHandler(__FILE__, __FUNCTION__, __LINE__, #condition, message);	\
	}																							\

#define TEST____(message)					\
	{										\
		std::cout << "TEST : ";				\
		std::cout << message << std::endl;	\
	}										\

#ifdef ERROR_TEST_ENABLE
	#define GAME_ASSERT(condition, message) CUSTOM_ASSERT(condition, message)
	#define GAME_TEST_TEST_PRINT(message) TEST____(message)
	#define CHECK_HR(expression) HR(expression)
#else	//디버그 모드가 아니면 아무 동작 하지 않음
	#define GAME_ASSERT(condition, message)
	#define GAME_TEST_TEST_PRINT(message)
	#define CHECK_HR(expression)
#endif


#endif