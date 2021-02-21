#include "cocos_research_action_SpriteFrameActionScene.h"

#include <new>
#include <numeric>
#include <sstream>
#include <string>

#include "2d/CCActionInstant.h"
#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "audio/include/AudioEngine.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const int TAG_Action_Animation = 20140416;

	class SoundAction : public ActionInstant
	{
	private:
		SoundAction( const char* sound_path ) : mSoundPath( sound_path )
		{}

		CC_DISALLOW_COPY_AND_ASSIGN( SoundAction );

	public:
		static SoundAction* create( const char* sound_path )
		{
			auto ret = new ( std::nothrow ) SoundAction( sound_path );
			if( ret )
			{
				ret->autorelease();
			}
			else
			{
				CC_SAFE_DELETE( ret );
			}

			return ret;
		}

	public:
		void update( float time ) override
		{
			ActionInstant::update( time );
			experimental::AudioEngine::play2d( mSoundPath, false, 0.1f );
		}
		SoundAction* reverse() const override
		{
			return this->clone();
		}
		SoundAction* clone() const override
		{
			auto a = new ( std::nothrow ) SoundAction( mSoundPath.c_str() );
			a->autorelease();

			return a;
		}

	private:
		const std::string mSoundPath;
	};
}

namespace cocos_research_action
{
	SpriteFrameActionScene::SpriteFrameActionScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mTestNode( nullptr )
		, mTestAction( nullptr )
	{}

	SpriteFrameActionScene::~SpriteFrameActionScene()
	{
		mTestAction->release();
	}

	Scene* SpriteFrameActionScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) SpriteFrameActionScene( back_to_the_previous_scene_callback );
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

	bool SpriteFrameActionScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleSize = _director->getVisibleSize();
		const auto visibleOrigin = _director->getVisibleOrigin();

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[ESC] : Return to Root";
			ss << std::endl;
			ss << std::endl;
			ss << "[Q] : Play";
			ss << std::endl;
			ss << "[W] : Stop";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 8, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition(
				visibleOrigin
				+ Vec2( 0.f, visibleSize.height )
			);
			addChild( label, std::numeric_limits<int>::max() );
		}
			
		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 5, 29, 81, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// Research
		//
		{
			// Node
			{
				mTestNode = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				mTestNode->setScale( _director->getContentScaleFactor() );
				mTestNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mTestNode );
			}

			// Action
			{
				Animate* animate_action_1 = nullptr;
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );

					animate_action_1 = Animate::create( animation_object );
				}

				auto sound_action = CallFunc::create( []() {
					experimental::AudioEngine::play2d( "sounds/fx/powerup_001.ogg", false, 0.1f );
				} );

				Animate* animate_action_2 = nullptr;
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

					animate_action_2 = Animate::create( animation_object );
				}

				mTestAction = Sequence::create( animate_action_1, sound_action, animate_action_2, nullptr );
				mTestAction->setTag( TAG_Action_Animation );
				mTestAction->retain();
			}
		}
		

		return true;
	}

	void SpriteFrameActionScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SpriteFrameActionScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void SpriteFrameActionScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void SpriteFrameActionScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_Q: // Play Once
			if( !mTestNode->getActionByTag( TAG_Action_Animation ) )
			{
				mTestNode->runAction( mTestAction );
			}
			break;
		case EventKeyboard::KeyCode::KEY_W: // Stop
			if( 0 < mTestNode->getNumberOfRunningActions() )
			{
				mTestNode->stopAllActions();
			}
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
}
