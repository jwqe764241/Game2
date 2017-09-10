#include <Sources/GameTimer.h>

GameTimer::GameTimer()
	: SecPerCount(0.0), DeltaTime(-1.0), BaseTime(0), PausedTime(0),
	StopTime(0), PrevTime(0), CurTime(0), IsStopped(false)
{
	//초당 카운트 구한 뒤에
	__int64 countPerSec;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&countPerSec));

	//1카운트에 몇초가 걸렸는지 체크 후 SecPerCount 초기화
	SecPerCount = 1.0 / static_cast<double>(countPerSec);
}


float GameTimer::GetTotalTime()
{
	if (IsStopped) 
	{
		return ((StopTime - PausedTime) - BaseTime) * SecPerCount;
	}
	else 
	{
		return ((CurTime - PausedTime) - BaseTime) * SecPerCount;
	}
}

float GameTimer::GetDeltaTime()
{
	return DeltaTime;
}


void GameTimer::Reset()
{
	__int64 curTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&curTime));

	BaseTime  = curTime;
	PrevTime  = curTime;
	StopTime  = 0;
	IsStopped = false;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&startTime));

	if (IsStopped) 
	{
		PausedTime += (startTime - StopTime);
		PrevTime = startTime;
		StopTime = 0;
		IsStopped = false;
	}
}

void GameTimer::Stop()
{
	if (!IsStopped) 
	{
		__int64 curTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&curTime));

		StopTime = curTime;
		IsStopped = true;
	}
}

void GameTimer::Tick()
{
	if (IsStopped) 
	{
		DeltaTime = 0.0;
		return;
	}

	__int64 curTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&curTime));
	CurTime = curTime;

	DeltaTime = (CurTime - PrevTime) * SecPerCount;
	
	PrevTime = CurTime;

	if (DeltaTime < 0.0) 
	{
		DeltaTime = 0.0;
	}
}