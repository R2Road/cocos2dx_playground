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

#include "cpgui_ToolBarNode.h"

#include "step_defender_game_Constant.h"
#include "step_defender_game_TileMapNode.h"

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
			, mGridIndexConverter( game::TileSheetConfig.TileWidth, game::TileSheetConfig.TileHeight )

			, mRulerNode( nullptr )
			, mParallaxNode( nullptr )
			, mTouchNode( nullptr )
			, mCurrentBackgroundLayer( nullptr )
			, mStage( nullptr )
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
				ss << "+ " << getTitle();
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow L/R] : Do Parralax";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
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
				tool_bar_node->SelectTool( 1 );
			}

			//
			// Touch Node
			//
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setAnchorPoint( Vec2::ZERO );
				button->setScale9Enabled( true );
				button->setContentSize( visibleSize );
				button->setPosition( visibleOrigin );
				button->addTouchEventListener( CC_CALLBACK_2( ParallaxLayerEditorScene::onButton, this ) );
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
						auto label = Label::createWithTTF( std::to_string( i * Parallax_Ruler_Part_Width ), "fonts/NanumSquareR.ttf", 6 + c.Index );
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

				const auto height_div_result = std::div( static_cast<int>( game::WorldConfig.WorldSize.height ), game::TileSheetConfig.TileHeight );

				for( const auto& c : game::ParallaxNodeConfigContainer )
				{
					const auto parallax_width = ( game::WorldConfig.WorldSize.width * c.Rate ) + visibleSize.width;
					const auto width_div_result = std::div( static_cast<int>( parallax_width ), game::TileSheetConfig.TileWidth );

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
			// Stage Area
			//
			{
				mStage = LayerColor::create( Color4B( 160u, 160u, 160u, 160u ), game::WorldConfig.BottomSize.width, game::WorldConfig.BottomSize.height );
				addChild( mStage, 1 );
			}

			//
			// Setup
			//
			schedule( schedule_selector( ParallaxLayerEditorScene::update4Move ) );
			onLayerSelect( 0 );

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
					mParallaxNode->setPositionX( new_position );
				}

				mStage->setPosition( mRulerNode->getPosition() );
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
					mParallaxNode->setPositionX( new_position );
				}

				mStage->setPosition( mRulerNode->getPosition() );
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
				if( layer_index == c->getTag() )
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


		void ParallaxLayerEditorScene::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			auto button = static_cast<ui::Button*>( sender );

			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				const auto pos = mCurrentBackgroundLayer->convertToNodeSpace( button->getTouchBeganPosition() );
				const auto point = mGridIndexConverter.Position2Point( pos.x, pos.y );
				CCLOG( "B : %d, %d", point.x, point.y );

				mCurrentBackgroundLayer->UpdateTile( point.x, point.y, 1, 0 );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				const auto pos = mCurrentBackgroundLayer->convertToNodeSpace( button->getTouchMovePosition() );
				const auto point = mGridIndexConverter.Position2Point( pos.x, pos.y );
				CCLOG( "M : %d, %d", point.x, point.y );

				mCurrentBackgroundLayer->UpdateTile( point.x, point.y, 1, 0 );
			}
			else if( ui::Widget::TouchEventType::ENDED == touch_event_type || ui::Widget::TouchEventType::CANCELED == touch_event_type )
			{
				const auto pos = mCurrentBackgroundLayer->convertToNodeSpace( button->getTouchEndPosition() );
				const auto point = mGridIndexConverter.Position2Point( pos.x, pos.y );
				CCLOG( "E : %d, %d", point.x, point.y );

				mCurrentBackgroundLayer->UpdateTile( point.x, point.y, 1, 0 );
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
