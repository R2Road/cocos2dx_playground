#include "EntryScene.h"

#include <new>

#include "Research_Setting.h"
#include "PlayGroundScene.h"

USING_NS_CC;

Scene* EntryScene::create()
{
	auto ret = new ( std::nothrow ) EntryScene();
	if( !ret || !ret->init() )
	{
		delete ret;
		ret = nullptr;
	}
	else
	{
		ret->autorelease();
	}

	return ret;
}

bool EntryScene::init()
{
	if( !Scene::init() )
	{
		return false;
	}

	scheduleOnce( schedule_selector( EntryScene::update_forLoad ), 0.f );

	return true;
}

void EntryScene::update_forLoad( float /*dt*/ )
{
	research::Setting::load();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "textures/texture_001.plist", "textures/texture_001.png" );
	_director->getTextureCache()->getTextureForKey( "textures/texture_001.png" )->setAliasTexParameters();

	_director->replaceScene( PlayGroundScene::create() );
}
