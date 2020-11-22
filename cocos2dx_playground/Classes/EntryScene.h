#pragma once

#include "2d/CCScene.h"

class EntryScene : public cocos2d::Scene
{
private:
	EntryScene() = default;

public:
    static cocos2d::Scene* create();

private:
	bool init() override;

	void update4Load( float dt );
};
