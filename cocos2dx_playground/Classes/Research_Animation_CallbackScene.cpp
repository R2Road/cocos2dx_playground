#include "Research_Animation_CallbackScene.h"

#include <new>
#include <sstream>

#include "Research_Animation_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_AnimationNode = 20140416;
	const int TAG_AnimationStatusNode = 20160528;

	const int TAG_AnimationAction = 111;
}

namespace research
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
				return false;

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto origin = Director::getInstance()->getVisibleOrigin();

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ Animation - Callback";
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Return to Root";
				ss << "\n";
				ss << "\n";
				ss << "[A] : Play Animation";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					origin.x
					, origin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}
			
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 70, 0, 110, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// Animation
			//
			{
				auto animation_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				animation_node->setTag( TAG_AnimationNode );
				animation_node->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				animation_node->setScale( 2.f );
				animation_node->setPosition( Vec2(
					static_cast<int>( origin.x + ( visibleSize.width * 0.5f ) )
					, static_cast<int>( origin.y + ( visibleSize.height * 0.5f ) )
				) );
				addChild( animation_node, 1 );
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

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
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
				label->setTag( TAG_AnimationStatusNode );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition( Vec2(
					static_cast<int>( origin.x + ( visibleSize.width * 0.5f ) )
					, static_cast<int>( origin.y + ( visibleSize.height * 0.5f ) )
				) );
				addChild( label, 9999 );

				// setup string
				AnimationEndCallback();
			}

			return true;
		}

		void CallbackScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( CallbackScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void CallbackScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			mSequenceAction->release();

			Node::onExit();
		}

		void CallbackScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}

		void CallbackScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				if( !isScheduled( schedule_selector( CallbackScene::updateForExit ) ) )
					scheduleOnce( schedule_selector( CallbackScene::updateForExit ), 0.f );
				break;

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
