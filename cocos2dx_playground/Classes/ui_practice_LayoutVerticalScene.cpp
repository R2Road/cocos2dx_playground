#include "ui_practice_LayoutVerticalScene.h"

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
		// Practice : Vertical Basic
		//
		{
			const Vec2 LayoutStartPosition( visibleSize.width * 0.1f, visibleSize.height * 0.45f );
			const float LayoutSpacing = visibleSize.width * 0.15f;

			// content = VisibleCount x 1
			{
				auto layout_node = MakeLayoutVertical( VisibleCount, 3, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, false, false );
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
					auto label = Label::createWithTTF( "Vertical\n\nVisible : 4\nContent : 3\n", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
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
				auto layout_node = MakeLayoutVertical( VisibleCount, 6, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, false, false );
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
					auto label = Label::createWithTTF( "Vertical\n\nVisible : 4\nContent : 6\n", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
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
				auto layout_node = MakeLayoutVertical( VisibleCount, 6, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, true, false );
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
					auto label = Label::createWithTTF( "Vertical\n\nVisible : 4\nContent : 6\nClipping", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
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


		//
		// Practice : Vertical + Gravity
		//
		// - Go and Read : LinearVerticalLayoutManager::doLayout(LayoutProtocol* layout)
		//
		{
			const Vec2 LayoutStartPosition( visibleSize.width * 0.6f, visibleSize.height * 0.69f );
			const float LayoutSpacing = visibleSize.width * 0.14f;
			const Vec2 LabelOffset( 0, 24 );

			// Gravity Left
			{
				auto layout_node = MakeLayoutVertical( 3, 2, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::LEFT, false, false );
				layout_node->setContentSize( Size( layout_node->getContentSize().width * 2, layout_node->getContentSize().height ) );
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
					auto label = Label::createWithTTF( "Vertical\n\nGravity : Left", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ LabelOffset
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// Gravity Center
			{
				auto layout_node = MakeLayoutVertical( 3, 2, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL, false, false );
				layout_node->setContentSize( Size( layout_node->getContentSize().width * 2, layout_node->getContentSize().height ) );
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
					auto label = Label::createWithTTF( "Vertical\n\nGravity : Center", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ LabelOffset
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// Gravity Right
			{
				auto layout_node = MakeLayoutVertical( 3, 2, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::RIGHT, true, false );
				layout_node->setContentSize( Size( layout_node->getContentSize().width * 2, layout_node->getContentSize().height ) );
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
					auto label = Label::createWithTTF( "Vertical\n\nGravity : Right", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height )
						+ LabelOffset
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}
		}


		//
		// Practice : Vertical and Flip
		//
		{
			const Vec2 LayoutStartPosition( visibleSize.width * 0.6f, visibleSize.height * 0.14f );
			const float LayoutSpacing = visibleSize.width * 0.14f;
			const Vec2 LabelOffset( 0, 24 );

			// Flip Y
			{
				auto layout_node = MakeLayoutVertical( 3, 2, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, false, true );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 0 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y + ( layout_node->getContentSize().height * 0.5f )
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
					auto label = Label::createWithTTF( "Vertical\n\nFlip Y\n\n", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, 0.f )
						+ LabelOffset
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// Flip Y : Over Eat
			{
				auto layout_node = MakeLayoutVertical( 3, 4, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, false, true );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 1 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y + ( layout_node->getContentSize().height * 0.5f )
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
					auto label = Label::createWithTTF( "Vertical\n\nFlip Y\nOver Eat\n", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, 0.f )
						+ LabelOffset
					);
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			// Flip Y : Over Eat + Clipping
			{
				auto layout_node = MakeLayoutVertical( 3, 4, ButtonSize, ButtonMargin, ui::LinearLayoutParameter::LinearGravity::NONE, true, true );
				layout_node->setPosition( Vec2(
					visibleOrigin.x + LayoutStartPosition.x + ( LayoutSpacing * 2 ) - ( layout_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + LayoutStartPosition.y + ( layout_node->getContentSize().height * 0.5f )
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
					auto label = Label::createWithTTF( "Vertical\n\nFlip Y\nOver Eat\nClipping", cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::CENTER );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						layout_node->getPosition()
						+ Vec2( layout_node->getContentSize().width * 0.5f, 0.f )
						+ LabelOffset
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

		Scene::onExit();
	}


	Node* LayoutVerticalScene::MakeLayoutVertical(
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
			button->addTouchEventListener( CC_CALLBACK_2( LayoutVerticalScene::onDummyButton, this ) );

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
