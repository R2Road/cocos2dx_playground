#include "algorithm_practice_astar_CostMapNodeScene.h"

#include <new>
#include <numeric>

#include "2d/CCDrawNode.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "algorithm_practice_astar_CostMapNode.h"

#include "cpg_Clamp.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const struct
	{
		int w = 10;
		int h = 10;
	} CostMapBlockAmount;
	const Size CostNodeSize( 16.f, 16.f );
}

namespace algorithm_practice_astar
{
	CostMapNodeScene::CostMapNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mCostMapNode( nullptr )
		, mIndicatorNode( nullptr )

		, mIndicatorPointX( 0 )
		, mIndicatorPointY( 0 )
	{}

	Scene* CostMapNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) CostMapNodeScene( back_to_the_previous_scene_callback );
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

	bool CostMapNodeScene::init()
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
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[Arrow] : Move Indicator";
			ss << cpg::linefeed;
			ss << "[SPACE] : Open";
			ss << cpg::linefeed;
			ss << "[Z] : Close";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[R] : Reset";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 7 );
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
		// CostMapNode
		//
		{
			mCostMapNode = CostMapNode::create( CostMapBlockAmount.w, CostMapBlockAmount.h, CostNodeSize );
			mCostMapNode->setPosition(
				visibleCenter
				- Vec2( mCostMapNode->getContentSize().width * 0.5f, mCostMapNode->getContentSize().height * 0.5f )
			);
			addChild( mCostMapNode );
		}

		//
		// Indicator
		//
		{
			auto draw_node = DrawNode::create();
			draw_node->drawRect( Vec2::ZERO, Vec2( CostNodeSize.width, CostNodeSize.height ), Color4F::RED );
			addChild( draw_node, 1 );

			mIndicatorNode = draw_node;
		}

		//
		//
		//
		moveIndicator( 0, 0 );

		return true;
	}

	void CostMapNodeScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( CostMapNodeScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void CostMapNodeScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void CostMapNodeScene::requestMoveIndicator( const int move_x, const int move_y )
	{
		moveIndicator( mIndicatorPointX + move_x, mIndicatorPointY + move_y );
	}
	void CostMapNodeScene::moveIndicator( const int new_x, const int new_y )
	{
		mIndicatorPointX = cpg::clamp( new_x, 0, CostMapBlockAmount.w - 1 );
		mIndicatorPointY = cpg::clamp( new_y, 0, CostMapBlockAmount.h - 1 );

		mIndicatorNode->setPosition(
			mCostMapNode->getPosition()
			+ Vec2( CostNodeSize.width * mIndicatorPointX, CostNodeSize.height * mIndicatorPointY )
		);
	}


	void CostMapNodeScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		switch( key_code )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			requestMoveIndicator( -1, 0 );
			return;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			requestMoveIndicator( 1, 0 );
			return;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			requestMoveIndicator( 0, 1 );
			return;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			requestMoveIndicator( 0, -1 );
			return;

		case EventKeyboard::KeyCode::KEY_SPACE:
			mCostMapNode->Open( mIndicatorPointX, mIndicatorPointY, 11, 111 );
			return;
		case EventKeyboard::KeyCode::KEY_Z:
			mCostMapNode->Close( mIndicatorPointX, mIndicatorPointY );
			return;

		case EventKeyboard::KeyCode::KEY_R:
			mCostMapNode->Reset();
			return;
		}
	}
}
