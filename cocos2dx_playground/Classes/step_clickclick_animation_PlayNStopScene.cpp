#include "step_clickclick_animation_PlayNStopScene.h"

#include <new>
#include <numeric>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "step_clickclick_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_Action_Animation = 20140416;
}

namespace step_clickclick
{
	namespace animation
	{
		PlayNStopScene::PlayNStopScene() :
			mKeyboardListener( nullptr )

			, mAnimationNode( nullptr )
			, mAnimationAction( nullptr )
		{}

		PlayNStopScene::~PlayNStopScene()
		{
			mAnimationAction->release();
		}

		Scene* PlayNStopScene::create()
		{
			auto ret = new ( std::nothrow ) PlayNStopScene();
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

		bool PlayNStopScene::init()
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
				ss << "[A] : Play Animation";
				ss << cpg::linefeed;
				ss << "[S] : Stop Animation";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 0, 50, 20, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Animation Node
			//
			{
				mAnimationNode = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				mAnimationNode->setScale( _director->getContentScaleFactor() );
				mAnimationNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mAnimationNode );
			}

			//
			// Animation
			//
			{
				auto animation_object = Animation::create();
				animation_object->setDelayPerUnit( 0.1f );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

				mAnimationAction = Animate::create( animation_object );
				mAnimationAction->setTag( TAG_Action_Animation );
				mAnimationAction->retain();
			}

			return true;
		}

		void PlayNStopScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayNStopScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void PlayNStopScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void PlayNStopScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_clickclick::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_A: // Play Once
				if( !mAnimationNode->getActionByTag( TAG_Action_Animation ) )
				{
					mAnimationNode->runAction( mAnimationAction );
				}
				break;

			case EventKeyboard::KeyCode::KEY_S: // Stop
				if( 0 < mAnimationNode->getNumberOfRunningActions() )
				{
					mAnimationNode->stopAllActions();
				}
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
