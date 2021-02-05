#include "graph_practice_Collection01Scene.h"

#include <new>
#include <numeric>
#include <sstream>
#include <string>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCTweenFunction.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_StringTable.h"
#include "graph_practice_GraphAndNameNode.h"

USING_NS_CC;

namespace
{
	const int TAG_TileScaleView = 20140416;
}

namespace graph_practice
{
	Collection01Scene::Collection01Scene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mTimeScale( 1.f )
		, mElapsedTime( 0.f )
		, mbPause( false )

		, mGraphViewNodeContainer()
	{}

	Scene* Collection01Scene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) Collection01Scene( back_to_the_previous_scene_callback );
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

	bool Collection01Scene::init()
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
			ss << std::endl;
			ss << std::endl;
			ss << "[ESC] : Return to Root";
			ss << std::endl;
			ss << std::endl;
			ss << "[Arrow Up/Down] : Time Scale Up/Down";
			ss << std::endl;
			ss << "[Space] : Pause/Resume";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition( Vec2(
				visibleOrigin.x
				, visibleOrigin.y + visibleSize.height
			) );
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
			label->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width
				, visibleOrigin.y + visibleSize.height
			) );
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

		
		const int GraphWidth = 110;
		const int GraphHeight = 110;
		const float StartRateX = 0.15f;
		const float SpacingX = ( 1.0f - ( StartRateX * 2 ) ) / 3;
		const float StartRateY = 0.25;
		const float SpacingY = 0.5;

		//
		// Practice 1
		//
		{
			auto graph_view_node = GraphAndNameNode::create( "Linear", GraphWidth, GraphHeight, []( float g_x )->float { return g_x; } );
			graph_view_node->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRateX + ( SpacingX * 0 ) ) ) - ( graph_view_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f ) - ( graph_view_node->getContentSize().height * 0.5f )
			);
			addChild( graph_view_node );

			mGraphViewNodeContainer.push_back( graph_view_node );
		}

		//
		// Practice 2
		//
		{
			auto graph_view_node = GraphAndNameNode::create( "quadratic In", GraphWidth, GraphHeight, []( float g_x )->float { return tweenfunc::quadraticIn( g_x ); } );
			graph_view_node->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRateX + ( SpacingX * 1 ) ) ) - ( graph_view_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * ( StartRateY + ( SpacingY * 0 ) ) ) - ( graph_view_node->getContentSize().height * 0.5f )
			);
			addChild( graph_view_node );

			mGraphViewNodeContainer.push_back( graph_view_node );
		}

		//
		// Practice 3
		//
		{
			auto graph_view_node = GraphAndNameNode::create( "quadratic Out", GraphWidth, GraphHeight, []( float g_x )->float { return tweenfunc::quadraticOut( g_x ); } );
			graph_view_node->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRateX + ( SpacingX * 2 ) ) ) - ( graph_view_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * ( StartRateY + ( SpacingY * 0 ) ) ) - ( graph_view_node->getContentSize().height * 0.5f )
			);
			addChild( graph_view_node );

			mGraphViewNodeContainer.push_back( graph_view_node );
		}

		//
		// Practice 4
		//
		{
			auto graph_view_node = GraphAndNameNode::create( "quadratic In Out", GraphWidth, GraphHeight, []( float g_x )->float { return tweenfunc::quadraticInOut( g_x ); } );
			graph_view_node->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRateX + ( SpacingX * 3 ) ) ) - ( graph_view_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * ( StartRateY + ( SpacingY * 0 ) ) ) - ( graph_view_node->getContentSize().height * 0.5f )
			);
			addChild( graph_view_node );

			mGraphViewNodeContainer.push_back( graph_view_node );
		}



		//
		// Practice 5
		//
		{
			auto graph_view_node = GraphAndNameNode::create( "sineEase In", GraphWidth, GraphHeight, []( float g_x )->float { return tweenfunc::sineEaseIn( g_x ); } );
			graph_view_node->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRateX + ( SpacingX * 1 ) ) ) - ( graph_view_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * ( StartRateY + ( SpacingY * 1 ) ) ) - ( graph_view_node->getContentSize().height * 0.5f )
			);
			addChild( graph_view_node );

			mGraphViewNodeContainer.push_back( graph_view_node );
		}

		//
		// Practice 6
		//
		{
			auto graph_view_node = GraphAndNameNode::create( "sineEase Out", GraphWidth, GraphHeight, []( float g_x )->float { return tweenfunc::sineEaseOut( g_x ); } );
			graph_view_node->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRateX + ( SpacingX * 2 ) ) ) - ( graph_view_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * ( StartRateY + ( SpacingY * 1 ) ) ) - ( graph_view_node->getContentSize().height * 0.5f )
			);
			addChild( graph_view_node );

			mGraphViewNodeContainer.push_back( graph_view_node );
		}

		//
		// Practice 7
		//
		{
			auto graph_view_node = GraphAndNameNode::create( "sineEase In Out", GraphWidth, GraphHeight, []( float g_x )->float { return tweenfunc::sineEaseInOut( g_x ); } );
			graph_view_node->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRateX + ( SpacingX * 3 ) ) ) - ( graph_view_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * ( StartRateY + ( SpacingY * 1 ) ) ) - ( graph_view_node->getContentSize().height * 0.5f )
			);
			addChild( graph_view_node );

			mGraphViewNodeContainer.push_back( graph_view_node );
		}

		return true;
	}

	void Collection01Scene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( Collection01Scene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void Collection01Scene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}
	void Collection01Scene::update( float dt )
	{
		if( !mbPause )
		{
			mElapsedTime += ( dt * mTimeScale );
			if( 1.1f < mElapsedTime )
			{
				mElapsedTime = 0.f;
			}

			for( auto g : mGraphViewNodeContainer )
			{
				g->UpdateView( std::min( 1.f, mElapsedTime ) );
			}
		}

		Node::update( dt );
	}


	void Collection01Scene::UpdateTimeScaleView()
	{
		static_cast<Label*>( getChildByTag( TAG_TileScaleView ) )->setString( StringUtils::format( "Time Scale : %.2f", mTimeScale ) );
	}


	void Collection01Scene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
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
