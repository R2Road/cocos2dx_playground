#include "graph_practice_test_GraphViewNodeScene.h"

#include <new>
#include <numeric>
#include <string>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "graph_practice_GraphViewNode.h"

USING_NS_CC;

namespace
{
	const float TimeLimit4Graph = 3.141592f * 2.f;
	const float TimeLimit4Update = ( 3.141592f * 2.f ) + 0.1f;
	const int TAG_TileScaleView = 20140416;
}

namespace graph_practice_test
{
	GraphViewNodeScene::GraphViewNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mTimeScale( 1.f )
		, mElapsedTime( 0.f )
		, mbPause( false )

		, mGraphViewNodeContainer()
	{}

	Scene* GraphViewNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) GraphViewNodeScene( back_to_the_previous_scene_callback );
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

	bool GraphViewNodeScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		scheduleUpdate();

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
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[Arrow U/D] : Time Scale U/D";
			ss << cpg::linefeed;
			ss << "[Space] : Pause/Resume";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition(
				visibleOrigin
				+ Vec2( 0.f, visibleSize.height )
			);
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Time Scale
		//
		{
			auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
			label->setTag( TAG_TileScaleView );
			label->setAnchorPoint( Vec2( 1.f, 1.f ) );
			label->setColor( Color3B::GREEN );
			label->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width, visibleSize.height )
			);
			addChild( label, std::numeric_limits<int>::max() );

			UpdateTimeScaleView();
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 14, 20, 38, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}


		//
		// Practice x 1
		//
		{
			auto graph_view_node = graph_practice::GraphViewNode::create(
				{ true, true }
				, { 50, 50, TimeLimit4Graph, 1.5f }
				, []( float g_x )->float { return ( 0.4f * g_x ) - 1.f; }
			);
			graph_view_node->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.75f )
				- Vec2( graph_view_node->getContentSize().width * 0.5f, graph_view_node->getContentSize().height * 0.5f )
			);
			addChild( graph_view_node );

			mGraphViewNodeContainer.push_back( graph_view_node );
		}

		//
		// Practice x 2
		//
		{
			auto graph_view_node = graph_practice::GraphViewNode::create(
				{ true, true }
				, { 0, 0, TimeLimit4Graph, 1.5f }
			, []( float g_x )->float { return ( 0.4f * g_x ) - 1.f; }
			);
			graph_view_node->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.3f )
				- Vec2( graph_view_node->getContentSize().width * 0.5f, graph_view_node->getContentSize().height * 0.5f )
			);
			addChild( graph_view_node );

			mGraphViewNodeContainer.push_back( graph_view_node );
		}

		return true;
	}

	void GraphViewNodeScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GraphViewNodeScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void GraphViewNodeScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}
	void GraphViewNodeScene::update( float dt )
	{
		if( !mbPause )
		{
			mElapsedTime += ( dt * mTimeScale );
			if( TimeLimit4Update < mElapsedTime )
			{
				mElapsedTime = 0.f;
			}

			for( auto g : mGraphViewNodeContainer )
			{
				g->UpdateView( mElapsedTime );
			}
		}

		Node::update( dt );
	}


	void GraphViewNodeScene::UpdateTimeScaleView()
	{
		static_cast<Label*>( getChildByTag( TAG_TileScaleView ) )->setString( StringUtils::format( "Time Scale : %.2f", mTimeScale ) );
	}


	void GraphViewNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}

		if( EventKeyboard::KeyCode::KEY_UP_ARROW == keycode )
		{
			mTimeScale = std::min( 10.f, mTimeScale + 0.1f );
			UpdateTimeScaleView();
		}
		if( EventKeyboard::KeyCode::KEY_DOWN_ARROW == keycode )
		{
			mTimeScale = std::max( 0.1f, mTimeScale - 0.1f );
			UpdateTimeScaleView();
		}
		if( EventKeyboard::KeyCode::KEY_SPACE == keycode )
		{
			mbPause = !mbPause;
		}
	}
}
