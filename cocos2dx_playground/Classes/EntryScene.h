#pragma once

#include "cocos2d.h"

class EntryScene : public cocos2d::Scene
{
private:
	EntryScene() = default;

public:
    static cocos2d::Scene* create();

private:
	bool init() override;

	void update_forLoad( float dt );
};
