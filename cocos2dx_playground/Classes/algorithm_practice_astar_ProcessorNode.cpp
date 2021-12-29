#include "algorithm_practice_astar_ProcessorNode.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCTextureCache.h"

#include "algorithm_practice_astar_CostMapNode.h"
#include "algorithm_practice_Grid4TileMap.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "cpg_TileSheetUtility.h"
#include "cpg_ui_ToolBarNode.h"

USING_NS_CC;

namespace algorithm_practice_astar
{
	ProcessorNode::ProcessorNode( const Config config, const cpg::TileSheetConfiguration& tile_sheet_configuration, const Grid4TileMap* const grid_4_tile_map ) :
		mKeyboardListener( nullptr )

		, mConfig( config )
		, mTileSheetConfiguration( tile_sheet_configuration )
		, mGrid4TileMap( grid_4_tile_map )

		, mMode( eMode::Step )
		, mElapsedTime4Loop( 0.f )
		, mStep( eStep::Entry )

		, mOpenList()
		, mCloseList()
		, mCurrentPoint()

		, mToolBarNode( nullptr )
		, mCurrentPointIndicatorNode( nullptr )
		, mCostMapNode( nullptr )
	{}

	ProcessorNode* ProcessorNode::create( const Config config, const cpg::TileSheetConfiguration& tile_sheet_configuration, const Grid4TileMap* const grid_4_tile_map )
	{
		auto ret = new ( std::nothrow ) ProcessorNode( config, tile_sheet_configuration, grid_4_tile_map );
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

	bool ProcessorNode::init()
	{
		if( !Node::init() )
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
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[R] : " << "Reset";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[Space] : " << "Step";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 11, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition(
				visibleOrigin
				+ Vec2( 0.f, visibleSize.height )
				- Vec2( 0.f, 20.f )
			);
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Tool Bar - for Mode
		//
		{
			mToolBarNode = cpg_ui::ToolBarNode::create( ui::Layout::Type::VERTICAL, Size( 80.f, 20.f ) );
			addChild( mToolBarNode );

			mToolBarNode->AddTool( static_cast<int>( eMode::Step ), "Step Mode", 10, std::bind( &ProcessorNode::onModeSelect, this, static_cast<int>( eMode::Step ) ) );
			mToolBarNode->AddTool( static_cast<int>( eMode::Loop ), "Loop Mode", 10, std::bind( &ProcessorNode::onModeSelect, this, static_cast<int>( eMode::Loop ) ) );

			mToolBarNode->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width, visibleSize.height )
				+ Vec2( -mToolBarNode->getContentSize().width, -mToolBarNode->getContentSize().height )
			);

			// Set Indicator
			mToolBarNode->SelectTool( static_cast<int>( mMode ) );
		}

		//
		// Current Point Indicator
		//
		{
			auto draw_node = DrawNode::create();
			draw_node->drawRect( Vec2::ZERO, Vec2( mTileSheetConfiguration.GetTileWidth(), mTileSheetConfiguration.GetTileHeight() ), Color4F::RED );
			draw_node->setVisible( false );
			addChild( draw_node, 1 );

			mCurrentPointIndicatorNode = draw_node;
		}

		//
		// CostMapNode
		//
		{
			mCostMapNode = CostMapNode::create( mConfig.MapWidth, mConfig.MapHeight, Size( mTileSheetConfiguration.GetTileWidth(), mTileSheetConfiguration.GetTileHeight() ) );
			mCostMapNode->setPosition(
				visibleCenter
				- Vec2( mCostMapNode->getContentSize().width * 0.5f, mCostMapNode->getContentSize().height * 0.5f )
			);
			addChild( mCostMapNode );
		}

		return true;
	}


	void ProcessorNode::onEnter()
	{
		Node::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ProcessorNode::onKeyPressed, this );
		mKeyboardListener->setEnabled( isVisible() );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void ProcessorNode::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Node::onExit();
	}


	void ProcessorNode::setVisible( bool visible )
	{
		Node::setVisible( visible );

		if( !visible )
		{
			mMode = eMode::Step;
			mToolBarNode->SelectTool( static_cast<int>( mMode ) );
			unschedule( schedule_selector( ProcessorNode::algorithmLoop ) );

			mStep = eStep::Entry;

			mOpenList.clear();
			mCloseList.clear();

			mCostMapNode->Reset();

			mCurrentPointIndicatorNode->setVisible( false );
		}

		if( mKeyboardListener )
		{
			mKeyboardListener->setEnabled( visible );
		}
	}


	void ProcessorNode::onModeSelect( const int mode_index )
	{
		mMode = static_cast<eMode>( mode_index );
		CCLOG( "Mode Index : %d", mode_index );

		switch( mMode )
		{
		case eMode::Step:
			if( isScheduled( schedule_selector( ProcessorNode::algorithmLoop ) ) )
			{
				unschedule( schedule_selector( ProcessorNode::algorithmLoop ) );
			}
			break;
		case eMode::Loop:
			if( !isScheduled( schedule_selector( ProcessorNode::algorithmLoop ) ) )
			{
				schedule( schedule_selector( ProcessorNode::algorithmLoop ) );
			}
			break;
		}
	}


	void ProcessorNode::updateCurrentPointView()
	{
		mCurrentPointIndicatorNode->setPosition(
			mCostMapNode->getPosition()
			+ Vec2( mTileSheetConfiguration.GetTileWidth() * mCurrentPoint.x, mTileSheetConfiguration.GetTileHeight() * mCurrentPoint.y )
		);
	}


	void ProcessorNode::algorithmStep()
	{
		if( eStep::Entry == mStep )
		{
			// Do Something
		}
		else if( eStep::Loop == mStep )
		{
			// Do Something
		}
	}
	void ProcessorNode::algorithmLoop( float dt )
	{
		mElapsedTime4Loop += dt;

		if( 0.02f < mElapsedTime4Loop )
		{
			mElapsedTime4Loop = 0.f;

			algorithmStep();

			if( eStep::End == mStep )
			{
				mMode = eMode::Step;
				mToolBarNode->SelectTool( static_cast<int>( mMode ) );
				unschedule( schedule_selector( ProcessorNode::algorithmLoop ) );
			}
		}
	}


	void ProcessorNode::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_R == key_code )
		{
			mStep = eStep::Entry;

			mOpenList.clear();
			mCloseList.clear();

			mCostMapNode->Reset();

			mCurrentPointIndicatorNode->setVisible( false );
			return;
		}

		if( EventKeyboard::KeyCode::KEY_SPACE == key_code )
		{
			if( eMode::Step == mMode )
			{
				algorithmStep();
			}
		}

		if( EventKeyboard::KeyCode::KEY_F1 == key_code )
		{
			if( eMode::Step == mMode )
			{
				mMode = eMode::Loop;
			}
			else
			{
				mMode = eMode::Step;
			}
		}
	}
}
