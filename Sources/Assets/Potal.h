#pragma once

#include <Windows.h>

class Potal{
private:
	RECT Area;
	RECT Dest;

public:
	Potal();
	~Potal();

	void Initialize(RECT area, RECT potalTo);
	RECT GetArea() const;
	RECT GetDestination() const;
};