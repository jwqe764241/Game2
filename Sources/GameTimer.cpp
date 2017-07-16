#include <Sources/GameTimer.h>

CGameTimer::CGameTimer()
	: m_dSecPerCount(0.0), m_dDeltaTime(-1.0), m_iBaseTime(0), m_iPausedTime(0),
	m_iStopTime(0), m_iPrevTime(0), m_iCurTime(0), m_bIsStopped(false)
{
	__int64 countPerSec;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&countPerSec));
	m_dSecPerCount = 1.0 / static_cast<double>(countPerSec);
}


float CGameTimer::TotalTime()
{
	if (m_bIsStopped) {
		return ((m_iStopTime - m_iPausedTime) - m_iBaseTime) * m_dSecPerCount;
	}
	else {
		return ((m_iCurTime - m_iPausedTime) - m_iBaseTime) * m_dSecPerCount;
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

	m_iBaseTime  = curTime;
	m_iPrevTime  = curTime;
	m_iStopTime  = 0;
	m_bIsStopped = false;
}

void CGameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&startTime));

	if (m_bIsStopped) {
		m_iPausedTime += (startTime - m_iStopTime);
		m_iPrevTime = startTime;
		m_iStopTime = 0;
		m_bIsStopped = false;
	}
}

void CGameTimer::Stop()
{
	if (!m_bIsStopped) {
		__int64 curTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&curTime));

		m_iStopTime = curTime;
		m_bIsStopped = true;
	}
}

void CGameTimer::Tick()
{
	if (m_bIsStopped) {
		m_dDeltaTime = 0.0;
		return;
	}

	__int64 curTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&curTime));
	m_iCurTime = curTime;

	m_dDeltaTime = (m_iCurTime - m_iPrevTime) * m_dSecPerCount;
	
	m_iPrevTime = m_iCurTime;

	if (m_dDeltaTime < 0.0) {
		m_dDeltaTime = 0.0;
	}
}