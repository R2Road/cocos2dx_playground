#include "input_practice_KeyAllowScene.h"

#include <cmath> // ceil
#include <sstream>
#include <cstdlib> // ldiv
#include <new>
#include <numeric>
#include <utility> // pair

#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIScrollView.h"

#include "cpg_StringTable.h"
#include "CPG_Input_KeyCodeNames.h"

#include "input_practice_Setting.h"

USING_NS_CC;

namespace
{
	const int TAG_KeyIndicator = 20140416;

	const Size calculateSizeOfKeyAllowControl( const char* str )
	{
		const Size key_allow_margin( 8.f, 4.f );
		auto temp = Label::createWithTTF( str, cpg::StringTable::GetFontPath(), 10 );
		return Size(
			std::ceilf( temp->getContentSize().width + ( key_allow_margin.width * 2 ) )
			, std::ceilf( temp->getContentSize().height + ( key_allow_margin.height * 2 ) )
		);
	}
	const std::pair<int, int> calculateKeyAllowControlsRowAndColumn( const Size view_size, const Size control_size, const Size control_margin )
	{
		const auto _row_count = ldiv(
			static_cast<int>( view_size.height )
			, static_cast<int>( control_size.height + control_margin.height )
		).quot;

		const auto div_result = std::ldiv( step_rain_of_chaos::input::KeyCodeContainerSize, _row_count );

		return std::make_pair(
			div_result.rem > 0 ? div_result.quot + 1 : div_result.quot
			, _row_count
		);
	}

	Node* createKeyAllowControl( const Size control_size, const EventKeyboard::KeyCode target_key_code, const ui::Widget::ccWidgetTouchCallback& callback )
	{
		auto root_node = Node::create();
		{
			//
			// Button
			//
			auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_0.png", ui::Widget::TextureResType::PLIST );
			button->setTag( static_cast<int>( target_key_code ) );
			button->getRendererNormal()->getTexture()->setAliasTexParameters();
			button->getRendererClicked()->getTexture()->setAliasTexParameters();
			button->getRendererDisabled()->getTexture()->setAliasTexParameters();
			button->setScale9Enabled( true );
			button->setContentSize( control_size );
			button->addTouchEventListener( callback );
			root_node->addChild( button );

			//
			// Key Name
			//
			auto label = Label::createWithTTF( cpg::input::KeyCodeNames::get( target_key_code ), cpg::StringTable::GetFontPath(), 10 );
			button->setTitleLabel( label );


			//
			// Indicator
			//
			auto indicator = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_4.png" );
			indicator->setTag( TAG_KeyIndicator );
			indicator->setVisible( false );
			indicator->setContentSize( control_size );
			root_node->addChild( indicator, std::numeric_limits<int>::max() );
		}

		return root_node;
	}
}

namespace input_practice
{
	KeyAllowScene::KeyAllowScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mAllowedKeys()
	{}

