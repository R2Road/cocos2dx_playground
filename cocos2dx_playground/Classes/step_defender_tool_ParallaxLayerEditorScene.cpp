#include "step_defender_tool_ParallaxLayerEditorScene.h"

#include <functional>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCParallaxNode.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"

#include "cpg_StringTable.h"
#include "cpgui_ToolBarNode.h"

#include "step_defender_game_Constant.h"
#include "step_defender_game_TileMapNode.h"
#include "step_defender_tool_TileSheetNode.h"

USING_NS_CC;

namespace
{
	const float ScrollSpeed = 300.f;

	const Color3B BackgroundColors[3] = {
		{ 90u, 200u, 255u }
		, Color3B::YELLOW
		, Color3B::WHITE
	};
}

namespace step_defender
{
	namespace tool
	{
		ParallaxLayerEditorScene::ParallaxLayerEditorScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mKeyCodeCollector()
			, mPosition2GridIndexConverter( game::TileSheetConfig.GetTileWidth(), game::TileSheetConfig.GetTileHeight() )

			, mRulerNode( nullptr )
			, mParallaxNode( nullptr )
			, mTouchNode( nullptr )
			, mCurrentBackgroundLayer( nullptr )
			
			, mCurrentTilePoint( { 0, 0 } )
			, mToolIndex( 0 )
		{}

		Scene* ParallaxLayerEditorScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) ParallaxLayerEditorScene( back_to_the_previous_scene_callback );
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

		bool ParallaxLayerEditorScene::init()
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

