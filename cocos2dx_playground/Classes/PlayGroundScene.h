#pragma once

#include "2d/CCScene.h"

class PlayGroundScene : public cocos2d::Scene
{
private:
	PlayGroundScene();

public:
    static cocos2d::Scene* create();

private:
	bool init() override;

public:
    void onEnter() override;
	void onExit() override;
    
	void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

private:
	cocos2d::EventListenerKeyboard* mKeyboardListener;
};
