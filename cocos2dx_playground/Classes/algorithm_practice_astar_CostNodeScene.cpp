#include "algorithm_practice_astar_CostNodeScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_node_PivotNode.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace algorithm_practice_astar
{
	CostNodeScene::CostNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* CostNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) CostNodeScene( back_to_the_previous_scene_callback );
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

	bool CostNodeScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleOrigin = _director->getVisibleOrigin();
		const auto visibleSize = _director->getVisibleSize();
		const Vec2 visibleCenter(
			visibleOrigin.x + ( visibleSize.width * 0.5f )
			, visibleOrigin.y + ( visibleSize.height * 0.5f )
		);

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[ESC] : Return to Root";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 7, Size::ZERO, TextHAlignment::LEFT );
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
			auto layer = LayerColor::create( Color4B( 8, 45, 48, 255 ) );
			addChild( layer, std::numeric_limits<int>::min() );
		}

		//
		// CostNode
		//
		{
			auto node = Node::create();
			node->setContentSize( Size( 16.f, 16.f ) );
			node->setPosition( visibleCenter );
			addChild( node );
			{
				node->addChild( cpg_node::PivotNode::create() );
			}
		}

		return true;
	}

	void CostNodeScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( CostNodeScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void CostNodeScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void CostNodeScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		switch( key_code )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
