#include "step_defender_parallax_node_BasicScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCParallaxNode.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

USING_NS_CC;

namespace
{
	const int TAG_Parallax = 10000;
}

namespace step_defender
{
	namespace parallax_node
	{
		BasicScene::BasicScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mKeyCodeCollector()
		{}

		Scene* BasicScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) BasicScene( back_to_the_previous_scene_callback );
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

		bool BasicScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleOrigin = _director->getVisibleOrigin();
			const auto visibleSize = _director->getVisibleSize();

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";

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
				auto background_layer = LayerColor::create( Color4B( 7, 39, 43, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			//
			//
			{
				auto node = ParallaxNode::create();
				node->setTag( TAG_Parallax );
				addChild( node );

				{
					auto sprite = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
					sprite->setScale( 1.f );
					node->addChild( sprite, 1, Vec2( 1.f, 1.f )
						, Vec2(
							visibleOrigin.x + ( visibleSize.width * 0.5f )
							, visibleOrigin.y + visibleSize.height * 0.5f
						)
					);
				}

				{
					auto sprite = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
					sprite->setScale( 2.f );
					node->addChild( sprite, 2, Vec2( 2.f, 1.f )
						, Vec2(
							visibleOrigin.x + ( visibleSize.width * 0.5f )
							, visibleOrigin.y + visibleSize.height * 0.5f
						)
					);
				}

				{
					auto sprite = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
					sprite->setScale( 3.f );
					node->addChild( sprite, 3, Vec2( 3.f, 1.f )
						, Vec2(
							visibleOrigin.x + ( visibleSize.width * 0.5f )
							, visibleOrigin.y + visibleSize.height * 0.5f
						)
					);
				}
			}

			schedule( schedule_selector( BasicScene::update4Move ) );

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( BasicScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void BasicScene::update4Move( float delta_time )
		{
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_LEFT_ARROW ) )
			{
				getChildByTag( TAG_Parallax )->setPosition( getChildByTag( TAG_Parallax )->getPosition() + Vec2( -100.f * delta_time, 0.f ) );
			}

			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) )
			{
				getChildByTag( TAG_Parallax )->setPosition( getChildByTag( TAG_Parallax )->getPosition() + Vec2( 100.f * delta_time, 0.f ) );
			}
		}

		void BasicScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			mKeyCodeCollector.onKeyPressed( key_code );
		}
		void BasicScene::onKeyReleased( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			mKeyCodeCollector.onKeyReleased( key_code );
		}
	}
}
