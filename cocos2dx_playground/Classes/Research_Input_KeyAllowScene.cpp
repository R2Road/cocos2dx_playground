#include "Research_Input_KeyAllowScene.h"

#include <cmath> // ceil
#include <sstream>
#include <cstdlib> // ldiv
#include <utility> // pair

#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIScrollView.h"
#include "CPG_Input_KeyCodeNames.h"

#include "RootScene.h"
#include "Research_Setting.h"

USING_NS_CC;

namespace research
{
	namespace input
	{
		namespace
		{
			const int TAG_KeyAllowControl_BG = 20140416;
			const Size calculateSizeOfKeyAllowControl( const char* str )
			{
				const Size key_allow_margin( 8.f, 4.f );
				auto temp = Label::createWithTTF( str, "fonts/arial.ttf", 10 );
				return Size(
					std::ceilf( temp->getContentSize().width + ( key_allow_margin.width * 2 ) )
					, std::ceilf( temp->getContentSize().height + ( key_allow_margin.height * 2 ) )
				);
			}
			const std::pair<int,int> calculateKeyAllowControlsRowAndColumn( const Size view_size, const Size control_size, const Size control_margin )
			{
				const auto _row_count = ldiv(
					static_cast<int>( view_size.height )
					, static_cast<int>( control_size.height + control_margin.height )
				).quot;

				const auto div_result = std::ldiv( cpg::input::AllowedKeys::ContainerSize, _row_count );

				return std::make_pair(
					div_result.rem > 0 ? div_result.quot + 1 : div_result.quot
					, _row_count
				);
			}

			Node* createKeyAllowControl( const Size control_size, const EventKeyboard::KeyCode target_key_code, const ui::Widget::ccWidgetTouchCallback& callback )
			{
				auto key_allow_control_root = Node::create();
				{
					auto key_allow_label = Label::createWithTTF( cpg::input::KeyCodeNames::get( target_key_code ), "fonts/arial.ttf", 10, Size::ZERO, TextHAlignment::CENTER );
					key_allow_control_root->addChild( key_allow_label, 2 );

					auto button = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_1.png", ui::Widget::TextureResType::PLIST );
					button->setTag( static_cast<int>( target_key_code ) );
					button->getRendererNormal()->getTexture()->setAliasTexParameters();
					button->getRendererClicked()->getTexture()->setAliasTexParameters();
					button->getRendererDisabled()->getTexture()->setAliasTexParameters();
					button->setScale9Enabled( true );
					button->setContentSize( control_size );
					button->addTouchEventListener( callback );
					key_allow_control_root->addChild( button, 1 );

					auto indicator = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
					indicator->setTag( TAG_KeyAllowControl_BG );
					indicator->setVisible( false );
					indicator->setContentSize( control_size );
					key_allow_control_root->addChild( indicator, 0 );
				}

				return key_allow_control_root;
			}
		}

		KeyAllowScene::KeyAllowScene() :
			mKeyboardListener( nullptr )
			, mGoExit( false )
			, mAllowedKeys()
		{}

		Scene* KeyAllowScene::create()
		{
			auto ret = new ( std::nothrow ) KeyAllowScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			ret->autorelease();

			return ret;
		}

		bool KeyAllowScene::init()
		{
			if( !Scene::init() )
				return false;

			auto visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();



			//
			// Summury
			//
			static const auto tab_char = "    ";
			std::stringstream ss;
			ss << "+ Key Allow Scene";
			ss << tab_char;
			ss << tab_char;
			ss << "[ESC] : Save & Exit";
			ss << tab_char;
			ss << tab_char;
			ss << "[Mouse] : Horizontal Scrolling";

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
			label->setColor( Color3B::GREEN );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition( Vec2(
				origin.x
				, origin.y + visibleSize.height
			) );
			addChild( label, 9999 );



			//
			// Load Allowed Keys
			//
			mAllowedKeys = cpg::input::AllowedKeys::load( research::Setting::getKeyAllowFileName().c_str() );



			//
			// Setup Key Allow Controls
			//
			auto scroll_view = ui::ScrollView::create();
			scroll_view->setDirection( ui::ScrollView::Direction::HORIZONTAL );
			scroll_view->setContentSize( visibleSize );
			addChild( scroll_view );

			auto key_allow_controls_root = Node::create();
			key_allow_controls_root->setPosition( Vec2( origin.x, origin.y ) );
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
					origin.x + ( side_margin.width * 2 ) + ( ( size_of_key_allow_control.width + margin_of_key_allow_control ) * row_n_column_count.first ) - margin_of_key_allow_control
					, visibleSize.height
				) );

				const Vec2 start_position(
					origin.x + side_margin.width + ( ( size_of_key_allow_control.width + margin_of_key_allow_control ) * 0.5f )
					, origin.y + side_margin.height + ( ( size_of_key_allow_control.height + margin_of_key_allow_control ) * 0.5f )
				);
				const Size spacing_of_control(
					( size_of_key_allow_control.width + margin_of_key_allow_control )
					, ( size_of_key_allow_control.height + margin_of_key_allow_control )
				);

				int grid_x = 0;
				int grid_y = 0;
				for( std::size_t cur = cpg::input::AllowedKeys::ContainerFirst; cpg::input::AllowedKeys::ContainerSize > cur; ++cur )
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

					auto bg = key_allow_control_root->getChildByTag( TAG_KeyAllowControl_BG );
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

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( KeyAllowScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void KeyAllowScene::onExit()
		{
			if( mKeyboardListener )
			{
				getEventDispatcher()->removeEventListener( mKeyboardListener );
				mKeyboardListener = nullptr;
			}
			Node::onExit();
		}

		void KeyAllowScene::updateForExit( float /*dt*/ )
		{
			cpg::input::AllowedKeys::save( mAllowedKeys, research::Setting::getKeyAllowFileName().c_str() );
			Director::getInstance()->replaceScene( RootScene::create() );
		}
		void KeyAllowScene::onKeyAllowControl( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::ENDED != touch_event_type )
				return;

			auto button = static_cast<ui::Button*>( sender );
			auto bg = button->getParent()->getChildByTag( TAG_KeyAllowControl_BG );
			bg->setVisible( !bg->isVisible() );

			mAllowedKeys[button->getTag()] = bg->isVisible();
		}

		void KeyAllowScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE != keycode )
				return;

			if( mGoExit )
				return;

			mGoExit = true;
			scheduleOnce( schedule_selector( KeyAllowScene::updateForExit ), 0.f );
		}
	}
}
