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

#include "graph_practice_GraphViewNode.h"

USING_NS_CC;

namespace
{
	const char* FontPath = "fonts/arial.ttf";
}

namespace graph_practice
{
	Collection01Scene::Collection01Scene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mElapsedTime( 0.f )

		, mGraphViewNode_1( nullptr )
		, mGraphViewNode_2( nullptr )
		, mGraphViewNode_3( nullptr )
		, mGraphViewNode_4( nullptr )
	{}

	Scene* Collection01Scene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) Collection01Scene( back_to_the_previous_scene_callback );
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

	bool Collection01Scene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		scheduleUpdate();

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

		
		const float StartRateX = 0.15;
		const float SpacingX = ( 1.0 - ( StartRateX * 2 ) ) / 3;
		const float StartRateY = 0.3;
		const float SpacingY = 0.5;

		//
		// Practice 1
		//
		{
			mGraphViewNode_1 = GraphViewNode::create( "Linear", 100, 100, []( float g_x )->float { return g_x; } );
			mGraphViewNode_1->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRateX + ( SpacingX * 0 ) ) ) - ( mGraphViewNode_1->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height * StartRateY - ( mGraphViewNode_1->getContentSize().height * 0.5f )
			);
			addChild( mGraphViewNode_1 );
		}

		//
		// Practice 2
		//
		{
			mGraphViewNode_2 = GraphViewNode::create( "quadratic In", 100, 100, []( float g_x )->float { return tweenfunc::quadraticIn( g_x ); } );
			mGraphViewNode_2->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRateX + ( SpacingX * 1 ) ) ) - ( mGraphViewNode_2->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height * StartRateY - ( mGraphViewNode_2->getContentSize().height * 0.5f )
			);
			addChild( mGraphViewNode_2 );
		}

		//
		// Practice 3
		//
		{
			mGraphViewNode_3 = GraphViewNode::create( "quadratic Out", 100, 100, []( float g_x )->float { return tweenfunc::quadraticOut( g_x ); } );
			mGraphViewNode_3->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRateX + ( SpacingX * 2 ) ) ) - ( mGraphViewNode_3->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height * StartRateY - ( mGraphViewNode_3->getContentSize().height * 0.5f )
			);
			addChild( mGraphViewNode_3 );
		}

		//
		// Practice 4
		//
		{
			mGraphViewNode_4 = GraphViewNode::create( "quadratic In Out", 100, 100, []( float g_x )->float { return tweenfunc::quadraticInOut( g_x ); } );
			mGraphViewNode_4->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRateX + ( SpacingX * 3 ) ) ) - ( mGraphViewNode_4->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height * StartRateY - ( mGraphViewNode_4->getContentSize().height * 0.5f )
			);
			addChild( mGraphViewNode_4 );
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

		Node::onExit();
	}
	void Collection01Scene::update( float dt )
	{
		mElapsedTime += dt;
		if( 1.f < mElapsedTime )
		{
			mElapsedTime = 0.f;
		}

		mGraphViewNode_1->UpdateView( mElapsedTime );
		mGraphViewNode_2->UpdateView( mElapsedTime );
		mGraphViewNode_3->UpdateView( mElapsedTime );
		mGraphViewNode_4->UpdateView( mElapsedTime );

		Node::update( dt );
	}


	void Collection01Scene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
