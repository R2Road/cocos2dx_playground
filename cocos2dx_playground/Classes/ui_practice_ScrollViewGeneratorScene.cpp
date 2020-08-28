#include "ui_practice_ScrollViewGeneratorScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpgui_ScrollViewGenerator.h"

USING_NS_CC;

namespace
{
	const char* FontPath = "fonts/NanumSquareR.ttf";
}

namespace ui_practice
{
	ScrollViewGeneratorScene::ScrollViewGeneratorScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* ScrollViewGeneratorScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) ScrollViewGeneratorScene( back_to_the_previous_scene_callback );
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

	bool ScrollViewGeneratorScene::init()
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

			auto label = Label::createWithTTF( ss.str(), FontPath, 9, Size::ZERO, TextHAlignment::LEFT );
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
		// Practice x 1
		// - Item Count : 0
		// - Visible Count : 0
		//
		{
			cpgui::ScrollViewGenerator::ItemContainerT item_info_container;
			const std::size_t visible_count = 0u;

			auto scroll_view = cpgui::ScrollViewGenerator::Create(
				""
				, item_info_container
				, CC_CALLBACK_2( ScrollViewGeneratorScene::onDummyButton, this )
				, 0
				, Size( 100, 18 )
			);
			scroll_view->setPosition( Vec2(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.2f, visibleSize.height * 0.45f )
				- Vec2( scroll_view->getContentSize().width * 0.5f, scroll_view->getContentSize().height * 0.5f )
			) );
			addChild( scroll_view );

			//
			// Explain
			//
			{
				auto label = Label::createWithTTF(
					StringUtils::format( "Item Count : %d\nVisible Count %d", item_info_container.size(), visible_count )
					, FontPath, 9, Size::ZERO, TextHAlignment::LEFT
				);
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					scroll_view->getPosition()
					+ Vec2( scroll_view->getContentSize().width * 0.5f, scroll_view->getContentSize().height + 10.f )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}
		}

		//
		// Practice x 2
		// - Item Count : 0
		// - Visible Count : 10
		//
		{
			const std::size_t visible_count = 10u;
			cpgui::ScrollViewGenerator::ItemContainerT item_info_container;

			auto scroll_view = cpgui::ScrollViewGenerator::Create(
				""
				, item_info_container
				, CC_CALLBACK_2( ScrollViewGeneratorScene::onDummyButton, this )
				, visible_count
				, Size( 100, 18 )
			);
			scroll_view->setPosition( Vec2(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.45f )
				- Vec2( scroll_view->getContentSize().width * 0.5f, scroll_view->getContentSize().height * 0.5f )
			) );
			addChild( scroll_view );

			//
			// Explain
			//
			{
				auto label = Label::createWithTTF(
					StringUtils::format( "Item Count : %d\nVisible Count %d", item_info_container.size(), visible_count )
					, FontPath, 9, Size::ZERO, TextHAlignment::LEFT
				);
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					scroll_view->getPosition()
					+ Vec2( scroll_view->getContentSize().width * 0.5f, scroll_view->getContentSize().height + 10.f )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}
		}

		//
		// Practice x 3
		// - Item Count : 40
		// - Visible Count : 10
		//
		{
			const std::size_t visible_count = 10u;
			cpgui::ScrollViewGenerator::ItemContainerT item_info_container;
			item_info_container.emplace_back( -1, "-1" );
			for( int i = 1; 40 > i; ++i )
			{
				item_info_container.emplace_back( i, std::to_string( i ) );
			}

			auto scroll_view = cpgui::ScrollViewGenerator::Create(
				""
				, item_info_container
				, CC_CALLBACK_2( ScrollViewGeneratorScene::onDummyButton, this )
				, visible_count
				, Size( 100, 18 )
			);
			scroll_view->setPosition( Vec2(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.8f, visibleSize.height * 0.45f )
				- Vec2( scroll_view->getContentSize().width * 0.5f, scroll_view->getContentSize().height * 0.5f )
			) );
			addChild( scroll_view );

			//
			// Explain
			//
			{
				auto label = Label::createWithTTF(
					StringUtils::format( "Item Count : %d\nVisible Count %d", item_info_container.size(), visible_count )
					, FontPath, 9, Size::ZERO, TextHAlignment::LEFT
				);
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					scroll_view->getPosition()
					+ Vec2( scroll_view->getContentSize().width * 0.5f, scroll_view->getContentSize().height + 10.f )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}
		}

		return true;
	}

	void ScrollViewGeneratorScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ScrollViewGeneratorScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void ScrollViewGeneratorScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void ScrollViewGeneratorScene::onDummyButton( Ref* sender, ui::Widget::TouchEventType touchEventType )
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

	void ScrollViewGeneratorScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
