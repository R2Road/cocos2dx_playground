#include "step_mole_game_StageTestScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"

#include "step_mole_RootScene.h"

USING_NS_CC;

namespace step_mole
{
	namespace game
	{
		StageTestScene::StageTestScene() : mKeyboardListener( nullptr )
		{}

		Scene* StageTestScene::create()
		{
			auto ret = new ( std::nothrow ) StageTestScene();
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

		bool StageTestScene::init()
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

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
				addChild( background_layer, -1 );
			}

			//
			// Stage
			//
			{
				const Size BlockSize( 30.f, 30.f );
				struct
				{
					const int BlockCount_Horizontal = 8;
					const int BlockCount_Vercital = 6;
				} StageConfig;
				const Size StageSize(
					BlockSize.width * StageConfig.BlockCount_Horizontal
					, BlockSize.width * StageConfig.BlockCount_Vercital
				);
				const Size StageMargin( 4.f, 4.f );
				const Size TotalSize(
					StageMargin
					+ StageSize
					+ StageMargin
				);

				auto root_node = Node::create();
				root_node->setContentSize( TotalSize );
				root_node->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - TotalSize.width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - TotalSize.height ) * 0.5f )
				) );
				addChild( root_node );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					root_node->addChild( pivot, std::numeric_limits<int>::max() );
				}

				//
				// Background
				//
				{
					auto background_layer = LayerColor::create( Color4B( 255, 0, 255, 150 ), root_node->getContentSize().width, root_node->getContentSize().height );
					root_node->addChild( background_layer, -1 );
				}

				// Click Area
				{
					auto click_area = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
					click_area->setScale9Enabled( true );
					click_area->setContentSize( StageSize );
					click_area->addTouchEventListener( CC_CALLBACK_2( StageTestScene::onStageClick, this ) );
					click_area->setPosition( Vec2(
						TotalSize.width * 0.5f
						, TotalSize.height * 0.5f
					) );
					root_node->addChild( click_area );
				}
			}

			return true;
		}

		void StageTestScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( StageTestScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void StageTestScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void StageTestScene::onStageClick( Ref* /*sender*/, ui::Widget::TouchEventType /*touch_event_type*/ )
		{
			CCLOG( "On Stage Click" );
		}

		void StageTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_mole::RootScene::create() );
				return;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