	Scene* KeyAllowScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) KeyAllowScene( back_to_the_previous_scene_callback );
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

	bool KeyAllowScene::init()
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
			const auto tab_char = "    ";
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << tab_char;
			ss << tab_char;
			ss << "[ESC] : Save & Exit";
			ss << tab_char;
			ss << tab_char;
			ss << "[Mouse] : Horizontal Scrolling";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			label->setColor( Color3B::GREEN );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition( Vec2(
				visibleOrigin.x
				, visibleOrigin.y + visibleSize.height
			) );
			addChild( label, std::numeric_limits<int>::max() );
		}


		//
		// Load Allowed Keys
		//
		mAllowedKeys = cpg::input::AllowedKeys::load( input_practice::Setting::getKeyAllowFileName().c_str() );



		//
		// Setup Key Allow Controls
		//
		auto scroll_view = ui::ScrollView::create();
		scroll_view->setDirection( ui::ScrollView::Direction::HORIZONTAL );
		scroll_view->setContentSize( visibleSize );
		addChild( scroll_view );

		auto key_allow_controls_root = Node::create();
		key_allow_controls_root->setPosition( Vec2( visibleOrigin.x, visibleOrigin.y ) );
		scroll_view->addChild( key_allow_controls_root );
		{
			static const Size size_of_key_allow_control = calculateSizeOfKeyAllowControl( cpg::input::KeyCodeNames::get_longest() );
			const Size expected_margin_of_key_allow_control( 2.f, 2.f );
			const Size side_margin( 20.f, 20.f );
			const auto row_n_column_count = calculateKeyAllowControlsRowAndColumn(
				visibleSize - ( side_margin * 2 )
				, size_of_key_allow_control
				, expected_margin_of_key_allow_control
			);

			const float margin_of_key_allow_control(
				0 >= row_n_column_count.second
				? 2.f
				: (
					( visibleSize.height - ( side_margin.height * 2 ) )
					- ( row_n_column_count.second * size_of_key_allow_control.height )
				)
				/ row_n_column_count.second
			);

			scroll_view->setInnerContainerSize( Size(
				visibleOrigin.x + ( side_margin.width * 2 ) + ( ( size_of_key_allow_control.width + margin_of_key_allow_control ) * row_n_column_count.first ) - margin_of_key_allow_control
				, visibleSize.height
			) );

			const Vec2 start_position(
				visibleOrigin.x + side_margin.width + ( ( size_of_key_allow_control.width + margin_of_key_allow_control ) * 0.5f )
				, visibleOrigin.y + side_margin.height + ( ( size_of_key_allow_control.height + margin_of_key_allow_control ) * 0.5f )
			);
			const Size spacing_of_control(
				( size_of_key_allow_control.width + margin_of_key_allow_control )
				, ( size_of_key_allow_control.height + margin_of_key_allow_control )
			);

			int grid_x = 0;
			int grid_y = 0;
			for( std::size_t cur = step_rain_of_chaos::input::KeyCodeContainerFirst; step_rain_of_chaos::input::KeyCodeContainerSize > cur; ++cur )
			{
				auto key_allow_control_root = createKeyAllowControl(
					size_of_key_allow_control
					, static_cast<EventKeyboard::KeyCode>( cur )
					, CC_CALLBACK_2( KeyAllowScene::onKeyAllowControl, this )
				);
				key_allow_control_root->setPosition( Vec2(
					start_position.x + ( spacing_of_control.width * grid_x )
					, start_position.y + ( spacing_of_control.height * grid_y )
				) );
				key_allow_controls_root->addChild( key_allow_control_root );

				auto bg = key_allow_control_root->getChildByTag( TAG_KeyIndicator );
				bg->setVisible( mAllowedKeys[cur] );

				++grid_y;
				if( row_n_column_count.second <= grid_y )
				{
					grid_y = 0;
					++grid_x;
				}
			}
		}

		return true;
	}

	void KeyAllowScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyReleased = CC_CALLBACK_2( KeyAllowScene::onKeyReleased, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void KeyAllowScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		cpg::input::AllowedKeys::save( mAllowedKeys, input_practice::Setting::getKeyAllowFileName().c_str() );

		Scene::onExit();
	}

	
	void KeyAllowScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE != keycode )
		{
			return;
		}

		helper::BackToThePreviousScene::MoveBack();
	}

	void KeyAllowScene::onKeyAllowControl( Ref* sender, ui::Widget::TouchEventType touch_event_type )
	{
		if( ui::Widget::TouchEventType::ENDED != touch_event_type )
		{
			return;
		}

		auto button = static_cast<ui::Button*>( sender );

		auto indicator_node = button->getParent()->getChildByTag( TAG_KeyIndicator );
		indicator_node->setVisible( !indicator_node->isVisible() );

		mAllowedKeys[button->getTag()] = indicator_node->isVisible();
	}
}
