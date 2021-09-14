#include "ui_practice_ScrollViewGeneratorScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "cpg_ui_ScrollViewGenerator.h"

USING_NS_CC;

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
		// Practice x 1
		// - Item Count : 0
		// - Visible Count : 0
		//
		{
			const cpg_ui::ScrollViewGenerator::Config config{ 8u, 0u, Size( 100, 18 ) };
			cpg_ui::ScrollViewGenerator::ItemContainerT item_info_container;

			auto scroll_view = cpg_ui::ScrollViewGenerator::Create(
				config
				, "Practice 1"
				, item_info_container
				, CC_CALLBACK_2( ScrollViewGeneratorScene::onDummyButton, this )
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
					StringUtils::format( "Item Count : %d\nVisible Count %d", item_info_container.size(), config.ItemVisibleCount )
					, cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT
				);
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					scroll_view->getPosition()
					+ Vec2( scroll_view->getContentSize().width * 0.5f, scroll_view->getContentSize().height + 20.f )
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
			const cpg_ui::ScrollViewGenerator::Config config{ 8u, 10u, Size( 100, 18 ) };
			cpg_ui::ScrollViewGenerator::ItemContainerT item_info_container;

			auto scroll_view = cpg_ui::ScrollViewGenerator::Create(
				config
				, "Practice 2"
				, item_info_container
				, CC_CALLBACK_2( ScrollViewGeneratorScene::onDummyButton, this )
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
					StringUtils::format( "Item Count : %d\nVisible Count %d", item_info_container.size(), config.ItemVisibleCount )
					, cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT
				);
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					scroll_view->getPosition()
					+ Vec2( scroll_view->getContentSize().width * 0.5f, scroll_view->getContentSize().height + 20.f )
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
			const cpg_ui::ScrollViewGenerator::Config config{ 8u, 10u, Size( 100, 18 ) };
			cpg_ui::ScrollViewGenerator::ItemContainerT item_info_container;
			item_info_container.emplace_back( -1, "-1" );
			for( int i = 1; 40 > i; ++i )
			{
				item_info_container.emplace_back( i, std::to_string( i ) );
			}

			auto scroll_view = cpg_ui::ScrollViewGenerator::Create(
				config
				, ""
				, item_info_container
				, CC_CALLBACK_2( ScrollViewGeneratorScene::onDummyButton, this )
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
					StringUtils::format( "Item Count : %d\nVisible Count %d", item_info_container.size(), config.ItemVisibleCount )
					, cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT
				);
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					scroll_view->getPosition()
					+ Vec2( scroll_view->getContentSize().width * 0.5f, scroll_view->getContentSize().height + 20.f )
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
