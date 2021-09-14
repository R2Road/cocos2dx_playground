#include "ui_practice_LayoutHorizontalScene.h"

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
	LayoutHorizontalScene::LayoutHorizontalScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* LayoutHorizontalScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) LayoutHorizontalScene( back_to_the_previous_scene_callback );
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

	bool LayoutHorizontalScene::init()
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

		const int VisibleCount = 4;
		const Size ButtonSize( 18, 18 );
		const Size ButtonMargin( 1, 1 );

		//
		// Practice : Horizontal Basic
		//
		{
			const Vec2 LayoutStartPosition( visibleSize.width * 0.18f, visibleSize.height * 0.7f );
			const float LayoutSpacing = -visibleSize.height * 0.3f;

			// content = VisibleCount x 1
			{
				auto layout_node = MakeLayoutHorizontal( VisibleCount, 3, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, false, false );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y + ( LayoutSpacing * 0 ) - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					pivot->setPosition( layout_node->getPosition() );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Horizontal\n\nVisible : 4\nContent : 3", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 10.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// content = VisibleCount x 1.5
			{
				auto layout_node = MakeLayoutHorizontal( VisibleCount, 6, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, false, false );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y + ( LayoutSpacing * 1 ) - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					pivot->setPosition( layout_node->getPosition() );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Horizontal\n\nVisible : 4\nContent : 6", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 10.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// content = VisibleCount x 1.5, Clipping
			{
				auto layout_node = MakeLayoutHorizontal( VisibleCount, 6, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, true, false );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y + ( LayoutSpacing * 2 ) - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					pivot->setPosition( layout_node->getPosition() );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Horizontal\n\nVisible : 4\nContent : 6\nClipping", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 10.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}
		}

		//
		// Practice : Horizontal + Gravity
		//
		// - Go and Read : LinearHorizontalLayoutManager::doLayout(LayoutProtocol* layout)
		//
		{
			const Vec2 LayoutStartPosition( visibleSize.width * 0.5f, visibleSize.height * 0.68f );
			const float LayoutSpacing = visibleSize.width * 0.2f;

			// Gravity Top
			{
				auto layout_node = MakeLayoutHorizontal( 3, 2, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::TOP, false, false );
				layout_node->setContentSize( Size( layout_node->getContentSize().width, layout_node->getContentSize().height * 2 ) );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 0 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					pivot->setPosition( layout_node->getPosition() );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Horizontal\n\nGravity Top", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 10.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// Gravity Center
			{
				auto layout_node = MakeLayoutHorizontal( 3, 2, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL, false, false );
				layout_node->setContentSize( Size( layout_node->getContentSize().width, layout_node->getContentSize().height * 2 ) );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 1 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					pivot->setPosition( layout_node->getPosition() );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Horizontal\n\nGravity Center", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 10.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// Gravity Bottom
			{
				auto layout_node = MakeLayoutHorizontal( 3, 2, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::BOTTOM, true, false );
				layout_node->setContentSize( Size( layout_node->getContentSize().width, layout_node->getContentSize().height * 2 ) );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 2 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					pivot->setPosition( layout_node->getPosition() );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Horizontal\n\nGravity Bottom", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 10.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}
		}

		//
		// Practice : Horizontal and Flip
		//
		{
			const Vec2 LayoutStartPosition( visibleSize.width * 0.623f, visibleSize.height * 0.18f );
			const float LayoutSpacing = visibleSize.width * 0.20f;

			// Flip X
			{
				auto layout_node = MakeLayoutHorizontal( 3, 2, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, false, true );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 0 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					pivot->setPosition( layout_node->getPosition() );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Horizontal\n\nFlip X\n\n", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( -layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 10.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// Flip X : Over Eat
			{
				auto layout_node = MakeLayoutHorizontal( 3, 4, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, false, true );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 1 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					pivot->setPosition( layout_node->getPosition() );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Horizontal\n\nFlip X\nOver Eat\n", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( -layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 10.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// Flip X : Over Eat + Clipping
			{
				auto layout_node = MakeLayoutHorizontal( 3, 4, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, true, true );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 2 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y - ( layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					pivot->setPosition( layout_node->getPosition() );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Horizontal\n\nFlip X\nOver Eat\nClipping", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( -layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ Vec2( 0.f, 10.f )
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}
		}

		return true;
	}

	void LayoutHorizontalScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( LayoutHorizontalScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void LayoutHorizontalScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	Node* LayoutHorizontalScene::MakeLayoutHorizontal(
		const int button_visible_count
		, const int button_content_count
		, const Size button_size
		, const Size button_margin
		, const ui::LinearLayoutParameter::LinearGravity vertical_gravity
		, const bool clipping_enable
		, const bool flip
	)
	{
		const Size LayoutSize(
			( button_margin.width + button_size.width + button_margin.width ) * button_visible_count
			, ( button_margin.height + button_size.height + button_margin.height )
		);

		auto layout_node = ui::Layout::create();
		layout_node->setContentSize( LayoutSize );
		layout_node->setLayoutType( ui::Layout::Type::HORIZONTAL );
		layout_node->setClippingEnabled( clipping_enable );
		layout_node->setBackGroundColor( Color3B::BLUE );
		layout_node->setBackGroundColorOpacity( 150u );
		layout_node->setBackGroundColorType( cocos2d::ui::Layout::BackGroundColorType::SOLID );
		layout_node->setFlippedX( flip );

		// Pivot
		{
			auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
			pivot->setScale( 4.f );
			pivot->setPosition( layout_node->getPosition() );
			addChild( pivot, std::numeric_limits<int>::max() );
		}

		// Content
		for( int i = 1; button_content_count >= i; ++i )
		{
			auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
			button->setTag( i );
			button->setScale9Enabled( true );
			button->setContentSize( button_size );
			button->addTouchEventListener( CC_CALLBACK_2( LayoutHorizontalScene::onDummyButton, this ) );

			// Align
			{
				auto param = ui::LinearLayoutParameter::create();
				{
					param->setGravity( vertical_gravity );

					auto margin = ui::Margin( button_margin.width, button_margin.height, button_margin.width, button_margin.height );
					param->setMargin( margin );
				}

				button->setLayoutParameter( param );
			}

			// Title
			{
				auto label = Label::createWithTTF( std::to_string( i ), cpg::StringTable::GetFontPath(), FontSize );
				label->setScaleX( label->getScaleX() * ( flip ? -1 : 1 ) );
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


	void LayoutHorizontalScene::onDummyButton( Ref* sender, ui::Widget::TouchEventType touchEventType )
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

	void LayoutHorizontalScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
