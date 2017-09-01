#include <Sources/Assets/Potal.h>

Potal::Potal()
{

}

Potal::~Potal()
{

}

void Potal::Initialize(RECT area, RECT dest)
{
	Area = area;
	Dest = dest;
}

RECT Potal::GetArea() const
{
	return Area;
}

RECT Potal::GetDestination() const
{
	return Dest;
}