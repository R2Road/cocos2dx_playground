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

	const Size calculateSizeOfKeyAllowControl()
	{
		const Size key_allow_margin( 8.f, 4.f );
		auto label = Label::createWithTTF( cpg::input::KeyCodeNames::get_longest(), cpg::StringTable::GetFontPath(), 10 );
		return Size(
			std::ceilf( label->getContentSize().width + ( key_allow_margin.width * 2 ) )
			, std::ceilf( label->getContentSize().height + ( key_allow_margin.height * 2 ) )
		);
	}

	const std::pair<int, int> calculateKeyAllowControlsRowAndColumn( const float visible_height, const float item_height )
	{
		const auto row_count = std::div( static_cast<int>( visible_height ), static_cast<int>( item_height ) ).quot;

		const auto div_result = std::div( step_rain_of_chaos::input::KeyCodeContainerSize, row_count );
		const auto column_count = div_result.rem > 0 ? div_result.quot + 1 : div_result.quot;

		return std::make_pair( column_count, row_count );
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
		{
			const auto scroll_view_size = visibleSize;
			const Size control_size = calculateSizeOfKeyAllowControl();
			const Size control_margin( 2.f, 2.f );
			const auto item_size = control_margin + control_size + control_margin;

			const Size inner_container_margin( 20.f, 20.f );
			const float item_area_height = scroll_view_size.height - ( inner_container_margin.height * 2 );
			const auto row_n_column_count = calculateKeyAllowControlsRowAndColumn( item_area_height, item_size.height );

			const float margin_of_key_allow_control(
				0 >= row_n_column_count.second
				? 2.f
				: ( item_area_height - ( row_n_column_count.second * control_size.height ) ) / row_n_column_count.second
			);

			const Vec2 start_position(
				inner_container_margin.width + ( ( control_size.width + margin_of_key_allow_control ) * 0.5f )
				, inner_container_margin.height + ( ( control_size.height + margin_of_key_allow_control ) * 0.5f )
			);
			const Size spacing_of_control(
				( control_size.width + margin_of_key_allow_control )
				, ( control_size.height + margin_of_key_allow_control )
			);

			auto scroll_view = ui::ScrollView::create();
			scroll_view->setDirection( ui::ScrollView::Direction::HORIZONTAL );
			scroll_view->setContentSize( scroll_view_size );
			scroll_view->setInnerContainerSize( Size(
				( inner_container_margin.width * 2 )
				+ ( ( control_size.width + margin_of_key_allow_control ) * row_n_column_count.first ) - margin_of_key_allow_control
				, scroll_view_size.height
			) );
			addChild( scroll_view );

			auto root_node = Node::create();
			scroll_view->addChild( root_node );
			{
				int grid_x = 0;
				int grid_y = 0;
				for( std::size_t cur = step_rain_of_chaos::input::KeyCodeContainerFirst; step_rain_of_chaos::input::KeyCodeContainerSize > cur; ++cur )
				{
					auto control = createKeyAllowControl(
						control_size
						, static_cast<EventKeyboard::KeyCode>( cur )
						, CC_CALLBACK_2( KeyAllowScene::onKeyAllowControl, this )
					);
					control->setPosition( 
						start_position
						+ Vec2( spacing_of_control.width * grid_x, spacing_of_control.height * grid_y )
					);
					root_node->addChild( control );

					control->getChildByTag( TAG_KeyIndicator )->setVisible( mAllowedKeys[cur] );

					++grid_y;
					if( row_n_column_count.second <= grid_y )
					{
						grid_y = 0;
						++grid_x;
					}
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
