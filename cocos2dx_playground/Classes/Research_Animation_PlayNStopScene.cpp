#include "Research_Animation_PlayNStopScene.h"

#include <new>
#include <sstream>

#include "Research_Animation_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_AnimationNode = 20140416;

	const int TAG_Action_Animation_Run_Repeat = 111;
}

namespace research
{
	namespace animation
	{
		PlayNStopScene::PlayNStopScene() : mKeyboardListener( nullptr ), mAction_Animation_Run_Repeat( nullptr )
		{}

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
				return false;

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto origin = Director::getInstance()->getVisibleOrigin();

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ Animation - Basic";
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Return to Root";
				ss << "\n";
				ss << "\n";
				ss << "[A] : Play Animation - Run Repeat";
				ss << "\n";
				ss << "[SpaceBar] : Stop Animation";

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
				auto background_layer = LayerColor::create( Color4B( 80, 0, 120, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// Animation Node
			//
			{
				auto animation_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				animation_node->setTag( TAG_AnimationNode );
				animation_node->setAnchorPoint( Vec2( 0.f, 0.f ) );
				animation_node->setScale( 2.f );
				animation_node->setPosition( Vec2(
					static_cast<int>( origin.x + ( visibleSize.width * 0.5f ) - ( animation_node->getContentSize().width * 0.5f ) )
					, static_cast<int>( origin.y + ( visibleSize.height * 0.5f ) - ( animation_node->getContentSize().height * 0.5f ) )
				) );
				addChild( animation_node, 1 );
			}

			//
			// Animation
			//
			{
				auto animation_object = Animation::create();
				animation_object->setDelayPerUnit( 0.2f );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

				auto animate_action = Animate::create( animation_object );

				auto repeat_action = RepeatForever::create( animate_action );

				mAction_Animation_Run_Repeat = repeat_action;
				mAction_Animation_Run_Repeat->setTag( TAG_Action_Animation_Run_Repeat );
				mAction_Animation_Run_Repeat->retain();
			}

			return true;
		}

		void PlayNStopScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayNStopScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void PlayNStopScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			mAction_Animation_Run_Repeat->release();

			Node::onExit();
		}

		void PlayNStopScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}

		void PlayNStopScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				if( !isScheduled( schedule_selector( PlayNStopScene::updateForExit ) ) )
					scheduleOnce( schedule_selector( PlayNStopScene::updateForExit ), 0.f );
				break;

			case EventKeyboard::KeyCode::KEY_A: // Play
			{
				auto animation_node = getChildByTag( TAG_AnimationNode );
				if( !animation_node->getActionByTag( TAG_Action_Animation_Run_Repeat ) )
					animation_node->runAction( mAction_Animation_Run_Repeat );
			}
			break;

			case EventKeyboard::KeyCode::KEY_SPACE: // Stop
			{
				auto animation_node = getChildByTag( TAG_AnimationNode );
				if( animation_node->getActionByTag( TAG_Action_Animation_Run_Repeat ) )
					animation_node->stopActionByTag( TAG_Action_Animation_Run_Repeat );
			}
			break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
