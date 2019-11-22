#include "RootScene.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

RootScene::RootScene() : listener( nullptr ) {}

Scene* RootScene::create()
{
	auto ret = new ( std::nothrow ) RootScene();
	if( !ret || !ret->init() )
	{
		delete ret;
		ret = nullptr;
		return nullptr;
	}

	ret->autorelease();

	return ret;
}

bool RootScene::init()
{
	if ( !Scene::init() )
        return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto label = Label::createWithTTF("[ESC] : Exit", "fonts/Marker Felt.ttf", 24);
	label->setPosition(Vec2(
		origin.x + visibleSize.width/2
		, origin.y + visibleSize.height - label->getContentSize().height
	));
	addChild(label, 1);

    return true;
}

void RootScene::onEnter()
{
	Scene::onEnter();

	listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2( RootScene::onKeyPressed, this );
	getEventDispatcher()->addEventListenerWithFixedPriority( listener, 1 );
}
void RootScene::onExit()
{
	if( listener )
		getEventDispatcher()->removeEventListener( listener );
	Scene::onExit();
}


void RootScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
{
	switch( keycode )
	{
	case EventKeyboard::KeyCode::KEY_ESCAPE:
	{
		cocos2d::Director::getInstance()->end();
	}
	break;

	default:
		CCLOG( "Key Code : %d", keycode );
	}
}
