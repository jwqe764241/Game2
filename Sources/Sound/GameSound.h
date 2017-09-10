#pragma once

#pragma comment(lib, "strmiids")

#include <Windows.h>
#include <dshow.h>

class GameSound
{
private:

public:
	GameSound();
	GameSound(const GameSound&);
	~GameSound();

	bool Initialize(WCHAR*);
	void Play();
	void Stop();
	void Shutdown();

	void UpdateEvent();

	void SetLoop(bool);
	bool GetLoop();

private:
	IGraphBuilder* m_builder;
	IMediaControl* m_control;
	IMediaPosition* m_position;
	IMediaEvent* m_event;

	bool m_stopped;

	bool m_loop;
};