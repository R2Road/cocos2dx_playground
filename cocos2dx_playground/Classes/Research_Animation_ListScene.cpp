#include "Research_Animation_ListScene.h"

#include <new>
#include <sstream>

#include "Research_Animation_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_AnimationNode = 20140416;

	
}

namespace research
{
	namespace animation
	{
		ListScene::ListScene() : mKeyboardListener( nullptr ), mAnimationActions()
		{}

		Scene* ListScene::create()
		{
			auto ret = new ( std::nothrow ) ListScene();
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

		bool ListScene::init()
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
				ss << "+ Animation - List";
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Return to Root";
				ss << "\n";
				ss << "\n";
				ss << "[A] : Play Animation - Idle";
				ss << "\n";
				ss << "[S] : Play Animation - Run";
				ss << "\n";
				ss << "[D] : Play Animation - Win";
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
				auto background_layer = LayerColor::create( Color4B( 75, 0, 115, 255 ) );
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
			mAnimationActions.reserve( 2 );
			{
				// ani 1
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

					auto animate_action = Animate::create( animation_object );

					auto repeat_action = RepeatForever::create( animate_action );
					repeat_action->setTag( static_cast<int>( eAnimationIndex::run ) );
					repeat_action->retain();

					mAnimationActions.push_back( repeat_action );
				}

				// ani 2
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_idle_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_idle_02.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_idle_03.png" ) );

					auto animate_action = Animate::create( animation_object );

					auto repeat_action = RepeatForever::create( animate_action );
					repeat_action->setTag( static_cast<int>( eAnimationIndex::idle ) );
					repeat_action->retain();

					mAnimationActions.push_back( repeat_action );
				}

				// ani 3
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_win_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_win_02.png" ) );

					auto animate_action = Animate::create( animation_object );

					auto repeat_action = RepeatForever::create( animate_action );
					repeat_action->setTag( static_cast<int>( eAnimationIndex::win ) );
					repeat_action->retain();

					mAnimationActions.push_back( repeat_action );
				}
			}

			return true;
		}

		void ListScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ListScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void ListScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			for( auto a : mAnimationActions )
			{
				a->release();
			}
			mAnimationActions.clear();

			Node::onExit();
		}

		void ListScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}

		void ListScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				if( !isScheduled( schedule_selector( ListScene::updateForExit ) ) )
					scheduleOnce( schedule_selector( ListScene::updateForExit ), 0.f );
				break;

			case EventKeyboard::KeyCode::KEY_A: // Play Idle
				playAnimation( eAnimationIndex::idle );
				break;

			case EventKeyboard::KeyCode::KEY_S: // Play Run
				playAnimation( eAnimationIndex::run );
				break;

			case EventKeyboard::KeyCode::KEY_D: // Play Win
				playAnimation( eAnimationIndex::win );
				break;

			case EventKeyboard::KeyCode::KEY_SPACE: // Play Win
				stopAnimation();
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void ListScene::playAnimation( const eAnimationIndex animation_index )
		{
			auto animation_node = getChildByTag( TAG_AnimationNode );
			if( !animation_node->getActionByTag( static_cast<int>( animation_index ) ) )
			{
				auto animation_action = getAnimationAction( animation_index );
				if( animation_action )
				{
					animation_node->stopAllActions();
					animation_node->runAction( animation_action );
				}
			}
		}
		void ListScene::stopAnimation()
		{
			auto animation_node = getChildByTag( TAG_AnimationNode );
			animation_node->stopAllActions();
		}
		cocos2d::Action* ListScene::getAnimationAction( const eAnimationIndex animation_index )
		{
			for( auto a : mAnimationActions )
			{
				if( static_cast<int>( animation_index ) == a->getTag() )
				{
					return a;
				}
			}

			return nullptr;
		}
	}
}
