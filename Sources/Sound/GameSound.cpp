#include "GameSound.h"
#include "../GameDefs.h"

GameSound::GameSound() : m_builder(nullptr), m_control(nullptr), m_event(nullptr), m_stopped(true), m_position(nullptr)
{
}

GameSound::GameSound(const GameSound &)
{
}

GameSound::~GameSound()
{
}

bool GameSound::Initialize(WCHAR * filename)
{
	HRESULT result;

	result = CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, reinterpret_cast<void**>(&m_builder));
	if (FAILED(result)) return false;

	result = m_builder->QueryInterface(IID_IMediaControl, reinterpret_cast<void**>(&m_control));
	if (FAILED(result)) return false;

	result = m_builder->QueryInterface(IID_IMediaPosition, reinterpret_cast<void**>(&m_position));
	if (FAILED(result)) return false;

	result = m_builder->QueryInterface(IID_IMediaEvent, reinterpret_cast<void**>(&m_event));
	if (FAILED(result)) return false;

	result = m_builder->RenderFile(filename, nullptr);
	if (FAILED(result)) return false;

	return true;
}

void GameSound::Play()
{
	Stop();
	m_stopped = false;
	m_control->Run();
}

void GameSound::Stop()
{
	if (!m_stopped) {
		m_stopped = true;
		m_control->Stop();
		m_position->put_CurrentPosition(0);
	}
}

void GameSound::Shutdown()
{
	Stop();
	Utils::Release(&m_event);
	Utils::Release(&m_position);
	Utils::Release(&m_control);
	Utils::Release(&m_builder);
}

void GameSound::UpdateEvent()
{
	HRESULT result;
	long evtCode;
	LONG_PTR param1, param2;

	result = m_event->GetEvent(&evtCode, &param1, &param2, 0);

	if (SUCCEEDED(result)) {
		m_event->FreeEventParams(evtCode, param1, param2);

		if (evtCode == EC_COMPLETE) {
			if (m_loop) {
				Play();
			}
			else {
				Stop();
			}
		}
	}
}

void GameSound::SetLoop(bool loop)
{
	m_loop = loop;
}

bool GameSound::GetLoop()
{
	return m_loop;
}
