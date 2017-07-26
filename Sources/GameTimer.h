#ifndef __GAMETIMER_H
#define __GAMETIMER_H

#include <Windows.h>

class CGameTimer{
private:
	////Not Used
	//struct _TIME_ELEMENTS {
	//	__int64 m_BaseTime;
	//	__int64 m_PausedTime;
	//	__int64 m_StopTime;
	//	__int64 m_PrevTime;
	//	__int64 m_CurTime;
	//};
	//using TIME_ELEMENTS = _TIME_ELEMENTS;
	////---------------

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

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurTime;

	bool m_IsStopped;
};

#endif