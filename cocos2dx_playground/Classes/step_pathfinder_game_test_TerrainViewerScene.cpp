#include "step_pathfinder_game_test_TerrainViewerScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_pathfinder_game_TerrainViewer.h"

USING_NS_CC;

namespace
{
	const char* FontPath = "fonts/NanumSquareR.ttf";
}

namespace step_pathfinder
{
	namespace game_test
	{
		TerrainViewerScene::TerrainViewerScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) : helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mTerrainData()
			, mTerrainViewer4Original( nullptr )
			, mTerrainViewer4Game( nullptr )
		{}

		Scene* TerrainViewerScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) TerrainViewerScene( back_to_the_previous_scene_callback );
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

		bool TerrainViewerScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();

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
				ss << "[R] : Reset";

				auto label = Label::createWithTTF( ss.str(), FontPath, 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 58, 0, 61, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Terrain View 4 Original
			//
			{
				// Explain Label
				auto label = Label::createWithTTF( "Original Data", FontPath, 11 );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.25f, visibleSize.height * 0.8f )
				);
				addChild( label, std::numeric_limits<int>::max() );


				// Terrain
				mTerrainViewer4Original = game::TerrainViewer::create( mTerrainData.GetWidth(), mTerrainData.GetHeight(), Size( 32, 32 ) );
				mTerrainViewer4Original->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.25f, visibleSize.height * 0.45f )
					- Vec2( mTerrainViewer4Original->getContentSize().width * 0.5f, mTerrainViewer4Original->getContentSize().height * 0.5f )
				);
				addChild( mTerrainViewer4Original );
			}

			//
			// Terrain View 4 Game
			//
			{
				// Explain Label
				auto label = Label::createWithTTF( "Fixed Data 4 Game", FontPath, 11 );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.75f, visibleSize.height * 0.8f )
				);
				addChild( label, std::numeric_limits<int>::max() );


				// Terrain
				mTerrainViewer4Game = game::TerrainViewer::create( mTerrainData.GetWidth(), mTerrainData.GetHeight(), Size( 32, 32 ) );
				mTerrainViewer4Game->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.75f, visibleSize.height * 0.45f )
					- Vec2( mTerrainViewer4Game->getContentSize().width * 0.5f, mTerrainViewer4Game->getContentSize().height * 0.5f )
				);
				addChild( mTerrainViewer4Game );
			}

			return true;
		}

		void TerrainViewerScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TerrainViewerScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void TerrainViewerScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void TerrainViewerScene::resetTerrain()
		{
			mTerrainData.FillRandomTileType();

			mTerrainViewer4Original->LoadTerrainData4Original( mTerrainData );
			mTerrainViewer4Game->LoadTerrainData4Game( mTerrainData );
		}


		void TerrainViewerScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			if( EventKeyboard::KeyCode::KEY_R == keycode )
			{
				resetTerrain();
			}
		}
	}
}