			setContentSize( game::WorldConfig.WorldSize );

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << "[Arrow L/R] : Do Parralax";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 7, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}
			
			//
			// Sky
			//
			{
				auto layer = LayerColor::create( game::WorldConfig.SkyColor );
				addChild( layer, std::numeric_limits<int>::min() );
			}

			//
			// Layer Selector
			//
			{
				auto tool_bar_node = cpgui::ToolBarNode::create();
				addChild( tool_bar_node, std::numeric_limits<int>::max() );

				tool_bar_node->AddTool( 1, "1", 10, std::bind( &ParallaxLayerEditorScene::onLayerSelect, this, 0 ) );
				tool_bar_node->AddTool( 2, "2", 10, std::bind( &ParallaxLayerEditorScene::onLayerSelect, this, 1 ) );
				tool_bar_node->AddTool( 3, "3", 10, std::bind( &ParallaxLayerEditorScene::onLayerSelect, this, 2 ) );

				tool_bar_node->setPosition(
					visibleCenter.x - ( tool_bar_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + visibleSize.height - tool_bar_node->getContentSize().height
				);

				// Set Indicator
				tool_bar_node->SelectTool( 3 );
			}

			//
			// Tool Selector
			//
			{
				auto tool_bar_node = cpgui::ToolBarNode::create( ui::Layout::Type::VERTICAL );
				addChild( tool_bar_node, std::numeric_limits<int>::max() );

				tool_bar_node->AddTool( 0, "step_defender_tool_icon_01_0.png", std::bind( &ParallaxLayerEditorScene::onToolSelect, this, 0 ) );
				tool_bar_node->AddTool( 1, "step_defender_tool_icon_01_1.png", std::bind( &ParallaxLayerEditorScene::onToolSelect, this, 1 ) );

				tool_bar_node->setPosition(
					visibleOrigin.x + visibleSize.width - tool_bar_node->getContentSize().width
					, visibleOrigin.y + visibleSize.height - tool_bar_node->getContentSize().height
				);

				// Set Indicator
				tool_bar_node->SelectTool( mToolIndex );
			}

			//
			// Tile Sheet Node
			//
			{
				auto tile_sheet_node = step_defender::tool::TileSheetNode::create( game::TileSheetConfig );
				tile_sheet_node->setPosition(
					Vec2( visibleOrigin.x + visibleSize.width, visibleCenter.y )
					- Vec2( tile_sheet_node->getContentSize().width + 10.f, tile_sheet_node->getContentSize().height * 0.5f )
				);
				tile_sheet_node->SetSelectCallback( CC_CALLBACK_2( ParallaxLayerEditorScene::onTileSelect, this ) );
				addChild( tile_sheet_node, std::numeric_limits<int>::max() );
			}

			//
			// Touch Node
			//
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_4.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setAnchorPoint( Vec2::ZERO );
				button->setScale9Enabled( true );
				button->setContentSize( visibleSize );
				button->setPosition( visibleOrigin );
				button->addTouchEventListener( CC_CALLBACK_2( ParallaxLayerEditorScene::onUpdateTile, this ) );
				addChild( button, std::numeric_limits<int>::max() - 1 );

				mTouchNode = button;
			}

			//
			// ParallaxNode Setup - Ruler
			//
			{
				const int Parallax_Ruler_Part_Width = 100.f;

				mRulerNode = ParallaxNode::create();
				addChild( mRulerNode, 1 );

				for( const auto& c : game::ParallaxNodeConfigContainer )
				{
					const float ruler_y = game::WorldConfig.WorldSize.height - ( game::WorldConfig.WorldSize.height * ( 0.2f + ( 0.05f * c.Index ) ) );

					auto background_node = Node::create();
					background_node->setTag( c.Index );
					background_node->setCascadeOpacityEnabled( true );
					mRulerNode->addChild( background_node, c.Index, Vec2( c.Rate, 1.f ), Vec2::ZERO );

					const auto background_width = ( game::WorldConfig.WorldSize.width * c.Rate ) + visibleSize.width;
					const auto div_result = std::div( static_cast<int>( background_width ), Parallax_Ruler_Part_Width );
					for( int i = 0, end = div_result.quot + ( div_result.rem > 0 ? 1 : 0 ); end > i; ++i )
					{
						auto label = Label::createWithTTF( std::to_string( i * Parallax_Ruler_Part_Width ), cpg::StringTable::GetFontPath(), 6 + c.Index );
						label->setAnchorPoint( Vec2( 0.f, 1.f ) );
						label->setColor( BackgroundColors[c.Index] );
						label->setPosition( Vec2(
							i * Parallax_Ruler_Part_Width
							, ruler_y
						) );
						background_node->addChild( label, std::numeric_limits<int>::max() );
					}

					//
					// Tail Guide
					//
					{
						auto layer = LayerColor::create( Color4B::GREEN, 5.f, ruler_y );
						layer->setPositionX( game::WorldConfig.WorldSize.width * c.Rate );
						background_node->addChild( layer, 1 );
					}
				}
			}

			//
			// ParallaxNode Setup - Main
			//
			{
				setContentSize( game::WorldConfig.WorldSize );

				mParallaxNode = ParallaxNode::create();
				addChild( mParallaxNode, 0 );

				const auto height_div_result = std::div( static_cast<int>( game::WorldConfig.WorldSize.height ), game::TileSheetConfig.GetTileHeight() );

				for( const auto& c : game::ParallaxNodeConfigContainer )
				{
					const auto parallax_width = ( game::WorldConfig.WorldSize.width * c.Rate ) + visibleSize.width;
					const auto width_div_result = std::div( static_cast<int>( parallax_width ), game::TileSheetConfig.GetTileWidth() );

					auto tile_map_node = game::TileMapNode::create(
						step_defender::game::TileMapNode::Config{
							width_div_result.quot + ( width_div_result.rem > 0 ? 1 : 0 )
							, height_div_result.quot + ( height_div_result.rem > 0 ? 1 : 0 )
						}
						, game::TileSheetConfig
					);
					tile_map_node->setTag( c.Index );
					mParallaxNode->addChild( tile_map_node, c.Index, Vec2( c.Rate, 1.f ), Vec2::ZERO );
				}
			}

			//
			// Setup
			//
			schedule( schedule_selector( ParallaxLayerEditorScene::update4Move ) );
			onLayerSelect( 2 );

			return true;
		}

		void ParallaxLayerEditorScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ParallaxLayerEditorScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( ParallaxLayerEditorScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ParallaxLayerEditorScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void ParallaxLayerEditorScene::update4Move( float delta_time )
		{
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) )
			{
				const auto new_position = mRulerNode->getPositionX() + ( -ScrollSpeed * delta_time );
				if( -getContentSize().width < new_position )
				{
					mRulerNode->setPositionX( new_position );
					mParallaxNode->setPositionX( new_position );
				}
				else
				{
					mRulerNode->setPositionX( -getContentSize().width );
					mParallaxNode->setPositionX( -getContentSize().width );
				}
			}

			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_LEFT_ARROW ) )
			{
				const auto new_position = mRulerNode->getPositionX() + ( ScrollSpeed * delta_time );
				if( 0.f > new_position )
				{
					mRulerNode->setPositionX( new_position );
					mParallaxNode->setPositionX( new_position );
				}
				else
				{
					mRulerNode->setPositionX( 0.f );
					mParallaxNode->setPositionX( 0.f );
				}
			}
		}


		void ParallaxLayerEditorScene::onLayerSelect( const int layer_index )
		{
			for( auto c : mRulerNode->getChildren() )
			{
				if( layer_index == c->getTag() )
				{
					c->setOpacity( 255u );
				}
				else
				{
					c->setOpacity( 60u );
				}
			}

			for( auto c : mParallaxNode->getChildren() )
			{
				if( layer_index >= c->getTag() )
				{
					mCurrentBackgroundLayer = static_cast<game::TileMapNode*>( c );
					c->setOpacity( 255u );
				}
				else
				{
					c->setOpacity( 60u );
				}
			}

			mTouchNode->setColor( BackgroundColors[layer_index] );
		}
		void ParallaxLayerEditorScene::onToolSelect( const int tool_index )
		{
			CCLOG( "%d", tool_index );
			mToolIndex = tool_index;
		}
		void ParallaxLayerEditorScene::onTileSelect( const int x, const int y )
		{
			mCurrentTilePoint.x = x;
			mCurrentTilePoint.y = y;
		}


		void ParallaxLayerEditorScene::onUpdateTile( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( 0 == mToolIndex )
			{
				onAddTile( sender, touch_event_type );
			}
			else
			{
				onEraseTile( sender, touch_event_type );
			}
		}
		void ParallaxLayerEditorScene::onAddTile( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			auto button = static_cast<ui::Button*>( sender );

			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				const auto pos = mCurrentBackgroundLayer->convertToNodeSpace( button->getTouchBeganPosition() );
				const auto point = mPosition2GridIndexConverter.Position2Point( pos.x, pos.y );
				CCLOG( "B : %d, %d", point.x, point.y );

				mCurrentBackgroundLayer->UpdateTile( point.x, point.y, mCurrentTilePoint.x, mCurrentTilePoint.y );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				const auto pos = mCurrentBackgroundLayer->convertToNodeSpace( button->getTouchMovePosition() );
				const auto point = mPosition2GridIndexConverter.Position2Point( pos.x, pos.y );
				CCLOG( "M : %d, %d", point.x, point.y );

				mCurrentBackgroundLayer->UpdateTile( point.x, point.y, mCurrentTilePoint.x, mCurrentTilePoint.y );
			}
			else if( ui::Widget::TouchEventType::ENDED == touch_event_type || ui::Widget::TouchEventType::CANCELED == touch_event_type )
			{
				const auto pos = mCurrentBackgroundLayer->convertToNodeSpace( button->getTouchEndPosition() );
				const auto point = mPosition2GridIndexConverter.Position2Point( pos.x, pos.y );
				CCLOG( "E : %d, %d", point.x, point.y );

				mCurrentBackgroundLayer->UpdateTile( point.x, point.y, mCurrentTilePoint.x, mCurrentTilePoint.y );
			}
		}
		void ParallaxLayerEditorScene::onEraseTile( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			auto button = static_cast<ui::Button*>( sender );

			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				const auto pos = mCurrentBackgroundLayer->convertToNodeSpace( button->getTouchBeganPosition() );
				const auto point = mPosition2GridIndexConverter.Position2Point( pos.x, pos.y );
				CCLOG( "E : %d, %d", point.x, point.y );

				mCurrentBackgroundLayer->EraseTile( point.x, point.y );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				const auto pos = mCurrentBackgroundLayer->convertToNodeSpace( button->getTouchMovePosition() );
				const auto point = mPosition2GridIndexConverter.Position2Point( pos.x, pos.y );
				CCLOG( "E : %d, %d", point.x, point.y );

				mCurrentBackgroundLayer->EraseTile( point.x, point.y );
			}
			else if( ui::Widget::TouchEventType::ENDED == touch_event_type || ui::Widget::TouchEventType::CANCELED == touch_event_type )
			{
				const auto pos = mCurrentBackgroundLayer->convertToNodeSpace( button->getTouchEndPosition() );
				const auto point = mPosition2GridIndexConverter.Position2Point( pos.x, pos.y );
				CCLOG( "E : %d, %d", point.x, point.y );

				mCurrentBackgroundLayer->EraseTile( point.x, point.y );
			}
		}


		void ParallaxLayerEditorScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
			mKeyCodeCollector.onKeyPressed( key_code );
		}
		void ParallaxLayerEditorScene::onKeyReleased( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			mKeyCodeCollector.onKeyReleased( key_code );
		}
	}
}
