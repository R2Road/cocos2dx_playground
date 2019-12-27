#pragma once

#include "cocos2d.h"

class EntryScene : public cocos2d::Scene
{
public:
	EntryScene() = default;

    static cocos2d::Scene* create();

	void update_forLoad( float dt );
};
