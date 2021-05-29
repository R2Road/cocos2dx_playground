#include "step_typetype_node_AnchorPointScene.h"

#include <new>
#include <numeric>

#include "cocos2d.h"

#include "cpg_SStream.h"

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace step_typetype
{
	namespace node
	{
		AnchorPointScene::AnchorPointScene() : mKeyboardListener( nullptr ) {}

		Scene* AnchorPointScene::create()
		{
			auto ret = new ( std::nothrow ) AnchorPointScene();
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

		bool AnchorPointScene::init()
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

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
				label->setPosition( Vec2(
					visibleOrigin.x + ( label->getContentSize().width * 0.5f )
					, visibleOrigin.y + visibleSize.height - ( label->getContentSize().height * 0.5f )
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 79, 10, 5, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Explain
			//
			{
				auto pivot_indicator = Sprite::create( "textures/step_typetype/step_typetype_helper_pivot.png" );
				pivot_indicator->setScale( 2.f );
				pivot_indicator->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.45f )
					, visibleOrigin.y + ( visibleSize.height * 0.8f )
				) );
				addChild( pivot_indicator );

				auto label = Label::createWithTTF( "< == Pivot", "fonts/NanumSquareR.ttf", 12 );
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
					auto pivot_indicator = Sprite::create( "textures/step_typetype/step_typetype_helper_pivot.png" );
					pivot_indicator->setScale( 2.f );
					root_node->addChild( pivot_indicator, 100 );

					auto sprite = Sprite::create( "textures/step_typetype/step_typetype_dummy_01.png" );
					sprite->setAnchorPoint( Vec2( 0.f, 0.f ) );
					root_node->addChild( sprite );

					auto label = Label::createWithTTF( "Anchor Point\n0, 0", "fonts/NanumSquareR.ttf", 12 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
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
					auto pivot_indicator = Sprite::create( "textures/step_typetype/step_typetype_helper_pivot.png" );
					pivot_indicator->setScale( 2.f );
					root_node->addChild( pivot_indicator, 100 );

					auto sprite = Sprite::create( "textures/step_typetype/step_typetype_dummy_02.png" );
					sprite->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
					root_node->addChild( sprite );

					auto label = Label::createWithTTF( "Anchor Point\n0.5, 0.5\n\nDefault", "fonts/NanumSquareR.ttf", 12 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
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

					auto sprite = Sprite::create( "textures/step_typetype/step_typetype_dummy_01.png" );
					sprite->setAnchorPoint( Vec2( 1.f, 1.f ) );
					root_node->addChild( sprite );

					auto label = Label::createWithTTF( "Anchor Point\n1, 1", "fonts/NanumSquareR.ttf", 12 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
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

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( AnchorPointScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void AnchorPointScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void AnchorPointScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_typetype::RootScene::create() );
				return;
			}
		}
	}
}
