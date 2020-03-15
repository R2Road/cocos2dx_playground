#include "step_typetype_sprite_AnchorPointScene.h"

#include <sstream>

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace step_typetype
{
	namespace sprite
	{
		AnchorPointScene::AnchorPointScene() : mPressedKeyCount( 0 ), mKeyboardListener( nullptr ) {}

		Scene* AnchorPointScene::create()
		{
			auto ret = new ( std::nothrow ) AnchorPointScene();
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

		bool AnchorPointScene::init()
		{
			if( !Scene::init() )
				return false;

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

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setPosition( Vec2(
					visibleOrigin.x + ( label->getContentSize().width * 0.5f )
					, visibleOrigin.y + visibleSize.height - ( label->getContentSize().height * 0.5f )
				) );
				addChild( label, 9999 );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 79, 10, 5, 255 ) );
				addChild( background_layer, -1 );
			}

			//
			// Explain
			//
			{
				auto pivot_indicator = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot_indicator->setScale( 2.f );
				pivot_indicator->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.45f )
					, visibleOrigin.y + ( visibleSize.height * 0.8f )
				) );
				addChild( pivot_indicator );

				auto label = Label::createWithTTF( "< == Pivot", "fonts/arial.ttf", 9 );
				label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				label->setPosition( pivot_indicator->getPosition() + Vec2( 10.f, 0.f ) );
				addChild( label );
			}

			//
			// 0, 0
			//
			{
				auto root_node = Node::create();
				root_node->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.25f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( root_node );
				{
					auto pivot_indicator = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot_indicator->setScale( 2.f );
					root_node->addChild( pivot_indicator, 100 );

					auto sprite = Sprite::createWithSpriteFrameName( "guide_01_3.png" );
					sprite->setAnchorPoint( Vec2( 0.f, 0.f ) );
					root_node->addChild( sprite );

					auto label = Label::createWithTTF( "Anchor Point\n0, 0", "fonts/arial.ttf", 9 );
					label->setColor( Color3B::GREEN );
					label->setPositionY( -visibleSize.height * 0.2f );
					root_node->addChild( label );
				}
			}

			//
			// 0.5, 0.5
			//
			{
				auto root_node = Node::create();
				root_node->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( root_node );
				{
					auto pivot_indicator = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot_indicator->setScale( 2.f );
					root_node->addChild( pivot_indicator, 100 );

					auto sprite = Sprite::createWithSpriteFrameName( "guide_01_3.png" );
					sprite->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
					root_node->addChild( sprite );

					auto label = Label::createWithTTF( "Anchor Point\n0.5, 0.5", "fonts/arial.ttf", 9 );
					label->setColor( Color3B::GREEN );
					label->setPositionY( -visibleSize.height * 0.2f );
					root_node->addChild( label );
				}
			}

			//
			// 1, 1
			//
			{
				auto root_node = Node::create();
				root_node->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.75f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( root_node );
				{
					auto pivot_indicator = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot_indicator->setScale( 2.f );
					root_node->addChild( pivot_indicator, 100 );

					auto sprite = Sprite::createWithSpriteFrameName( "guide_01_3.png" );
					sprite->setAnchorPoint( Vec2( 1.f, 1.f ) );
					root_node->addChild( sprite );

					auto label = Label::createWithTTF( "Anchor Point\n1, 1", "fonts/arial.ttf", 9 );
					label->setColor( Color3B::GREEN );
					label->setPositionY( -visibleSize.height * 0.2f );
					root_node->addChild( label );
				}
			}

			return true;
		}

		void AnchorPointScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( AnchorPointScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void AnchorPointScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void AnchorPointScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( step_typetype::RootScene::create() );
				return;
			}
		}
	}
}