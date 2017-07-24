#ifndef __ERROR_H
#define __ERROR_H

#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <string>

//디버그 모드일때
#ifndef NDEBUG
	#define ERROR_TEST_ENABLE
#endif


typedef LONG ERROR__;

void __cdecl ErrorHandler(
	_In_	HRESULT				hr,
	_In_	char const * const	fileName,
	_In_	const unsigned		lineNumber,
	_In_	char const * const	condition
);

void __cdecl ErrorHandler2(
	_In_	char const * const	fileName,
	_In_	char const * const	funcName,
	_In_	const unsigned		lineNumber,
	_In_	char const * const	condition,
	_In_	char const * const	message
);

void __cdecl ErrorHandler2(
	_In_	char const * const	fileName,
	_In_	char const * const	funcName,
	_In_	const unsigned		lineNumber,
	_In_	char const * const	condition,
	_In_	std::string			message
);



#define HR(expression)														\
		{																	\
			HRESULT hr = (expression);										\
			if (FAILED(hr)) {												\
				ErrorHandler(hr, __FILE__, __LINE__, #expression);			\
			}else{															\
				printf("SUCCESS--->hr : %d, in %s\n", hr, #expression);		\
			}																\
		}																	\



#define CUSTOM_ASSERT(condition, message)														\
	if (!(condition)) {																			\
		ErrorHandler2(__FILE__, __FUNCTION__, __LINE__, #condition, message);	\
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