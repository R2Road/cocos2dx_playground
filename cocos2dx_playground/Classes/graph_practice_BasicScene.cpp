#include "graph_practice_BasicScene.h"

#include <new>
#include <numeric>
#include <sstream>
#include <string>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCTweenFunction.h"
#include "ui/UIScale9Sprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "graph_practice_GraphViewNode.h"

USING_NS_CC;

namespace
{
	const char* FontPath = "fonts/arial.ttf";
	const int FontSize = 9;
	const int TAG_CurrentValueIndicator = 20140416;
}

namespace graph_practice
{
	BasicScene::BasicScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mElapsedTime( 0.f )

		, mGraphViewNode_1( nullptr )
		, mGraphViewNode_2( nullptr )
		, mGraphViewNode_3( nullptr )
		, mGraphViewNode_4( nullptr )
	{}

	Scene* BasicScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) BasicScene( back_to_the_previous_scene_callback );
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

	bool BasicScene::init()
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

		
		const float StartRate = 0.15;
		const float Spacing = ( 1.0 - ( StartRate * 2 ) ) / 3;

		//
		// Practice 1
		//
		{
			mGraphViewNode_1 = GraphViewNode::create( 100, 100 );
			mGraphViewNode_1->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRate + ( Spacing * 0 ) ) ) - ( mGraphViewNode_1->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height * 0.5f - ( mGraphViewNode_1->getContentSize().height * 0.5f )
			);
			addChild( mGraphViewNode_1 );
		}

		//
		// Practice 2
		//
		{
			mGraphViewNode_2 = GraphViewNode::create( 100, 100 );
			mGraphViewNode_2->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRate + ( Spacing * 1 ) ) ) - ( mGraphViewNode_2->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height * 0.5f - ( mGraphViewNode_2->getContentSize().height * 0.5f )
			);
			addChild( mGraphViewNode_2 );
		}

		//
		// Practice 3
		//
		{
			mGraphViewNode_3 = GraphViewNode::create( 100, 100 );
			mGraphViewNode_3->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRate + ( Spacing * 2 ) ) ) - ( mGraphViewNode_3->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height * 0.5f - ( mGraphViewNode_3->getContentSize().height * 0.5f )
			);
			addChild( mGraphViewNode_3 );
		}

		//
		// Practice 4
		//
		{
			mGraphViewNode_4 = GraphViewNode::create( 100, 100 );
			mGraphViewNode_4->setPosition(
				visibleOrigin.x + ( visibleSize.width * ( StartRate + ( Spacing * 3 ) ) ) - ( mGraphViewNode_4->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height * 0.5f - ( mGraphViewNode_4->getContentSize().height * 0.5f )
			);
			addChild( mGraphViewNode_4 );
		}

		return true;
	}

	void BasicScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void BasicScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Node::onExit();
	}
	void BasicScene::update( float dt )
	{
		mElapsedTime += dt;
		if( 1.f < mElapsedTime )
		{
			mElapsedTime = 0.f;
		}

		mGraphViewNode_1->UpdateView( mElapsedTime, mElapsedTime );
		mGraphViewNode_2->UpdateView( mElapsedTime, tweenfunc::quadraticIn( mElapsedTime ) );
		mGraphViewNode_3->UpdateView( mElapsedTime, tweenfunc::quadraticOut( mElapsedTime ) );
		mGraphViewNode_4->UpdateView( mElapsedTime, tweenfunc::quadraticInOut( mElapsedTime ) );

		Node::update( dt );
	}


	void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
