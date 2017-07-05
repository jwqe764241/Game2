#pragma once

#include <iostream>
#include <thread>

class TestService
{
private:
	std::thread* worker;
	bool		bIsRunning;
public:
	void Run(int i);
	void Work(int i);
	void Stop();

public:
	
	TestService();
	~TestService();
};

