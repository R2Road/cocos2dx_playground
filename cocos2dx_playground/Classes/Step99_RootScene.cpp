#include "Step99_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_StringTable.h"

#include "PlayGroundScene.h"

#include "cocos_research_action_SoundActionScene.h"
#include "cocos_research_action_SpriteFrameActionScene.h"

#include "step02_fsm1test_BasicScene.h"
#include "step02_fsm1test_AnimationControlScene.h"
#include "step_rain_of_chaos_collision_CollectionScene.h"

USING_NS_CC;

namespace step99
{
	RootScene::RootScene() : mKeyboardListener( nullptr ) {}

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

		const auto visibleSize = Director::getInstance()->getVisibleSize();
		const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[ESC] : Return to Playground";
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[1] : " << cocos_research_action::SoundActionScene::getTitle();
			ss << std::endl;
			ss << "[2] : " << cocos_research_action::SpriteFrameActionScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[Q] : " << step02::fsm1test::BasicScene::getTitle();
			ss << std::endl;
			ss << "[W] : " << step02::fsm1test::AnimationControlScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[E] : " << step_rain_of_chaos::collision::CollectionScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 12 );
			label->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 0, 9, 61, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
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


	void RootScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			_director->replaceScene( PlayGroundScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_1:
			_director->replaceScene( cocos_research_action::SoundActionScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( cocos_research_action::SpriteFrameActionScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_Q:
			_director->replaceScene( step02::fsm1test::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_W:
			_director->replaceScene( step02::fsm1test::AnimationControlScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_E:
			_director->replaceScene( step_rain_of_chaos::collision::CollectionScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
