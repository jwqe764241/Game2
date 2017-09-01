#pragma once

#include <Windows.h>

class GameTimer
{
private:
	double SecPerCount;
	double DeltaTime;

	__int64 BaseTime;
	__int64 PausedTime;
	__int64 StopTime;
	__int64 PrevTime;
	__int64 CurTime;

	bool IsStopped;

public:
	GameTimer();

	float GetTotalTime();
	float GetDeltaTime();

	void Reset();
	void Start();
	void Stop();
	void Tick();
};