#include "graph_practice_SineScene.h"

#include <cmath>
#include <new>
#include <numeric>
#include <sstream>
#include <string>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_StringTable.h"
#include "graph_practice_GraphViewNode.h"

USING_NS_CC;

namespace
{
	const float TimeLimit4Graph = 3.141592f * 2.f;
	const float TimeLimit4Update = ( 3.141592f * 2.f ) + 0.1f;

	const int TAG_TileScaleView = 20140416;
}

namespace graph_practice
{
	SineScene::SineScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mTimeScale( 1.f )
		, mElapsedTime( 0.f )
		, mbPause( false )

		, mGraphViewNode( nullptr )
	{}

	Scene* SineScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) SineScene( back_to_the_previous_scene_callback );
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

	bool SineScene::init()
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

			updateTimeScaleView();
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 14, 20, 38, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// Practice
		//
		{
			mGraphViewNode = graph_practice::GraphViewNode::create(
				{ false, false }
				, { 50, 50, TimeLimit4Graph, 1.5f }
			, []( float g_x )->float { return std::sin( g_x ); }
			);
			mGraphViewNode->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f )
				- Vec2( mGraphViewNode->getContentSize().width * 0.5f, mGraphViewNode->getContentSize().height * 0.5f )
			);
			addChild( mGraphViewNode );
		}

		//
		// Setup
		//
		schedule( schedule_selector( SineScene::update4Graph ) );

		return true;
	}

	void SineScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SineScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void SineScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}
	void SineScene::update4Graph( float dt )
	{
		if( !mbPause )
		{
			mElapsedTime += ( dt * mTimeScale );
			if( TimeLimit4Update < mElapsedTime )
			{
				mElapsedTime = 0.f;
			}

			mGraphViewNode->UpdateView( mElapsedTime );
		}
	}


	void SineScene::updateTimeScaleView()
	{
		static_cast<Label*>( getChildByTag( TAG_TileScaleView ) )->setString( StringUtils::format( "Time Scale : %.2f", mTimeScale ) );
	}


	void SineScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}

		if( EventKeyboard::KeyCode::KEY_UP_ARROW == keycode )
		{
			mTimeScale = std::min( 10.f, mTimeScale + 0.1f );
			updateTimeScaleView();
		}
		if( EventKeyboard::KeyCode::KEY_DOWN_ARROW == keycode )
		{
			mTimeScale = std::max( 0.1f, mTimeScale - 0.1f );
			updateTimeScaleView();
		}
		if( EventKeyboard::KeyCode::KEY_SPACE == keycode )
		{
			mbPause = !mbPause;
		}
	}
}
