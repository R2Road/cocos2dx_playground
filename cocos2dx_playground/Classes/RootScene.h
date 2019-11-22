#pragma once

#include "cocos2d.h"

class RootScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* create();

    bool init() override;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
};
