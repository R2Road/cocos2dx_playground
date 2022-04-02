#include "ui_research_button_research_EXButtonScene.h"

#include <new>
#include <numeric>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_ui_EXButtonNode.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace ui_research
{
	namespace button_research
	{
		EXButtonScene::EXButtonScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* EXButtonScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) EXButtonScene( back_to_the_previous_scene_callback );
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

		bool EXButtonScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();
			const Vec2 visibleCenter(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			);

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";

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
				auto background_layer = LayerColor::create( Color4B( 50, 75, 112, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Button Status View
			//
			auto button_status_label = Label::createWithTTF( "-", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			button_status_label->setColor( Color3B::GREEN );
			button_status_label->setPosition(
				Vec2( visibleCenter.x, 0.f )
				+ Vec2( 0.f, visibleSize.height * 0.7f )
			);
			addChild( button_status_label, std::numeric_limits<int>::max() );

			//
			// Research
			//
			{
				const Size button_size( 60.f, 100.f );

				auto ex_button = cpg_ui::EXButtonNode::create( button_size );
				ex_button->setPosition( visibleCenter );
				addChild( ex_button );
				
				// Background
				{
					ex_button->SetView_Background( LayerColor::create( Color4B::BLACK, 60u, 100u ) );
				}

				// Content View
				{
					auto sprite = Sprite::createWithSpriteFrameName( "item_sword_01_0.png" );
					sprite->setPosition( button_size.width * 0.5f, button_size.height * 0.5f );
					sprite->setScale( 6.f );

					ex_button->SetView_Content( sprite );
				}

				// Normal View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_0.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( button_size );

					ex_button->SetView_Button( cpg_ui::EXButtonNode::eViewIndex::Normal, sprite );
				}

				// Mouse Over View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_1.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( button_size );
					sprite->setVisible( false );
					{
						auto label = Label::createWithTTF( "Sword\nOf\nDummy", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::CENTER );
						label->setPosition( sprite->getContentSize().width * 0.5f, sprite->getContentSize().height * 0.5f );
						{
							auto fadeOutAction = FadeOut::create( 0.8f );
							auto fadeOutkDelay = DelayTime::create( 0.2f );
							auto fadeInAction = FadeIn::create( 0.6f );
							auto fadeInkDelay = DelayTime::create( 0.4f );
							auto blinkSequence = Sequence::create( fadeOutAction, fadeOutkDelay, fadeInAction, fadeInkDelay, nullptr );
							auto blinkrepeat = RepeatForever::create( blinkSequence );
							label->runAction( blinkrepeat );
						}
						sprite->addChild( label );
					}

					ex_button->SetView_Button( cpg_ui::EXButtonNode::eViewIndex::MouseOver, sprite );
				}

				// Push View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_0.png" );
					sprite->setVisible( false );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( button_size );
					sprite->setColor( Color3B::YELLOW );

					ex_button->SetView_Button( cpg_ui::EXButtonNode::eViewIndex::Push, sprite );
				}

				ex_button->SetCallback( [button_status_label]( const cpg_ui::EXButtonNode::eButtonEvent button_event )
				{
					switch( button_event )
					{
					case cpg_ui::EXButtonNode::eButtonEvent::MouseOver:
						CCLOG( "MouseOver" );
						button_status_label->setString( "MouseOver" );
						break;
					case cpg_ui::EXButtonNode::eButtonEvent::MouseLeave:
						CCLOG( "MouseLeave" );
						button_status_label->setString( "MouseLeave" );
						break;
					case cpg_ui::EXButtonNode::eButtonEvent::Push:
						CCLOG( "Push" );
						button_status_label->setString( "Push" );
						break;
					case cpg_ui::EXButtonNode::eButtonEvent::Move:
						CCLOG( "Move" );
						button_status_label->setString( "Move" );
						break;
					case cpg_ui::EXButtonNode::eButtonEvent::Release:
						CCLOG( "Release" );
						button_status_label->setString( "Release" );
						break;
					}
				} );
			}

			return true;
		}

		void EXButtonScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( EXButtonScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void EXButtonScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void EXButtonScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
