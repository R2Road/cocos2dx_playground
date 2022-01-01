#include "cocos_research_render_PrerenderScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCRenderTexture.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "cpg_TileSheetConfiguration.h"

#include "step_defender_game_TileMapNode.h"

USING_NS_CC;

namespace cocos_research_render
{
	PrerenderScene::PrerenderScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mTileMapNode( nullptr )
		, mRenderTextureNode( nullptr )

		, mbInputBlock( false )
	{}

	Scene* PrerenderScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) PrerenderScene( back_to_the_previous_scene_callback );
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

	bool PrerenderScene::init()
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
			ss << "[SPACE] : Do";

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
			auto background_layer = LayerColor::create( Color4B( 5, 29, 81, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// Research
		//
		{
			//
			// Load Tile Config
			//
			cpg::TileSheetConfiguration tile_sheet_configuration;
			CCASSERT( tile_sheet_configuration.Load( "datas/algorithm_practice/algorithm_practice_tile_sheet_config_01.json" ), "Failed - Load Tile Sheet Configuration" );

			//
			// Tile Maps
			//
			{
				mTileMapNode = step_defender::game::TileMapNode::create(
					step_defender::game::TileMapNode::Config{ 6u, 6u }
					, tile_sheet_configuration
				);
				mTileMapNode->setPosition( Vec2(
					0.f
					, visibleCenter.y - mTileMapNode->getContentSize().height * 0.5f
				) );
				addChild( mTileMapNode );
			}

			//
			//
			//
			{
				mRenderTextureNode = RenderTexture::create( visibleSize.width * 0.5f, visibleSize.height );
				mRenderTextureNode->setContentSize( Size( visibleSize.width * 0.5f, visibleSize.height ) );
				mRenderTextureNode->setVisible( false );
				mRenderTextureNode->setAutoDraw( false );
				mRenderTextureNode->setClearFlags( GL_COLOR_BUFFER_BIT );
				mRenderTextureNode->setClearColor( Color4F( 1.0f, 0.0f, 0.0f, 0.5f ) );
				mRenderTextureNode->getSprite()->getTexture()->setAliasTexParameters();
				addChild( mRenderTextureNode );

				auto sprite = Sprite::createWithTexture( mRenderTextureNode->getSprite()->getTexture() );
				sprite->setAnchorPoint( Vec2::ZERO );
				sprite->setScaleY( -1 );
				sprite->setPosition( visibleCenter.x, visibleOrigin.y + visibleSize.height );
				addChild( sprite );
			}
		}

		//
		//
		//
		mTileMapNode->FillAll( 4, 0 );


		return true;
	}

	void PrerenderScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PrerenderScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void PrerenderScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void PrerenderScene::test_UpdateEnd( float )
	{
		mTileMapNode->setPositionX( mTileMapNode->getPositionX() + 30.f );


		//
		//
		//
		{
			mRenderTextureNode->beginWithClear( mRenderTextureNode->getClearColor().r, mRenderTextureNode->getClearColor().g, mRenderTextureNode->getClearColor().b, mRenderTextureNode->getClearColor().a );
			mTileMapNode->visit(
				_director->getRenderer()
				, _director->getMatrix( cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW )
				, true
			);
			mRenderTextureNode->end();
		}


		unscheduleAllCallbacks();
		mbInputBlock = false;
	}

	void PrerenderScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_SPACE:
			scheduleOnce( schedule_selector( PrerenderScene::test_UpdateEnd ), 0.f );
			mbInputBlock = true;
			return;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
}
