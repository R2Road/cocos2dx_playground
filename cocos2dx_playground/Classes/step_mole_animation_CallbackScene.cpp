#include "step_mole_animation_CallbackScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCActionInstant.h"
#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_mole_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_AnimationNode = 20140416;
	const int TAG_AnimationStatusNode = 20160528;

	const int TAG_AnimationAction = 111;
}

namespace step_mole
{
	namespace animation
	{
		CallbackScene::CallbackScene() : mKeyboardListener( nullptr ), mSequenceAction( nullptr ) {}

		Scene* CallbackScene::create()
		{
			auto ret = new ( std::nothrow ) CallbackScene();
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

		bool CallbackScene::init()
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
				ss << "[A] : Play Animation";

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
				auto background_layer = LayerColor::create( Color4B( 70, 0, 110, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Animation
			//
			{
				auto animation_node = Sprite::createWithSpriteFrameName( "step_mole_target_wait_0.png" );
				animation_node->setTag( TAG_AnimationNode );
				animation_node->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				animation_node->setScale( _director->getContentScaleFactor() );
				animation_node->setPosition( Vec2(
					static_cast<int>( visibleOrigin.x + ( visibleSize.width * 0.5f ) )
					, static_cast<int>( visibleOrigin.y + ( visibleSize.height * 0.5f ) )
				) );
				addChild( animation_node );
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.06f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_wkup_0.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_wkup_1.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_wkup_2.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_wkup_3.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_wkup_4.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_idl_0.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_idl_1.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_idl_2.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_idl_3.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_idl_4.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_idl_5.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_idl_6.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_idl_7.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_dmgd1_0.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_dmgd1_1.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_dmgd1_2.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_dmgd2_0.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_dmgd2_1.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_dmgd2_2.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_dmgd2_3.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_dmgd2_4.png" ) );

					auto animate_action = Animate::create( animation_object );

					auto sequence_action = Sequence::create( CallFunc::create( std::bind( &CallbackScene::AnimationStartCallback, this ) ), animate_action, CallFunc::create( std::bind( &CallbackScene::AnimationEndCallback, this ) ), nullptr );

					mSequenceAction = sequence_action;
					mSequenceAction->setTag( TAG_AnimationAction );
					mSequenceAction->retain();
				}
			}

			//
			// Animation Status
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12 );
				label->setTag( TAG_AnimationStatusNode );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition( Vec2(
					static_cast<int>( visibleOrigin.x + ( visibleSize.width * 0.5f ) )
					, static_cast<int>( visibleOrigin.y + ( visibleSize.height * 0.5f ) )
				) );
				addChild( label, std::numeric_limits<int>::max() );

				// setup string
				AnimationEndCallback();
			}

			return true;
		}

		void CallbackScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( CallbackScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void CallbackScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			mSequenceAction->release();

			Scene::onExit();
		}

		void CallbackScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_mole::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_A: // Play
			{
				auto animation_node = getChildByTag( TAG_AnimationNode );
				if( !animation_node->getActionByTag( mSequenceAction->getTag() ) )
				{
					animation_node->runAction( mSequenceAction );
				}
			}
			break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void CallbackScene::AnimationStartCallback()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_AnimationStatusNode ) );
			label->setString( "Play" );
		}
		void CallbackScene::AnimationEndCallback()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_AnimationStatusNode ) );
			label->setString( "Stop" );
		}
	}
}
