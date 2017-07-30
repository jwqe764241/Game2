#ifndef __TESTLEVEL2_H
#define __TESTLEVEL2_H

#include <vector>

#include <Sources/Interface/IRenderable.h>
#include <Sources/Interface/ILevel.h>

class TestLevel2 : public ILevel{
private:
	std::vector<IRenderable*> m_RenderList;

public:
	TestLevel2();
	~TestLevel2();

	virtual bool Load() override;
	virtual void Unload() override;
	virtual void Update(float dt) override;
	virtual bool Render(ID3D11DeviceContext* deviceContext) override;

	virtual void onStart() override;
	virtual void onEnd() override;
};

#endif