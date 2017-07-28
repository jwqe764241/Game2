#ifndef __TESTLEVEL1_H
#define __TESTLEVEL1_H

#include <Sources/Objects/ILevel.h>

class TestLevel1 : public ILevel{


public:
	TestLevel1();
	~TestLevel1();

	virtual bool Load() override;
	virtual void Unload() override;
	virtual void Update(float dt) override;
	virtual bool Render() override;

	virtual void onStart() override;
	virtual void onEnd() override;
};

#endif