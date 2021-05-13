#include "algorithm_practice_loophero_SquareScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_Clamp.h"
#include "cpg_Random.h"
#include "cpg_StringTable.h"
#include "cpg_TileSheetConfiguration.h"

#include "step_defender_game_TileMapNode.h"

#include "algorithm_practice_loophero_Constant.h"

USING_NS_CC;

namespace algorithm_practice_loophero
{
	SquareScene::SquareScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mTileMapNode( nullptr )
		, mRoad()
	{}

	Scene* SquareScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) SquareScene( back_to_the_previous_scene_callback );
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

	bool SquareScene::init()
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
		// Load Tile Config
		//
		cpg::TileSheetConfiguration tile_sheet_configuration( 1, 1, 1, 1, "" );
		CCASSERT( tile_sheet_configuration.Load( "datas/algorithm_practice/algorithm_practice_tile_sheet_config_01.json" ), "Failed - Load Tile Sheet Configuration" );

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << std::endl;
			ss << std::endl;
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
		// Tile Maps
		//
		{
			mTileMapNode = step_defender::game::TileMapNode::create(
				step_defender::game::TileMapNode::Config{ GRID_WIDTH, GRID_HEIGHT }
				, tile_sheet_configuration
			);
			mTileMapNode->setPosition(
				visibleCenter
				- Vec2( mTileMapNode->getContentSize().width * 0.5f, mTileMapNode->getContentSize().height * 0.5f )
			);
			addChild( mTileMapNode );
		}

		//
		// Setup
		//
		{
			mTileMapNode->FillAll( 3, 0 );

			// 0. Ready
			const int required_road_count = algorithm_practice_loophero::ROAD_LENGTH - algorithm_practice_loophero::ROAD_PIVOT_COUNT;

			// 1. Adjust Pivot List
			auto CurrentPivotList = algorithm_practice_loophero::PIVOT_LIST;
			{
				const int required_half_road_count = ( algorithm_practice_loophero::ROAD_LENGTH - algorithm_practice_loophero::ROAD_PIVOT_COUNT ) / 2;

				const auto square_pivot_size = cpg::Random::GetInt( static_cast<int>( required_half_road_count * 0.4f ), static_cast<int>( required_half_road_count * 0.9f ) );

				const auto square_width = required_half_road_count - square_pivot_size > square_pivot_size ? required_half_road_count - square_pivot_size : square_pivot_size;
				const auto square_height = required_half_road_count - square_width;

				CurrentPivotList[1].x += square_width;
				CurrentPivotList[2].x += square_width;

				CurrentPivotList[0].y += square_height;
				CurrentPivotList[1].y += square_height;

				CCLOG( "width : %d, height : %d", square_width, square_height );
			}
			
			// 2. Make Road
			for( int i = 0; ROAD_PIVOT_COUNT > i; ++i )
			{
				const auto start_point = CurrentPivotList[i];
				const auto end_point = i + 1 < ROAD_PIVOT_COUNT ? CurrentPivotList[i + 1] : CurrentPivotList[0];

				const auto start2end = end_point - start_point;
				const cpg::Point dir{
					cpg::clamp( start2end.x, -1, 1 )
					, cpg::clamp( start2end.y, -1, 1 )
				};

				auto cur_point = start_point;
				do
				{
					mRoad.push_back( cur_point );

					cur_point += dir;
				} while( end_point != cur_point );
			}

			for( const auto& p : mRoad )
			{
				mTileMapNode->UpdateTile( p.x, p.y, 2, 0 );
			}

			CCASSERT( ROAD_LENGTH == mRoad.size(), "Not Enough Way Length" );
		}

		return true;
	}

	void SquareScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SquareScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void SquareScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void SquareScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
