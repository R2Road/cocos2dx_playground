#include "cocos_research_render_Prerender2Scene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCRenderTexture.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCRenderer.h"

#include "cpg_node_PivotNode.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "cpg_TileSheetConfiguration.h"

#include "step_defender_game_TileMapNode.h"

#include "cocos_research_render_CameraNode.h"

USING_NS_CC;

namespace cocos_research_render
{
	Prerender2Scene::Prerender2Scene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mActorNode( nullptr )
		, mCaptureTargetNode( nullptr )
		, mRenderTextureNode( nullptr )

		, mbInputBlock( false )
		, mCapturePivot()
	{}

	Scene* Prerender2Scene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) Prerender2Scene( back_to_the_previous_scene_callback );
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

	bool Prerender2Scene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleSize = _director->getVisibleSize();
		const auto visibleOrigin = _director->getVisibleOrigin();
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
			ss << "[Arrow] : Move Actor + Capture";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 8 );
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
			// Left
			{
				auto background_layer = LayerColor::create( Color4B( 5, 29, 81, 255 ), visibleSize.width * 0.5f, visibleSize.height );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			// Right
			{
				auto background_layer = LayerColor::create( Color4B::BLACK, visibleSize.width * 0.5f, visibleSize.height );
				background_layer->setPositionX( visibleCenter.x );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}
		}

		//
		// Research
		//
		{
			const step_defender::game::TileMapNode::Config stage_config{ 6u, 6u };

			// Load Tile Config
			cpg::TileSheetConfiguration tile_sheet_configuration;
			CCASSERT( tile_sheet_configuration.Load( "datas/algorithm_practice/algorithm_practice_tile_sheet_config_01.json" ), "Failed - Load Tile Sheet Configuration" );

			mCaptureTargetNode = Node::create();
			mCaptureTargetNode->setContentSize( Size(
				tile_sheet_configuration.GetTileWidth() * stage_config.MapWidth
				, tile_sheet_configuration.GetTileHeight() * stage_config.MapHeight
			) );
			mCaptureTargetNode->setPosition( Vec2(
				( visibleSize.width * 0.25f ) - ( mCaptureTargetNode->getContentSize().width * 0.5f )
				, visibleCenter.y - ( mCaptureTargetNode->getContentSize().height * 0.5f )
			) );
			addChild( mCaptureTargetNode );

			//
			// Pivot
			//
			{
				mCaptureTargetNode->addChild( cpg_node::PivotNode::create(), std::numeric_limits<int>::max() );
			}

			//
			// Actor
			//
			{
				auto sprite = Sprite::create( "textures/step_flipflip/step_flipflip_dummy_01.png" );
				sprite->getTexture()->setAntiAliasTexParameters();
				sprite->setPosition( tile_sheet_configuration.GetTileWidth() * 3u, tile_sheet_configuration.GetTileHeight() * 3u );
				mCaptureTargetNode->addChild( sprite, 1 );

				mActorNode = sprite;
			}

			//
			// Camera
			//
			{
				auto camera_node = CameraNode::create();
				camera_node->SetCenterWorldPosition( mCaptureTargetNode->getPosition() + Vec2( mCaptureTargetNode->getContentSize().width * 0.5f, mCaptureTargetNode->getContentSize().height * 0.5f ) );
				camera_node->SetMainLayer( mCaptureTargetNode );
				camera_node->SetPivotNode( mActorNode );
				mCaptureTargetNode->addChild( camera_node );
			}

			//
			// Tile Map
			//
			{
				auto tile_map_node = step_defender::game::TileMapNode::create(
					stage_config
					, tile_sheet_configuration
				);
				tile_map_node->FillAll( 2, 0 );
				mCaptureTargetNode->addChild( tile_map_node );

				tile_map_node->UpdateTile( 0, 0, 4, 0 );
				tile_map_node->UpdateTile( stage_config.MapWidth - 1, stage_config.MapHeight - 1, 1, 0 );
			}

			//
			// Util 4 Capture
			//
			{
				mRenderTextureNode = RenderTexture::create( mCaptureTargetNode->getContentSize().width, mCaptureTargetNode->getContentSize().height );
				mRenderTextureNode->setVisible( false );
				mRenderTextureNode->setAutoDraw( false );
				mRenderTextureNode->setClearFlags( GL_COLOR_BUFFER_BIT );
				mRenderTextureNode->setClearColor( Color4F::ORANGE );
				mRenderTextureNode->getSprite()->getTexture()->setAliasTexParameters();
				addChild( mRenderTextureNode );

			}

			//
			// Show Captured Image
			//
			{
				auto sprite = Sprite::createWithTexture( mRenderTextureNode->getSprite()->getTexture() );
				sprite->setPosition( Vec2( visibleSize.width * 0.75f, visibleCenter.y ) );
				sprite->setFlippedY( true );
				addChild( sprite );
			}

			//
			// Capture Area
			//
			{
				auto draw_node = DrawNode::create();
				draw_node->drawRect( Vec2::ZERO, Vec2( mCaptureTargetNode->getContentSize().width, mCaptureTargetNode->getContentSize().height ), Color4F::GREEN );
				draw_node->setPosition( mCaptureTargetNode->getPosition() );
				addChild( draw_node, 100 );

				mCapturePivot = draw_node->getPosition();
			}
		}

		return true;
	}

	void Prerender2Scene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( Prerender2Scene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void Prerender2Scene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void Prerender2Scene::requestCapture()
	{
		scheduleOnce( schedule_selector( Prerender2Scene::test_UpdateEnd ), 0.f );
		mbInputBlock = true;
	}
	void Prerender2Scene::test_UpdateEnd( float )
	{
		//
		//
		//
		{
			const auto last_position = mCaptureTargetNode->getPosition();
			auto temp_position = mCaptureTargetNode->getParent()->convertToWorldSpace( mCaptureTargetNode->getPosition() );
			temp_position -= mCapturePivot;

			//
			// Move 2 Capture Position
			//
			mCaptureTargetNode->setPosition( temp_position );

			//
			// Capture
			//
			mRenderTextureNode->beginWithClear( mRenderTextureNode->getClearColor().r, mRenderTextureNode->getClearColor().g, mRenderTextureNode->getClearColor().b, mRenderTextureNode->getClearColor().a );
			mCaptureTargetNode->visit(
				_director->getRenderer()
				, _director->getMatrix( cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW )
				, true
			);
			mRenderTextureNode->end();

			//
			// Forced Render
			//
			_director->getRenderer()->render(); // Important~!~!~!


			//
			// Rollback
			//
			mCaptureTargetNode->setPosition( last_position );
			mCaptureTargetNode->setScaleX( -mCaptureTargetNode->getScaleX() ); // - for "dirty"
			mCaptureTargetNode->setScaleX( -mCaptureTargetNode->getScaleX() ); // - for "dirty"
		}


		unscheduleAllCallbacks();
		mbInputBlock = false;
	}

	void Prerender2Scene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( mbInputBlock )
		{
			return;
		}

		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			mActorNode->setPositionX( mActorNode->getPositionX() + 5.f );
			requestCapture();
			return;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			mActorNode->setPositionX( mActorNode->getPositionX() - 5.f );
			requestCapture();
			return;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			mActorNode->setPositionY( mActorNode->getPositionY() + 5.f );
			requestCapture();
			return;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			mActorNode->setPositionY( mActorNode->getPositionY() - 5.f );
			requestCapture();
			return;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
}
