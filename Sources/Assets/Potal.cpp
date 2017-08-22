#include <Sources/Assets/Potal.h>

Potal::Potal()
{

}

Potal::~Potal()
{

}

void Potal::Initialize(RECT area, RECT dest)
{
	m_Area = area;
	m_Dest = dest;
}

RECT Potal::GetArea() const
{
	return m_Area;
}

RECT Potal::GetDestination() const
{
	return m_Dest;
}