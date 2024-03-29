#include "cocos_research_action_SoundActionScene.h"

#include <new>
#include <numeric>
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

#include "cpg_SoundAction.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const int TAG_Action_Animation = 20140416;
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
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[ESC] : Return to Root";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[Q] : Node 1 : Play";
			ss << cpg::linefeed;
			ss << "[W] : Node 1 : Stop";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[A] : Node 2 : Play";
			ss << cpg::linefeed;
			ss << "[S] : Node 2 : Stop";

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
		// Research 1 : Use CallFunc
		//
		{
			// Node
			{
				mTestNode_1 = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				mTestNode_1->setScale( _director->getContentScaleFactor() );
				mTestNode_1->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.3f, visibleSize.height * 0.6f )
				);
				addChild( mTestNode_1 );
			}

			// Explain
			{
				auto label = Label::createWithTTF( "Play Sound\nwith\nCallFunc Action", cpg::StringTable::GetFontPath(), 12 );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					mTestNode_1->getPosition()
					- Vec2( 0.f, 40.f )
				);
				addChild( label );
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

				mTestAction_1 = Sequence::create( animate_action_1, sound_action, animate_action_2, nullptr );
				mTestAction_1->setTag( TAG_Action_Animation );
				mTestAction_1->retain();
			}
		}

		//
		// Research 2 : Use SoundAction
		//
		{
			// Node
			{
				mTestNode_2 = Sprite::createWithSpriteFrameName( "actor001_run_03.png" );
				mTestNode_2->setScale( _director->getContentScaleFactor() );
				mTestNode_2->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.7f, visibleSize.height * 0.6f )
				);
				addChild( mTestNode_2 );
			}

			// Explain
			{
				auto label = Label::createWithTTF( "Play Sound\nwith\nSoundAction", cpg::StringTable::GetFontPath(), 12 );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setColor( Color3B::YELLOW );
				label->setPosition(
					mTestNode_2->getPosition()
					- Vec2( 0.f, 40.f )
				);
				addChild( label );
			}

			// Action
			{
				Animate* animate_action_1 = nullptr;
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

					animate_action_1 = Animate::create( animation_object );
				}

				auto sound_action_1 = cpg::SoundAction::create( "sounds/fx/jump_001.ogg" );

				Animate* animate_action_2 = nullptr;
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );

					animate_action_2 = Animate::create( animation_object );
				}

				auto sound_action_2 = sound_action_1->clone(); // clone test

				mTestAction_2 = Sequence::create( animate_action_1, sound_action_1, animate_action_2, sound_action_2, nullptr );
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
