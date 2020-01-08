#pragma once

#include "cocos2d.h"

class PlayGroundScene : public cocos2d::Scene
{
public:
	PlayGroundScene();

    static cocos2d::Scene* create();

    void onEnter() override;
	void onExit() override;
    
	void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

private:
	cocos2d::EventListenerKeyboard* mKeyboardListener;
};