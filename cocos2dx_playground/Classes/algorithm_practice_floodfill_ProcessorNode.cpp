#include "algorithm_practice_floodfill_ProcessorNode.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCTextureCache.h"

#include "algorithm_practice_floodfill_DirectionMapNode.h"
#include "algorithm_practice_floodfill_Grid4TileMap.h"

#include "cpg_StringTable.h"
#include "cpgui_ToolBarNode.h"

USING_NS_CC;

namespace algorithm_practice_floodfill
{
	ProcessorNode::ProcessorNode( const Config config, const cpg::TileSheetConfiguration& tile_sheet_configuration, const Grid4TileMap* const grid_4_tile_map ) :
		mKeyboardListener( nullptr )

		, mConfig( config )
		, mTileSheetConfiguration( tile_sheet_configuration )
		, mGrid4TileMap( grid_4_tile_map )

		, mMode( eMode::Step )
		, mElapsedTime4Loop( 0.f )
		, mStep( eStep::Entry )
		, mGrid4FloodFill()
		, mCurrentPoint()

		, mToolBarNode( nullptr )
		, mCurrentPointIndicatorNode( nullptr )
		, mDirectionMapNode( nullptr )
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
			ss << std::endl;
			ss << std::endl;
			ss << std::endl;
			ss << "[R] : " << "Reset";
			ss << std::endl;
			ss << "[Space] : " << "Step";
			ss << "[F1] : " << "Loop";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 7, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition(
				visibleOrigin
				+ Vec2( 0.f, visibleSize.height )
				- Vec2( 0.f, 20.f )
			);
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Setup Direction Grid
		//
		{
			mGrid4FloodFill.Reset( mConfig.MapWidth, mConfig.MapHeight );
		}

		//
		// Tool Bar - for Mode
		//
		{
			mToolBarNode = cpgui::ToolBarNode::create( ui::Layout::Type::VERTICAL, Size( 80.f, 20.f ) );
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
			auto texture = Director::getInstance()->getTextureCache()->getTextureForKey( mTileSheetConfiguration.GetTexturePath() );

			cpg::TileSheetUtility tile_sheet_utility;
			tile_sheet_utility.Setup(
				mTileSheetConfiguration.GetTileWidth(), mTileSheetConfiguration.GetTileHeight()
				, mTileSheetConfiguration.GetTileMargin_Width(), mTileSheetConfiguration.GetTileMargin_Height()
				, texture->getContentSizeInPixels().height
			);

			auto sprite = Sprite::createWithTexture( texture );
			sprite->setAnchorPoint( Vec2::ZERO );
			sprite->setScale( _director->getContentScaleFactor() );
			sprite->setTextureRect( tile_sheet_utility.ConvertTilePoint2TextureRect( 0, 4 ) );
			sprite->setVisible( false );
			addChild( sprite, 1 );

			mCurrentPointIndicatorNode = sprite;
		}

		//
		// Direction Maps
		//
		{
			mDirectionMapNode = DirectionMapNode::create( DirectionMapNode::Config{ mConfig.MapWidth, mConfig.MapHeight } );
			mDirectionMapNode->setPosition(
				visibleCenter
				- Vec2( mDirectionMapNode->getContentSize().width * 0.5f, mDirectionMapNode->getContentSize().height * 0.5f )
			);
			addChild( mDirectionMapNode );
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

			for( auto& d : mGrid4FloodFill )
			{
				d.Clear();
			}
			mDirectionMapNode->Reset();

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
			mDirectionMapNode->getPosition()
			+ Vec2( mTileSheetConfiguration.GetTileWidth() * mCurrentPoint.x, mTileSheetConfiguration.GetTileHeight() * mCurrentPoint.y )
		);
	}


	void ProcessorNode::algorithmStep()
	{
		if( eStep::Entry == mStep )
		{
			mStep = eStep::Loop;
			auto& current_cell = mGrid4FloodFill.Get( mGrid4TileMap->GetEntryPoint().x, mGrid4TileMap->GetEntryPoint().y );
			current_cell.Begin( { -1, -1 }, cpg::Direction4::eState::None );
			mDirectionMapNode->UpdateTile( mGrid4TileMap->GetEntryPoint().x, mGrid4TileMap->GetEntryPoint().y, current_cell.GetTotalDirection() );

			mCurrentPoint = mGrid4TileMap->GetEntryPoint();
			mCurrentPointIndicatorNode->setVisible( true );
			updateCurrentPointView();
		}
		else if( eStep::Loop == mStep )
		{
			auto& current_cell = mGrid4FloodFill.Get( mCurrentPoint.x, mCurrentPoint.y );
			if( current_cell.HasDirection() )
			{
				const auto current_direction = current_cell.PopDirection();
				mDirectionMapNode->UpdateTile( mCurrentPoint.x, mCurrentPoint.y, current_cell.GetTotalDirection() );

				auto new_point = mCurrentPoint + current_direction.GetPoint();
				if( mGrid4FloodFill.Get( new_point.x, new_point.y ).IsValid() )
				{
					return;
				}

				if( !mGrid4FloodFill.IsIn( new_point.x, new_point.y ) )
				{
					return;
				}

				if( eCellType::Road == mGrid4TileMap->GetCellType( new_point.x, new_point.y ) )
				{
					auto& next_cell = mGrid4FloodFill.Get( new_point.x, new_point.y );
					next_cell.Begin( mCurrentPoint, current_direction );
					mDirectionMapNode->UpdateTile( new_point.x, new_point.y, next_cell.GetTotalDirection() );

					mCurrentPoint = new_point;
					updateCurrentPointView();
				}
			}
			else
			{
				mCurrentPoint = current_cell.GetParentPoint();
				updateCurrentPointView();

				if( -1 == mCurrentPoint.x )
				{
					mStep = eStep::End;
				}
			}
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
			for( auto& d : mGrid4FloodFill )
			{
				d.Clear();
			}
			mDirectionMapNode->Reset();

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
