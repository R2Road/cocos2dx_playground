#include "step_clickclick_animation_PlayNStopScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_clickclick_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_AnimationNode = 20140416;

	const int TAG_Action_Animation_Run_Once = 111;
	const int TAG_Action_Animation_Run_Repeat = 222;
}

namespace step_clickclick
{
	namespace animation
	{
		PlayNStopScene::PlayNStopScene() :
			mKeyboardListener( nullptr )
			, mNormalAction( nullptr )
			, mRepeatAction( nullptr )
		{}

		PlayNStopScene::~PlayNStopScene()
		{
			mNormalAction->release();
			mRepeatAction->release();
		}

		Scene* PlayNStopScene::create()
		{
			auto ret = new ( std::nothrow ) PlayNStopScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
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
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << std::endl;
				ss << "[A] : Play Animation - Once";
				ss << std::endl;
				ss << "[S] : Play Animation - Loop";
				ss << std::endl;
				ss << "[SpaceBar] : Stop Animation";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
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
				auto animation_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				animation_node->setTag( TAG_AnimationNode );
				animation_node->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				animation_node->setScale( _director->getContentScaleFactor() );
				animation_node->setPosition( Vec2(
					static_cast<int>( visibleOrigin.x + ( visibleSize.width * 0.5f ) )
					, static_cast<int>( visibleOrigin.y + ( visibleSize.height * 0.5f ) )
				) );
				addChild( animation_node );
			}

			//
			// Animation
			//
			{
				// Normal Action
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

					mNormalAction = Animate::create( animation_object );
					mNormalAction->setTag( TAG_Action_Animation_Run_Once );
					mNormalAction->retain();
				}

				// Repeat Action
				{
					mRepeatAction = RepeatForever::create( static_cast<Animate*>( mNormalAction->clone() ) );

					mRepeatAction->setTag( TAG_Action_Animation_Run_Repeat );
					mRepeatAction->retain();
				}
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

			Node::onExit();
		}

		void PlayNStopScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_clickclick::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_A: // Play Once
			{
				auto animation_node = getChildByTag( TAG_AnimationNode );
				animation_node->stopActionByTag( TAG_Action_Animation_Run_Once );
				animation_node->stopActionByTag( TAG_Action_Animation_Run_Repeat );
				animation_node->runAction( mNormalAction );
			}
			break;

			case EventKeyboard::KeyCode::KEY_S: // Play Repeat
			{
				auto animation_node = getChildByTag( TAG_AnimationNode );
				if( !animation_node->getActionByTag( TAG_Action_Animation_Run_Repeat ) )
				{
					animation_node->stopActionByTag( TAG_Action_Animation_Run_Once );
					animation_node->runAction( mRepeatAction );
				}
			}
			break;

			case EventKeyboard::KeyCode::KEY_SPACE: // Stop
			{
				auto animation_node = getChildByTag( TAG_AnimationNode );
				if( 0 < animation_node->getNumberOfRunningActions() )
				{
					animation_node->stopAllActions();
				}
			}
			break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
