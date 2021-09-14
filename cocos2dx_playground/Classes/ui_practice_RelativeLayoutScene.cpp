#include "ui_practice_RelativeLayoutScene.h"

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

namespace ui_practice
{
	RelativeLayoutScene::RelativeLayoutScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* RelativeLayoutScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) RelativeLayoutScene( back_to_the_previous_scene_callback );
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

	bool RelativeLayoutScene::init()
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
			const Size ButtonSize( 18, 18 );
			const Size ButtonMargin( 1, 1 );

			auto layout_node = ui::Layout::create();
			layout_node->setContentSize( Size( 160, 160 ) );
			layout_node->setLayoutType( ui::Layout::Type::RELATIVE );
			layout_node->setClippingEnabled( false );
			layout_node->setBackGroundColor( Color3B::BLUE );
			layout_node->setBackGroundColorOpacity( 150u );
			layout_node->setBackGroundColorType( cocos2d::ui::Layout::BackGroundColorType::SOLID );
			layout_node->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.25f ) - ( layout_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f ) - ( layout_node->getContentSize().height * 0.5f )
			) );
			addChild( layout_node );

			// Pivot
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 4.f );
				pivot->setPosition( layout_node->getPosition() );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			// Content
			int cur = static_cast<int>( ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT );
			const int end = static_cast<int>( ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM );
			for( ; end >= cur; ++cur )
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setTag( cur );
				button->setScale9Enabled( true );
				button->setContentSize( ButtonSize );
				button->addTouchEventListener( CC_CALLBACK_2( RelativeLayoutScene::onDummyButton, this ) );

				// Align
				{
					auto param = ui::RelativeLayoutParameter::create();
					{
						param->setAlign( static_cast<ui::RelativeLayoutParameter::RelativeAlign>( cur ) );

						auto margin = ui::Margin( ButtonMargin.width, ButtonMargin.height, ButtonMargin.width, ButtonMargin.height );
						param->setMargin( margin );
					}

					button->setLayoutParameter( param );
				}

				// Title
				{
					auto label = Label::createWithTTF( std::to_string( cur ), cpg::StringTable::GetFontPath(), 9 );
					button->setTitleLabel( label );
				}

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					button->addChild( pivot, std::numeric_limits<int>::max() );
				}

				layout_node->addChild( button );
			}
		}

		return true;
	}

	void RelativeLayoutScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( RelativeLayoutScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void RelativeLayoutScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void RelativeLayoutScene::onDummyButton( Ref* sender, ui::Widget::TouchEventType touchEventType )
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

	void RelativeLayoutScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
