#include "ui_practice_LayoutVerticalScene.h"

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
#include "ui/UILayoutParameter.h"
#include "ui/UIScrollView.h"

USING_NS_CC;

namespace
{
	const char* FontPath = "fonts/arial.ttf";
	const int FontSize = 9;
}

namespace ui_practice
{
	LayoutVerticalScene::LayoutVerticalScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* LayoutVerticalScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) LayoutVerticalScene( back_to_the_previous_scene_callback );
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

	bool LayoutVerticalScene::init()
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

			auto label = Label::createWithTTF( ss.str(), FontPath, FontSize, Size::ZERO, TextHAlignment::LEFT );
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
		// Practice : Vertical
		//
		{
			const Vec2 LayoutStartPosition( visibleSize.width * 0.15f, visibleSize.height * 0.45f );
			const float LayoutSpacing = visibleSize.width * 0.12f;

			// content = VisibleCount x 1
			{
				auto layout_node = MakeLayoutVertical( VisibleCount, 3, ButtonSize, ButtonMargin, false );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 0 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Explain
				{
					auto label = Label::createWithTTF( "Vertical\n\nVisible : 4\nContent : 3\n", FontPath, FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 20.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// content = VisibleCount x 1.5
			{
				auto layout_node = MakeLayoutVertical( VisibleCount, 6, ButtonSize, ButtonMargin, false );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 1 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Explain
				{
					auto label = Label::createWithTTF( "Vertical\n\nVisible : 4\nContent : 6\n", FontPath, FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 20.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// content = VisibleCount x 1.5, Clipping
			{
				auto layout_node = MakeLayoutVertical( VisibleCount, 6, ButtonSize, ButtonMargin, true );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 2 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Explain
				{
					auto label = Label::createWithTTF( "Vertical\n\nVisible : 4\nContent : 6\nClipping", FontPath, FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 20.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}
		}


		return true;
	}

	void LayoutVerticalScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( LayoutVerticalScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void LayoutVerticalScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Node::onExit();
	}


	Node* LayoutVerticalScene::MakeLayoutVertical(
		const int button_visible_count
		, const int button_count
		, const Size button_size
		, const Size button_margin
		, const bool clipping_enable
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

		// Pivot
		{
			auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
			pivot->setScale( 4.f );
			pivot->setPosition( layout_node->getPosition() );
			addChild( pivot, std::numeric_limits<int>::max() );
		}

		// Content
		for( int i = 1; button_count >= i; ++i )
		{
			auto button = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
			button->setTag( i );
			button->setScale9Enabled( true );
			button->setContentSize( button_size );
			button->addTouchEventListener( CC_CALLBACK_2( LayoutVerticalScene::onDummyButton, this ) );

			// Align
			{
				auto param = ui::LinearLayoutParameter::create();
				{
					auto margin = ui::Margin( button_margin.width, button_margin.height, button_margin.width, button_margin.height );
					param->setMargin( margin );
				}

				button->setLayoutParameter( param );
			}

			// Title
			{
				auto label = Label::createWithTTF( std::to_string( i ), FontPath, FontSize );
				button->setTitleLabel( label );
			}

			// Pivot
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				button->addChild( pivot, std::numeric_limits<int>::max() );
			}

			layout_node->addChild( button );
		}

		return layout_node;
	}


	void LayoutVerticalScene::onDummyButton( Ref* sender, ui::Widget::TouchEventType touchEventType )
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

	void LayoutVerticalScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
