#include "ui_research_layout_research_OptionBarNodeScene.h"

#include <new> // std::nothrow
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "cpg_ui_ToolBarNode.h"

USING_NS_CC;

namespace
{
	const int TAG_Toolbar_FillNClear = 100;
}

namespace cpgui
{
	namespace research_layout
	{
		OptionBarNodeScene::OptionBarNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) : helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* OptionBarNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new( std::nothrow ) OptionBarNodeScene( back_to_the_previous_scene_callback );
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

		bool OptionBarNodeScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visible_origin = _director->getVisibleOrigin();
			const auto visible_size = _director->getVisibleSize();

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[1] : Fill";
				ss << cpg::linefeed;
				ss << "[2] : Clear";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->getFontAtlas()->setAliasTexParameters();
				label->setAnchorPoint( Vec2( 0.0f, 1.0f ) );
				label->setPosition(
					visible_origin
					+ Vec2( 0.f, visible_size.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 90, 80, 80, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Test Setup : Horizontal
			//
			{
				auto tool_bar_node = cpg_ui::ToolBarNode::create();
				addChild( tool_bar_node );

				tool_bar_node->AddTool( 1, "step_rain_of_chaos_actor_01_idle_0.png", []() { CCLOG( "1" ); } );
				tool_bar_node->AddTool( 2, Sprite::createWithSpriteFrameName( "step_rain_of_chaos_actor_01_idle_1.png" ), []() { CCLOG( "2" ); } );
				tool_bar_node->AddTool( 3, "step_rain_of_chaos_actor_01_idle_2.png", []() { CCLOG( "3" ); } );

				tool_bar_node->setPosition(
					visible_origin
					+ Vec2( getContentSize().width * 0.2f, getContentSize().height * 0.6f )
					- Vec2( tool_bar_node->getContentSize().width * 0.5f, tool_bar_node->getContentSize().height * 0.5f )
				);

				// Set Indicator
				tool_bar_node->SelectTool( 1 );
			}

			//
			// Test Setup : Vertical
			//
			{
				auto tool_bar_node = cpg_ui::ToolBarNode::create( ui::Layout::Type::VERTICAL );
				addChild( tool_bar_node );

				tool_bar_node->AddTool( 1, "step_rain_of_chaos_actor_01_idle_0.png", []() { CCLOG( "1" ); } );
				tool_bar_node->AddTool( 2, Sprite::createWithSpriteFrameName( "step_rain_of_chaos_actor_01_idle_1.png" ), []() { CCLOG( "2" ); } );
				tool_bar_node->AddTool( 3, "step_rain_of_chaos_actor_01_idle_2.png", []() { CCLOG( "3" ); } );

				tool_bar_node->setPosition(
					visible_origin
					+ Vec2( getContentSize().width * 0.5f, getContentSize().height * 0.6f )
					- Vec2( tool_bar_node->getContentSize().width * 0.5f, tool_bar_node->getContentSize().height * 0.5f )
				);

				// Set Indicator
				tool_bar_node->SelectTool( 2 );
			}

			//
			// Test Setup : Size, Margin
			//
			{
				auto tool_bar_node = cpg_ui::ToolBarNode::create( ui::Layout::Type::VERTICAL, Size( 40.f, 40.f ), Size( 4.f, 8.f ) );
				addChild( tool_bar_node );

				tool_bar_node->AddTool( 1, "step_rain_of_chaos_actor_01_idle_0.png", []() { CCLOG( "1" ); } );
				tool_bar_node->AddTool( 2, Sprite::createWithSpriteFrameName( "step_rain_of_chaos_actor_01_idle_1.png" ), []() { CCLOG( "2" ); } );
				tool_bar_node->AddTool( 3, "step_rain_of_chaos_actor_01_idle_2.png", []() { CCLOG( "3" ); } );

				tool_bar_node->setPosition(
					visible_origin
					+ Vec2( getContentSize().width * 0.8f, getContentSize().height * 0.6f )
					- Vec2( tool_bar_node->getContentSize().width * 0.5f, tool_bar_node->getContentSize().height * 0.5f )
				);

				// Set Indicator
				tool_bar_node->SelectTool( 3 );
			}

			//
			// Test Setup : Fill, Clear
			//
			{
				auto tool_bar_node = cpg_ui::ToolBarNode::create();
				tool_bar_node->setTag( TAG_Toolbar_FillNClear );
				addChild( tool_bar_node );
				tool_bar_node->setPosition(
					visible_origin
					+ Vec2( getContentSize().width * 0.5f, getContentSize().height * 0.1f )
					- Vec2( tool_bar_node->getContentSize().width * 0.5f, tool_bar_node->getContentSize().height * 0.5f )
				);
			}

			return true;
		}

		void OptionBarNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( OptionBarNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void OptionBarNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void OptionBarNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			if( EventKeyboard::KeyCode::KEY_1 == keycode )
			{
				auto tool_bar_node = static_cast<cpg_ui::ToolBarNode*>( getChildByTag( TAG_Toolbar_FillNClear ) );

				tool_bar_node->AddTool( 1, "step_rain_of_chaos_actor_01_idle_0.png", []() { CCLOG( "1" ); } );
				tool_bar_node->AddTool( 2, "step_rain_of_chaos_actor_01_idle_1.png", []() { CCLOG( "2" ); } );
				tool_bar_node->AddTool( 3, "step_rain_of_chaos_actor_01_idle_2.png", []() { CCLOG( "3" ); } );

				tool_bar_node->setPosition(
					( getContentSize().width * 0.5f ) - ( tool_bar_node->getContentSize().width * 0.5f )
					, ( getContentSize().height * 0.1f ) - ( tool_bar_node->getContentSize().height * 0.5f )
				);
			}

			if( EventKeyboard::KeyCode::KEY_2 == keycode )
			{
				auto tool_bar_node = static_cast<cpg_ui::ToolBarNode*>( getChildByTag( TAG_Toolbar_FillNClear ) );
				tool_bar_node->ClearTools();

				tool_bar_node->setPosition(
					( getContentSize().width * 0.5f ) - ( tool_bar_node->getContentSize().width * 0.5f )
					, ( getContentSize().height * 0.1f ) - ( tool_bar_node->getContentSize().height * 0.5f )
				);
			}
		}
	}
}