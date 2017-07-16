#include "TestService.h"

TestService::TestService()
{
	bIsRunning = false;
}

TestService::~TestService()
{
}

void TestService::Run(int i) {
	if (bIsRunning != true) {
		bIsRunning = true;
		worker = new std::thread(&TestService::Work, this, i);
	}
}

void TestService::Work(int i) {
	while (bIsRunning) {
		std::cout << i << " DSfsdfdsf" << std::endl;
	}
}

void TestService::Stop() {
	if (bIsRunning != false) {
		bIsRunning = false;
		worker->join();
		delete worker;
	}
}