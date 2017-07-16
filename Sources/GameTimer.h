#ifndef __GAMETIMER_H
#define __GAMETIMER_H

#include <Windows.h>

//Performance??
typedef struct _TIME_ELEMENTS {
	__int64 m_iBaseTime;
	__int64 m_iPausedTime;
	__int64 m_iStopTime;
	__int64 m_iPrevTime;
	__int64 m_iCurTime;
}TIME_ELEMENTS, * LPTIMTE_ELEMENTS;

class CGameTimer
{

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