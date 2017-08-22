#include "Button.h"
#include "Sources\Input\GameInput.h"

Button::Button(wchar_t * buttonText, float x, float y, float width, float height)
	: m_posX(x), m_posY(y), m_width(width), m_height(height)
{
}

Button::~Button()
{
}

void Button::Load(ID3D11Device * device, int width, int height)
{
}

void Button::Load(ID3D11Device * device, wchar_t * normalImagePath, wchar_t * hoveredImagePath, wchar_t * clickedImagePath)
{
	m_normalButtonImagePath = normalImagePath;
	m_hoveredButtonImagePath = hoveredImagePath;
	m_clickedButtonImagePath = clickedImagePath;

	m_normalButtonImage.Initialize(device, normalImagePath, m_width, m_height);
	if (hoveredImagePath != nullptr) {
		m_hoveredButtonImage.Initialize(device, hoveredImagePath, m_width, m_height);
	}
	if (clickedImagePath != nullptr) {
		m_clickedButtonImage.Initialize(device, clickedImagePath, m_width, m_height);
	}

	// TODO: Initialize TextView...
}

void Button::Release()
{
	m_normalButtonImage.Release();
	if (m_hoveredButtonImagePath) {
		m_hoveredButtonImage.Release();
	}
	if (m_clickedButtonImagePath) {
		m_clickedButtonImage.Release();
	}
}

void Button::Reset()
{
}

void Button::Update(float dt)
{
}

void Button::Update(float x, float y)
{
	GameInput& input = GameInput::GetInstance();

	m_hovered = IsHovered(x, y);
	if (m_hovered && input.IsMousePressed(MOUSE_LEFT)) {
		m_clicked = true;
	}
	else {
		m_clicked = false;
	}
}

void Button::Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight)
{
	if (m_clicked) {
		m_clickedButtonImage.Render(deviceContext, screenWidth, screenHeight, m_posX, m_posY);
	}
	else if (m_hovered) {
		m_hoveredButtonImage.Render(deviceContext, screenWidth, screenHeight, m_posX, m_posY);
	}
	else {
		m_normalButtonImage.Render(deviceContext, screenWidth, screenHeight, m_posX, m_posY);
	}
}

int Button::GetIndexCount()
{
	int indexCount;
	if (m_clicked) {
		indexCount = m_clickedButtonImage.GetIndexCount();
	}
	else if (m_hovered) {
		indexCount = m_hoveredButtonImage.GetIndexCount();
	}
	else {
		indexCount = m_normalButtonImage.GetIndexCount();
	}

	return indexCount;
}

ID3D11ShaderResourceView * Button::GetTexture()
{
	if (m_clicked) {
		return m_clickedButtonImage.GetTexture();
	}
	else if (m_hovered) {
		return m_hoveredButtonImage.GetTexture();
	}
	else {
		return m_normalButtonImage.GetTexture();
	}
}

D3DXVECTOR2 Button::GetPosition() const
{
	return D3DXVECTOR2(m_posX, m_posY);
}

void Button::SetPosition(const D3DXVECTOR2 pos)
{
	m_posX = pos.x;
	m_posY = pos.y;
}

bool Button::IsHovered(float x, float y)
{
	if (x >= m_posX && x <= m_posX + m_width &&
		y >= m_posY && y <= m_posY + m_height) {
		return true;
	}
	return false;
}
