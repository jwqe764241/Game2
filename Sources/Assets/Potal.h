#pragma once

#include <Windows.h>

class Potal{
private:
	RECT m_Area;
	RECT m_Dest;

public:
	Potal();
	~Potal();

	void Initialize(RECT area, RECT potalTo);
	RECT GetArea() const;
	RECT GetDestination() const;
};