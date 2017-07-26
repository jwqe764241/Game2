#ifndef __GAMETIMER_H
#define __GAMETIMER_H

#include <Windows.h>

class CGameTimer
{
private:
	//Not User Yet
	struct _TIME_ELEMENTS {
		__int64 m_iBaseTime;
		__int64 m_iPausedTime;
		__int64 m_iStopTime;
		__int64 m_iPrevTime;
		__int64 m_iCurTime;
	};
	using TIME_ELEMENTS = _TIME_ELEMENTS;
	//---------------

public:
	CGameTimer();

	float TotalTime();
	float DeltaTime();

	void Reset();
	void Start();
	void Stop();
	void Tick();

private:
	double m_dSecPerCount;
	double m_dDeltaTime;

	__int64 m_iBaseTime;
	__int64 m_iPausedTime;
	__int64 m_iStopTime;
	__int64 m_iPrevTime;
	__int64 m_iCurTime;

	bool m_bIsStopped;
};

#endif