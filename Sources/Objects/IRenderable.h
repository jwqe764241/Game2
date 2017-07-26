#pragma once

class IRenderable
{
public:
	virtual void Load() = 0;
	virtual void Destroy() = 0;
	virtual void Reset() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
};