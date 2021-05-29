#include "ui_practice_layout_test_ChangeSequenceScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"
#include "ui/UILayoutParameter.h"
#include "ui/UIScrollView.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const int FontSize = 9;
}

namespace ui_practice
{
	namespace layout_test
	{
		ChangeSequenceScene::ChangeSequenceScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mLayout( nullptr )
			, mDirection( true )
		{}

		Scene* ChangeSequenceScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) ChangeSequenceScene( back_to_the_previous_scene_callback );
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

		bool ChangeSequenceScene::init()
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
				ss << "[A] : Reverse";
				ss << cpg::linefeed;
				ss << "[S] : Swap 1st N 2nd";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::LEFT );
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
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			const int VisibleCount = 4;
			const Size ButtonSize( 18, 18 );
			const Size ButtonMargin( 1, 1 );

			//
			// Practice
			//
			{
				const Vec2 LayoutStartPosition( visibleSize.width * 0.5f, visibleSize.height * 0.5f );
				
				mLayout = MakeLayoutVertical( VisibleCount, 4, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, false, false );
				mLayout->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x - ( mLayout->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y - ( mLayout->getContentSize().height * 0.5f )
				) );
				addChild( mLayout );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					pivot->setPosition( mLayout->getPosition() );
					addChild( pivot, std::numeric_limits<int>::max() );
				}
			}

			return true;
		}

		void ChangeSequenceScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ChangeSequenceScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ChangeSequenceScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		ui::Layout* ChangeSequenceScene::MakeLayoutVertical(
			const int button_visible_count
			, const int button_content_count
			, const Size button_size
			, const Size button_margin
			, const ui::LinearLayoutParameter::LinearGravity horizontal_gravity
			, const bool clipping_enable
			, const bool flip
		)
		{
			const Size LayoutSize(
				button_margin.width + button_size.width + button_margin.width
				, ( button_margin.height + button_size.height + button_margin.height ) * button_visible_count
			);

			auto layout_node = ui::Layout::create();
			layout_node->setContentSize( LayoutSize );
			layout_node->setLayoutType( ui::Layout::Type::VERTICAL );
			layout_node->setClippingEnabled( clipping_enable );
			layout_node->setBackGroundColor( Color3B::BLUE );
			layout_node->setBackGroundColorOpacity( 150u );
			layout_node->setBackGroundColorType( cocos2d::ui::Layout::BackGroundColorType::SOLID );
			layout_node->setFlippedY( flip );

			// Content
			for( int i = 1; button_content_count >= i; ++i )
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setTag( i );
				button->setScale9Enabled( true );
				button->setContentSize( button_size );
				button->addTouchEventListener( CC_CALLBACK_2( ChangeSequenceScene::onDummyButton, this ) );

				// Align
				{
					auto param = ui::LinearLayoutParameter::create();
					{
						param->setGravity( horizontal_gravity );

						auto margin = ui::Margin( button_margin.width, button_margin.height, button_margin.width, button_margin.height );
						param->setMargin( margin );
					}

					button->setLayoutParameter( param );
				}

				// Title
				{
					auto label = Label::createWithTTF( std::to_string( i ), cpg::StringTable::GetFontPath(), FontSize );
					label->setScaleY( label->getScaleY() * ( flip ? -1 : 1 ) );
					button->setTitleLabel( label );
				}

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					button->addChild( pivot, std::numeric_limits<int>::max() );
				}

				layout_node->addChild( button, i );
			}

			return layout_node;
		}


		void ChangeSequenceScene::onDummyButton( Ref* sender, ui::Widget::TouchEventType touchEventType )
		{
			if( cocos2d::ui::Widget::TouchEventType::BEGAN == touchEventType )
			{
				auto button_node = static_cast<Node*>( sender );
				CCLOG( "Btn : Began : %d", button_node->getTag() );
			}
			else if( cocos2d::ui::Widget::TouchEventType::ENDED == touchEventType )
			{
				auto button_node = static_cast<Node*>( sender );
				CCLOG( "Btn : End : %d", button_node->getTag() );
			}
		}

		void ChangeSequenceScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			if( EventKeyboard::KeyCode::KEY_A == keycode )
			{
				int temp;
				for( auto c : mLayout->getChildren() )
				{
					temp = c->getLocalZOrder();
					c->setLocalZOrder( mLayout->getChildrenCount() - temp );
					CCLOG( "%d > %d", temp, c->getLocalZOrder() );
				}

				mLayout->requestDoLayout();
			}
			else if( EventKeyboard::KeyCode::KEY_S == keycode )
			{
				CCASSERT( 2 <= mLayout->getChildrenCount(), "ChangeSequenceScene - Swap 1st N 2nd" );

				auto c1 = ( *mLayout->getChildren().begin() );
				auto c2 = *( ++mLayout->getChildren().begin() );

				auto temp_z = c1->getLocalZOrder();
				c1->setLocalZOrder( c2->getLocalZOrder() );
				c2->setLocalZOrder( temp_z );

				mLayout->requestDoLayout();
			}
		}
	}
}
