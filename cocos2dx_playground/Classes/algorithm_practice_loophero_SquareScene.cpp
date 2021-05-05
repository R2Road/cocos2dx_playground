#include "algorithm_practice_loophero_SquareScene.h"

#include <functional>
#include <fstream>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "cocos/platform/CCFileUtils.h"
#include "renderer/CCTextureCache.h"

#include "cpg_StringTable.h"
#include "cpg_TileSheetConfiguration.h"
#include "cpg_ui_ToolBarNode.h"

#include "step_defender_game_TileMapNode.h"

USING_NS_CC;

namespace
{
	const std::size_t GRID_WIDTH = 21;
	const std::size_t GRID_HEIGHT = 13;
}

namespace algorithm_practice_loophero
{
	SquareScene::SquareScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mGrid4TileMap()
		, mTileMapNode( nullptr )

		, mWay()
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
		mTileMapNode->FillAll( 1, 0 );

		int pivot_x = 6;
		int pivot_y = 10;
		for( int i = 0 ; 10 > i; ++i )
		{
			mWay.push_back( { pivot_x + i, pivot_y } );
		}

		pivot_x = mWay.rbegin()->x;
		pivot_y = mWay.rbegin()->y - 1;
		for( int i = 0; 7 > i; ++i )
		{
			mWay.push_back( { pivot_x, pivot_y - i } );
		}

		pivot_x = mWay.rbegin()->x - 1;
		pivot_y = mWay.rbegin()->y;
		for( int i = 0; 10 > i; ++i )
		{
			mWay.push_back( { pivot_x - i, pivot_y } );
		}

		pivot_x = mWay.rbegin()->x;
		pivot_y = mWay.rbegin()->y + 1;
		for( int i = 0; 7 > i; ++i )
		{
			mWay.push_back( { pivot_x, pivot_y + i } );
		}

		for( const auto& p : mWay )
		{
			mTileMapNode->UpdateTile( p.x, p.y, 0, 0 );
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
