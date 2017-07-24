/*
	jwqe764241 작성
*/

#include <Sources/Utils/error.h>

#include <iostream>
#include <stdlib.h>



void __cdecl ErrorHandler(
	_In_	HRESULT				hr,
	_In_	char const * const	fileName,
	_In_	const unsigned		lineNumber,
	_In_	char const * const	condition
) {

	std::cerr.setf(std::ios::showbase);
	std::cerr.setf(std::ios::hex, std::ios::basefield);
	std::cerr << "-------------------------------------" << std::endl;
	std::cerr << "Assertion Get False" << std::endl;
	std::cerr << "      Return Code : " << hr << std::endl;
	std::cerr.unsetf(std::ios::showbase);
	std::cerr.unsetf(std::ios::hex);
	std::cerr << "      File : " << fileName << " : " << lineNumber << " at Line " << std::endl;
	std::cerr << "      Condition Expression → " << condition << std::endl;
	std::cerr << "-------------------------------------" << std::endl;

	std::abort();
}

void __cdecl ErrorHandler2(
	_In_	char const * const	fileName,
	_In_	char const * const	funcName,
	_In_	const unsigned		lineNumber,
	_In_	char const * const	condition,
	_In_	char const * const	message
) {
	std::cerr << "-------------------------------------" << std::endl;
	std::cerr << "Assertion Get False" << std::endl;
	std::cerr << "      File : "<< fileName << " : " << lineNumber << " Line at " << funcName << std::endl;
	std::cerr << "      Condition Expression → " << condition << std::endl;
	std::cerr << "      message : " << message << std::endl;
	std::cerr << "-------------------------------------" << std::endl;

	//강제종료 -> abort() 호출
	std::abort();
}

void __cdecl ErrorHandler2(
	_In_	char const * const	fileName,
	_In_	char const * const	funcName,
	_In_	const unsigned		lineNumber,
	_In_	char const * const	condition,
	_In_	std::string			message
) {
	ErrorHandler2(fileName, funcName, lineNumber, condition, message.c_str());
}