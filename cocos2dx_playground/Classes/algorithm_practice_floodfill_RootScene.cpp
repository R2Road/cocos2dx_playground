#include "algorithm_practice_floodfill_RootScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "algorithm_practice_floodfill_Direction4Scene.h"
#include "algorithm_practice_floodfill_DirectionMapNodeScene.h"

#include "algorithm_practice_floodfill_TestScene.h"
#include "algorithm_practice_RootScene.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace algorithm_practice_floodfill
{
	RootScene::RootScene() : mKeyboardListener( nullptr )
	{}

	Scene* RootScene::create()
	{
		auto ret = new ( std::nothrow ) RootScene();
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

	bool RootScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleOrigin = _director->getVisibleOrigin();
		const auto visibleSize = _director->getVisibleSize();
		const Vec2 visibleCenter(
			visibleOrigin.x + ( visibleSize.width * 0.5f )
			, visibleOrigin.y + ( visibleSize.height * 0.5f )
		);

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[ESC] : Return to Root";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[1] : " << DirectionMapNodeScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[2] : " << Direction4Scene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "=============================";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[SPACE] : " << TestScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10 );
			label->setPosition( visibleCenter );
			addChild( label, std::numeric_limits<int>::max() );
		}
			
		//
		// Background
		//
		{
			auto layer = LayerColor::create( Color4B( 8, 45, 48, 255 ) );
			addChild( layer, std::numeric_limits<int>::min() );
		}

		return true;
	}

	void RootScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( RootScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void RootScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void RootScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		switch( key_code )
		{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( algorithm_practice::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_1:
				_director->replaceScene( algorithm_practice_floodfill::DirectionMapNodeScene::create( helper::CreateSceneMover<RootScene>() ) );
				return;
			case EventKeyboard::KeyCode::KEY_2:
				_director->replaceScene( algorithm_practice_floodfill::Direction4Scene::create( helper::CreateSceneMover<RootScene>() ) );
				return;

			case EventKeyboard::KeyCode::KEY_SPACE:
				_director->replaceScene( algorithm_practice_floodfill::TestScene::create( helper::CreateSceneMover<RootScene>() ) );
				return;
		}
	}
}
