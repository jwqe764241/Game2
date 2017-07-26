#include <Sources/GameTimer.h>

CGameTimer::CGameTimer()
	: m_dSecPerCount(0.0), m_dDeltaTime(-1.0), m_BaseTime(0), m_PausedTime(0),
	m_StopTime(0), m_PrevTime(0), m_CurTime(0), m_IsStopped(false)
{
	__int64 countPerSec;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&countPerSec));
	m_dSecPerCount = 1.0 / static_cast<double>(countPerSec);
}


float CGameTimer::TotalTime()
{
	if (m_IsStopped) 
	{
		return ((m_StopTime - m_PausedTime) - m_BaseTime) * m_dSecPerCount;
	}
	else 
	{
		return ((m_CurTime - m_PausedTime) - m_BaseTime) * m_dSecPerCount;
	}
}

float CGameTimer::DeltaTime()
{
	return m_dDeltaTime;
}


void CGameTimer::Reset()
{
	__int64 curTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&curTime));

	m_BaseTime  = curTime;
	m_PrevTime  = curTime;
	m_StopTime  = 0;
	m_IsStopped = false;
}

void CGameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&startTime));

	if (m_IsStopped) 
	{
		m_PausedTime += (startTime - m_StopTime);
		m_PrevTime = startTime;
		m_StopTime = 0;
		m_IsStopped = false;
	}
}

void CGameTimer::Stop()
{
	if (!m_IsStopped) 
	{
		__int64 curTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&curTime));

		m_StopTime = curTime;
		m_IsStopped = true;
	}
}

void CGameTimer::Tick()
{
	if (m_IsStopped) 
	{
		m_dDeltaTime = 0.0;
		return;
	}

	__int64 curTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&curTime));
	m_CurTime = curTime;

	m_dDeltaTime = (m_CurTime - m_PrevTime) * m_dSecPerCount;
	
	m_PrevTime = m_CurTime;

	if (m_dDeltaTime < 0.0) 
	{
		m_dDeltaTime = 0.0;
	}
}