#include "cocos_research_action_SoundActionScene.h"

#include <new>
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
#include "base/ccUTF8.h"

#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const int TAG_Action_Animation = 20140416;

	class SoundAction : public ActionInstant
	{
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
		void execute()
		{
			experimental::AudioEngine::play2d( mSoundPath, false, 0.1f );
		}

		void update( float time ) override
		{
			ActionInstant::update( time );
			this->execute();
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
		SoundAction( const char* sound_path ) : mSoundPath( sound_path )
		{}

		CC_DISALLOW_COPY_AND_ASSIGN( SoundAction );

	private:
		std::string mSoundPath;
	};
}

namespace cocos_research_action
{
	SoundActionScene::SoundActionScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mTestNode_1( nullptr )
		, mTestNode_2( nullptr )

		, mTestAction_1( nullptr )
		, mTestAction_2( nullptr )
	{}

	SoundActionScene::~SoundActionScene()
	{
		mTestAction_1->release();
		mTestAction_2->release();
	}

	Scene* SoundActionScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) SoundActionScene( back_to_the_previous_scene_callback );
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
		}
		else
		{
			ret->scheduleUpdate();
			ret->autorelease();
		}

		return ret;
	}

	bool SoundActionScene::init()
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
			ss << "[Q] : Node 1 : Play";
			ss << std::endl;
			ss << "[W] : Node 1 : Stop";
			ss << std::endl;
			ss << std::endl;
			ss << "[A] : Node 2 : Play";
			ss << std::endl;
			ss << "[S] : Node 2 : Stop";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition( Vec2(
				visibleOrigin.x
				, visibleOrigin.y + visibleSize.height
			) );
			addChild( label, 9999 );
		}
			
		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 5, 29, 81, 255 ) );
			addChild( background_layer, 0 );
		}

		//
		// Research 1
		//
		{
			// Node
			{
				mTestNode_1 = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				mTestNode_1->setScale( _director->getContentScaleFactor() );
				mTestNode_1->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.3f, visibleSize.height * 0.5f )
				);
				addChild( mTestNode_1 );
			}

			// Animation
			{
				Animate* animate_action_1 = nullptr;
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );

					animate_action_1 = Animate::create( animation_object );
				}

				auto sound_action = SoundAction::create( "sounds/fx/jump_001.ogg" );

				Animate* animate_action_2 = nullptr;
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

					animate_action_2 = Animate::create( animation_object );
				}

				mTestAction_1 = Sequence::create( animate_action_1, sound_action, animate_action_2, nullptr );
				mTestAction_1->setTag( TAG_Action_Animation );
				mTestAction_1->retain();
			}
		}

		//
		// Research 2
		//
		{
			// Node
			{
				mTestNode_2 = Sprite::createWithSpriteFrameName( "actor001_run_03.png" );
				mTestNode_2->setScale( _director->getContentScaleFactor() );
				mTestNode_2->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.7f, visibleSize.height * 0.5f )
				);
				addChild( mTestNode_2 );
			}

			// Animation
			{
				Animate* animate_action_1 = nullptr;
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

					animate_action_1 = Animate::create( animation_object );
				}

				auto sound_action = CallFunc::create( []() {
					experimental::AudioEngine::play2d( "sounds/fx/powerup_001.ogg", false, 0.1f );
				} );

				Animate* animate_action_2 = nullptr;
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );

					animate_action_2 = Animate::create( animation_object );
				}

				mTestAction_2 = Sequence::create( animate_action_1, sound_action, animate_action_2, nullptr );
				mTestAction_2->setTag( TAG_Action_Animation );
				mTestAction_2->retain();
			}
		}
		

		return true;
	}

	void SoundActionScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SoundActionScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void SoundActionScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void SoundActionScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_Q: // Play Once
			if( !mTestNode_1->getActionByTag( TAG_Action_Animation ) )
			{
				mTestNode_1->runAction( mTestAction_1 );
			}
			break;
		case EventKeyboard::KeyCode::KEY_W: // Stop
			if( 0 < mTestNode_1->getNumberOfRunningActions() )
			{
				mTestNode_1->stopAllActions();
			}
			break;

		case EventKeyboard::KeyCode::KEY_A: // Play Once
			if( !mTestNode_2->getActionByTag( TAG_Action_Animation ) )
			{
				mTestNode_2->runAction( mTestAction_2 );
			}
			break;
		case EventKeyboard::KeyCode::KEY_S: // Stop
			if( 0 < mTestNode_2->getNumberOfRunningActions() )
			{
				mTestNode_2->stopAllActions();
			}
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
}
