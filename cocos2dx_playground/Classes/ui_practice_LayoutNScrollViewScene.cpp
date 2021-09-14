#include "ui_practice_LayoutNScrollViewScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"
#include "ui/UILayoutParameter.h"
#include "ui/UIScrollView.h"
#include "2d/CCSprite.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "ui_practice_RootScene.h"

USING_NS_CC;

namespace
{
	const std::size_t STRING_COUNT = 20;
	const char* STRINGs[STRING_COUNT] = {
		"alksdjflaskdjflasjdfalsdkfj"
		, "10928310470192830193"
		, "123"
		, "0s9d8f09sd8f098s0d8f"
		, "GJLAKSDJFLASKJDFKLAJSLFJ"
		, "1"
		, "2"
		, "3"
		, "4"
		, "5"
		, "6"
		, "7"
		, "8"
		, "9"
		, "10"
		, "11"
		, "12"
		, "13"
		, "14"
		, "15"
	};

	Size CalculateContentPieceSize( const Size side_margin )
	{
		Size label_max_size;
		{
			auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
			for( const auto& s : STRINGs )
			{
				label->setString( s );

				label_max_size.width = label->getBoundingBox().size.width > label_max_size.width ? label->getBoundingBox().size.width : label_max_size.width;
				label_max_size.height = label->getBoundingBox().size.height > label_max_size.height ? label->getBoundingBox().size.height : label_max_size.height;
			}
		}

		return Size(
			side_margin.width
			+ label_max_size.width
			+ side_margin.width

			, side_margin.height
			+ label_max_size.height
			+ side_margin.height
		);
	}
}

namespace ui_practice
{
	LayoutNScrollViewScene::LayoutNScrollViewScene() : mKeyboardListener( nullptr ) {}

	Scene* LayoutNScrollViewScene::create()
	{
		auto ret = new ( std::nothrow ) LayoutNScrollViewScene();
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

	bool LayoutNScrollViewScene::init()
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

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
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

		//
		// Practice
		//
		{
			const int VisibleButtonCount = 10;
			const Size ButtonMargin( 2, 2 );
			const Size ButtonSize( CalculateContentPieceSize( ButtonMargin ) );

			const Size ListInnerMargin( 1, 1 );
			const Size ListVisibleSize(
				ListInnerMargin.width + ButtonSize.width + ListInnerMargin.width
				, ( ListInnerMargin.height + ButtonSize.height + ListInnerMargin.height ) * VisibleButtonCount
			);

			const Size ListTotalSize(
				ListInnerMargin.width + ButtonSize.width + ListInnerMargin.width
				, ( ListInnerMargin.height + ButtonSize.height + ListInnerMargin.height ) * STRING_COUNT
			);

			const Size RootMargin( 2, 2 );
			const Size RootSize(
				RootMargin
				+ ListVisibleSize
				+ RootMargin
			);


			auto scroll_view = ui::ScrollView::create();
			scroll_view->setDirection( ui::ScrollView::Direction::VERTICAL );
			scroll_view->setContentSize( ListVisibleSize );
			scroll_view->setInnerContainerSize( ListTotalSize );
			scroll_view->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f ) - ( ListVisibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f ) - ( ListVisibleSize.height * 0.5f )
			) );
			addChild( scroll_view );
			{
				auto layout_node = ui::Layout::create();
				layout_node->setContentSize( ListTotalSize );
				layout_node->setLayoutType( ui::Layout::Type::VERTICAL );
				layout_node->setBackGroundColor( Color3B::BLUE );
				layout_node->setBackGroundColorOpacity( 150u );
				layout_node->setBackGroundColorType( cocos2d::ui::Layout::BackGroundColorType::SOLID );
				scroll_view->addChild( layout_node );
				{
					int i = 0;
					for( const auto s : STRINGs )
					{
						auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
						button->setTag( i );
						button->setScale9Enabled( true );
						button->setContentSize( ButtonSize );
						button->setPositionX( RootSize.width * 0.5f );
						button->addTouchEventListener( CC_CALLBACK_2( LayoutNScrollViewScene::onDummyButton, this ) );

						// Label
						{
							auto label = Label::createWithTTF( s, cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
							button->setTitleLabel( label );
						}

						// Align
						{
							auto param = ui::LinearLayoutParameter::create();
							auto margin = ui::Margin( ListInnerMargin.width, ListInnerMargin.height, ListInnerMargin.width, ListInnerMargin.height );
							param->setMargin( margin );

							button->setLayoutParameter( param );
						}

						// Pivot
						{
							auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
							button->addChild( pivot, std::numeric_limits<int>::max() );
						}

						layout_node->addChild( button );

						++i;
					}
				}
			}
		}

		return true;
	}

	void LayoutNScrollViewScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( LayoutNScrollViewScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void LayoutNScrollViewScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void LayoutNScrollViewScene::onDummyButton( Ref* sender, ui::Widget::TouchEventType touchEventType )
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

	void LayoutNScrollViewScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			_director->replaceScene( RootScene::create() );
			return;
		}
	}
}
