#include "algorithm_practice_astar_ProcessorNode.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "algorithm_practice_astar_CostMapNode.h"
#include "algorithm_practice_astar_Grid4TileMap.h"

#include "cpg_Direction8.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "cpg_TileSheetUtility.h"
#include "cpg_ui_ToolBarNode.h"

#include "step_defender_game_TileMapNode.h"

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
		, mUpdateList()
		, mCurrentPoint()

		, mToolBarNode( nullptr )
		, mCostMapNode( nullptr )
		, mPathNode( nullptr )
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

		//
		// Tile Maps
		//
		{
			mPathNode = step_defender::game::TileMapNode::create(
				step_defender::game::TileMapNode::Config{ mConfig.MapWidth, mConfig.MapHeight }
				, mTileSheetConfiguration
			);
			mPathNode->setPosition(
				visibleCenter
				- Vec2( mPathNode->getContentSize().width * 0.5f, mPathNode->getContentSize().height * 0.5f )
			);
			addChild( mPathNode, 99 );
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
			mPathNode->Reset();
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


	void ProcessorNode::algorithmStep()
	{
		if( eStep::Entry == mStep )
		{
			//
			// Algorithm
			//
			Node4AStar new_node{ mGrid4TileMap->GetEntryPoint(), cpg::Point{ -1, -1 }, mGrid4TileMap->GetEntryPoint(), mGrid4TileMap->GetExitPoint() };
			mOpenList.push_back( new_node );

			//
			// ETC
			//
			mCostMapNode->Open( new_node.GetPoint().x, new_node.GetPoint().y, new_node.GetCost2Start(), new_node.GetCost2End() );
			mStep = eStep::Loop;
		}
		else if( eStep::Loop == mStep )
		{
			//
			// Algorithm
			//
			if( mOpenList.empty() )
			{
				mStep = eStep::End;
				return;
			}

			// Select Min
			Node4AStarContainerT::iterator min_itr = mOpenList.begin();
			for( auto cur = ( ++mOpenList.begin() ), end = mOpenList.end(); end != cur; ++cur )
			{
				if( min_itr->GetCost2End() > cur->GetCost2End() )
				{
					min_itr = cur;
				}
			}

			// Move
			min_itr->Close();
			const Node4AStar current_node = *min_itr;
			mCloseList.push_back( current_node );
			mOpenList.erase( min_itr );
			mUpdateList.push_back( current_node );

			// Found Exit
			if( mGrid4TileMap->GetExitPoint() == current_node.GetPoint() )
			{
				mStep = eStep::Result;
				return;
			}

			// Collect Open List
			cpg::Direction8 dir8;
			cpg::Point temp_point;
			for( int i = 0; 8 > i; ++i, dir8.Rotate( true ) )
			{
				temp_point = current_node.GetPoint() + dir8.GetPoint();

				if( !mGrid4TileMap->IsIn( temp_point.x, temp_point.y ) )
				{
					continue;
				}

				if( eCellType::Road != mGrid4TileMap->GetCellType( temp_point.x, temp_point.y ) )
				{
					continue;
				}

				if( mOpenList.end() != std::find_if( mOpenList.begin(), mOpenList.end(), [temp_point]( const Node4AStar& other_node )->bool {
					return other_node.GetPoint() == temp_point;
				} ) )
				{
					continue;
				}

				if( mCloseList.end() != std::find_if( mCloseList.begin(), mCloseList.end(), [temp_point]( const Node4AStar& other_node )->bool {
					return other_node.GetPoint() == temp_point;
				} ) )
				{
					continue;
				}

				Node4AStar new_node{ temp_point, current_node.GetPoint(), mGrid4TileMap->GetEntryPoint(), mGrid4TileMap->GetExitPoint() };
				mOpenList.push_back( new_node );
				mUpdateList.push_back( new_node );
			}

			//
			// ETC
			//
			for( const auto& u : mUpdateList )
			{
				if( Node4AStar::eStatus::Open == u.GetStatus() )
				{
					mCostMapNode->Open( u.GetPoint().x, u.GetPoint().y, u.GetCost2Start(), u.GetCost2End() );
				}
				else
				{
					mCostMapNode->Close( u.GetPoint().x, u.GetPoint().y );
				}
			}
			mUpdateList.clear();
		}
		else if( eStep::Result == mStep )
		{
			std::list<cpg::Point> result;

			auto current_itr = std::find_if( mCloseList.begin(), mCloseList.end(), [target_point = mGrid4TileMap->GetExitPoint()]( const Node4AStar& other_node )->bool {
				return other_node.GetPoint() == target_point;
			} );
			result.push_back( current_itr->GetPoint() );

			while( -1 != current_itr->GetPreviousPoint().x )
			{
				result.push_back( current_itr->GetPreviousPoint() );

				current_itr = std::find_if( mCloseList.begin(), mCloseList.end(), [target_point = current_itr->GetPreviousPoint()]( const Node4AStar& other_node )->bool {
					return other_node.GetPoint() == target_point;
				} );
			}

			for( const auto& p : result )
			{
				mPathNode->UpdateTile( p.x, p.y, 0, 4 );
			}

			mStep = eStep::End;
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
			mPathNode->Reset();
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
