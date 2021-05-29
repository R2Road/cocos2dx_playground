#include "step_mole_animation_CallbackScene.h"

#include <new>
#include <numeric>

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

#include "cpg_SStream.h"

#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const int TAG_AnimationAction = 20140416;
}

namespace step_mole
{
	namespace animation
	{
		CallbackScene::CallbackScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mAnimationNode( nullptr )
			, mSequenceAction( nullptr )
			, mAnimationStatusLabel( nullptr )
		{}

		Scene* CallbackScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) CallbackScene( back_to_the_previous_scene_callback );
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

		bool CallbackScene::init()
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
				auto background_layer = LayerColor::create( Color4B( 70, 0, 110, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Animation
			//
			{
				mAnimationNode = Sprite::createWithSpriteFrameName( "step_mole_target_wait_0.png" );
				mAnimationNode->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				mAnimationNode->setScale( _director->getContentScaleFactor() );
				mAnimationNode->setPosition(
					visibleOrigin
					+ Vec2( static_cast<int>( visibleSize.width * 0.5f ), static_cast<int>( visibleSize.height * 0.5f ) )
				);
				addChild( mAnimationNode );
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
				mAnimationStatusLabel = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12 );
				mAnimationStatusLabel->setColor( Color3B::GREEN );
				mAnimationStatusLabel->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				mAnimationStatusLabel->setPosition(
					visibleOrigin
					+ Vec2( static_cast<int>( visibleSize.width * 0.5f ), static_cast<int>( visibleSize.height * 0.5f ) )
				);
				addChild( mAnimationStatusLabel, std::numeric_limits<int>::max() );

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
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_A: // Play
			{
				if( !mAnimationNode->getActionByTag( mSequenceAction->getTag() ) )
				{
					mAnimationNode->runAction( mSequenceAction );
				}
			}
			break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void CallbackScene::AnimationStartCallback()
		{
			mAnimationStatusLabel->setString( "Play" );
		}
		void CallbackScene::AnimationEndCallback()
		{
			mAnimationStatusLabel->setString( "Stop" );
		}
	}
}
