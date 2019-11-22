#pragma once

#include "cocos2d.h"

class RootScene : public cocos2d::Scene
{
public:
	RootScene();

    static cocos2d::Scene* create();

    void onEnter() override;
	void onExit() override;
    
	void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );

private:
	cocos2d::EventListenerKeyboard* listener;
};
