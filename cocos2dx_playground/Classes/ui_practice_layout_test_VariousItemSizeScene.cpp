#include "ui_practice_layout_test_VariousItemSizeScene.h"

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
		VariousItemSizeScene::VariousItemSizeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mLayout( nullptr )
			, mDirection( true )
		{}

		Scene* VariousItemSizeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) VariousItemSizeScene( back_to_the_previous_scene_callback );
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

		bool VariousItemSizeScene::init()
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

			const int VisibleCount = 10;
			const Size ButtonSize( 18, 18 );
			const Size ButtonMargin( 1, 1 );

			//
			// Practice
			//
			{
				mLayout = MakeLayoutVertical( VisibleCount, ButtonSize, ButtonMargin, false );
				mLayout->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f ) - ( mLayout->getContentSize().width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f ) - ( mLayout->getContentSize().height * 0.5f )
				) );
				addChild( mLayout );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					pivot->setPosition( mLayout->getPosition() );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				// Items
				{
					// Content
					bool size_scale = false;
					Size temp_button_size;
					Size temp_button_margin;
					for( int i = 1; 4 >= i; ++i, size_scale = !size_scale )
					{
						temp_button_size.setSize(
							ButtonSize.width
							, ButtonSize.height * ( size_scale ? 2 : 1 )
						);

						// working max size???
						temp_button_margin.setSize(
							ButtonMargin.width
							, ButtonMargin.height * ( size_scale ? 10 : 1 )
						);

						auto button = MakeLayoutItem( i, temp_button_size, temp_button_margin, ui::LinearLayoutParameter::LinearGravity::NONE );
						mLayout->addChild( button, i );
					}
				}
			}

			return true;
		}

		void VariousItemSizeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( VariousItemSizeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void VariousItemSizeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		ui::Layout* VariousItemSizeScene::MakeLayoutVertical(
			const int button_visible_count
			, const Size pivot_button_size
			, const Size button_margin
			, const bool clipping_enable
		)
		{
			const Size LayoutSize(
				button_margin.width + pivot_button_size.width + button_margin.width
				, ( button_margin.height + pivot_button_size.height + button_margin.height ) * button_visible_count
			);

			auto layout_node = ui::Layout::create();
			layout_node->setContentSize( LayoutSize );
			layout_node->setLayoutType( ui::Layout::Type::VERTICAL );
			layout_node->setClippingEnabled( clipping_enable );
			layout_node->setBackGroundColor( Color3B::BLUE );
			layout_node->setBackGroundColorOpacity( 150u );
			layout_node->setBackGroundColorType( cocos2d::ui::Layout::BackGroundColorType::SOLID );

			return layout_node;
		}

		cocos2d::Node* VariousItemSizeScene::MakeLayoutItem(
			const int item_tag
			, const cocos2d::Size button_size
			, const cocos2d::Size button_margin
			, const ui::LinearLayoutParameter::LinearGravity horizontal_gravity
		)
		{
			auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
			button->setScale9Enabled( true );
			button->setContentSize( button_size );
			button->addTouchEventListener( CC_CALLBACK_2( VariousItemSizeScene::onDummyButton, this ) );

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
				auto label = Label::createWithTTF( std::to_string( item_tag ), cpg::StringTable::GetFontPath(), FontSize );
				label->setScaleY( label->getScaleY() );
				button->setTitleLabel( label );
			}

			// Pivot
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				button->addChild( pivot, std::numeric_limits<int>::max() );
			}

			return button;
		}


		void VariousItemSizeScene::onDummyButton( Ref* sender, ui::Widget::TouchEventType touchEventType )
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

		void VariousItemSizeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
