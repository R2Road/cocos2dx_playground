#include "graph_practice_test_GraphAndNameNodeScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_StringTable.h"
#include "graph_practice_GraphAndNameNode.h"

USING_NS_CC;

namespace graph_practice_test
{
	GraphAndNameNodeScene::GraphAndNameNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mGraphAndNameNode( nullptr )
		, mElapsedTime( 0.f )
	{}

	Scene* GraphAndNameNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) GraphAndNameNodeScene( back_to_the_previous_scene_callback );
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

	bool GraphAndNameNodeScene::init()
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

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition(
				visibleOrigin
				+ Vec2( 0.f, visibleSize.height )
			);
			addChild( label, std::numeric_limits<int>::max() );
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
			mGraphAndNameNode = graph_practice::GraphAndNameNode::create( { true, true }, "Linear", 100, 100, []( float g_x )->float { return g_x; } );
			mGraphAndNameNode->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				- Vec2( mGraphAndNameNode->getContentSize().width * 0.5f, mGraphAndNameNode->getContentSize().height * 0.5f )
			);
			addChild( mGraphAndNameNode );
		}

		return true;
	}

	void GraphAndNameNodeScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GraphAndNameNodeScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void GraphAndNameNodeScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}
	void GraphAndNameNodeScene::update( float dt )
	{
		mElapsedTime += dt;
		if( 1.1f < mElapsedTime )
		{
			mElapsedTime = 0.f;
		}

		mGraphAndNameNode->UpdateView( std::min( 1.f, mElapsedTime ) );

		Node::update( dt );
	}


	void GraphAndNameNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
