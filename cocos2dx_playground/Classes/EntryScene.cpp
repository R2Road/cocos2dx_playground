#include "EntryScene.h"

#include <new>

#include "Research_Setting.h"
#include "RootScene.h"

USING_NS_CC;

Scene* EntryScene::create()
{
	auto ret = new ( std::nothrow ) EntryScene();
	if( !ret || !ret->init() )
	{
		delete ret;
		ret = nullptr;
		return nullptr;
	}

	ret->scheduleOnce( schedule_selector( EntryScene::update_forLoad ), 0.f );

	return ret;
}

void EntryScene::update_forLoad( float dt )
{
	Research::Setting::load();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "textures/texture_001.plist", "textures/texture_001.png" );
	Director::getInstance()->getTextureCache()->getTextureForKey( "textures/texture_001.png" )->setAliasTexParameters();

	Director::getInstance()->replaceScene( RootScene::create() );
}
