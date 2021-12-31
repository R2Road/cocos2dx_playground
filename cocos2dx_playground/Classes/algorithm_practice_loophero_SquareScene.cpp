#include "algorithm_practice_loophero_SquareScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "cpg_TileSheetConfiguration.h"

#include "step_defender_game_TileMapNode.h"

#include "algorithm_practice_loophero_Constant.h"
#include "algorithm_practice_loophero_SquareBuilder.h"

USING_NS_CC;

namespace algorithm_practice_loophero
{
	SquareScene::SquareScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mSizeView( nullptr )
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
		cpg::TileSheetConfiguration tile_sheet_configuration;
		CCASSERT( tile_sheet_configuration.Load( "datas/algorithm_practice/algorithm_practice_tile_sheet_config_01.json" ), "Failed - Load Tile Sheet Configuration" );

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
		// Size View
		//
		{
			mSizeView = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 14, Size::ZERO, TextHAlignment::RIGHT );
			mSizeView->setAnchorPoint( Vec2( 1.f, 1.f ) );
			mSizeView->setColor( Color3B::GREEN );
			mSizeView->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width, visibleSize.height )
			);
			addChild( mSizeView, std::numeric_limits<int>::max() );
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
			Build();
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


	void SquareScene::Build()
	{
		mTileMapNode->FillAll( 3, 0 );

		const auto square_size = SquareBuilder::Build( &mRoad );

		// Show
		mSizeView->setString( StringUtils::format(
			"W : %d, H : %d\nTotal : %d"
			, square_size.x
			, square_size.y
			, mRoad.size()
		) );

		for( const auto& p : mRoad )
		{
			mTileMapNode->UpdateTile( p.x, p.y, 2, 0 );
		}
	}


	void SquareScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}

		if( EventKeyboard::KeyCode::KEY_R == key_code )
		{
			Build();
		}
	}
}
