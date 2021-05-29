#include "step_pathfinder_game_test_PlayerMoveScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_pathfinder_game_PlayerNode.h"
#include "step_pathfinder_game_TerrainViewer.h"

USING_NS_CC;

namespace
{
	const int TerrainWidth = 5;
	const int TerrainHeight = 5;
}

namespace step_pathfinder
{
	namespace game_test
	{
		PlayerMoveScene::PlayerMoveScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) : helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mTerrainViewer( nullptr )
			, mPlayer( nullptr )
			, mPlayerPoint()
		{}

		Scene* PlayerMoveScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) PlayerMoveScene( back_to_the_previous_scene_callback );
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

		bool PlayerMoveScene::init()
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
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Arrow] : Move Player";
				ss << cpg::linefeed;
				ss << "[R] : Reset Player Position";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 58, 0, 61, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Terrain View
			//
			{
				mTerrainViewer = game::TerrainViewer::create( TerrainWidth, TerrainHeight, Size( 48, 48 ) );
				mTerrainViewer->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
					- Vec2( mTerrainViewer->getContentSize().width * 0.5f, mTerrainViewer->getContentSize().height * 0.5f )
				);
				addChild( mTerrainViewer );
			}

			//
			// Player
			//
			{
				mPlayer = game::PlayerNode::create();
				mTerrainViewer->addChild( mPlayer, 1 );
			}

			//
			// Setup
			//
			mPlayer->setPosition( mTerrainViewer->ConvertPoint2Position( mPlayerPoint.x, mPlayerPoint.y ) );

			return true;
		}

		void PlayerMoveScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayerMoveScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void PlayerMoveScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void PlayerMoveScene::playerMove( const int move_x, const int move_y )
		{
			mPlayerPoint.x += move_x;
			mPlayerPoint.y += move_y;

			mPlayer->setPosition( mTerrainViewer->ConvertPoint2Position( mPlayerPoint.x, mPlayerPoint.y ) );
		}
		void PlayerMoveScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			if( EventKeyboard::KeyCode::KEY_R == keycode )
			{
				mPlayerPoint = { 0, 0 };
				mPlayer->setPosition( mTerrainViewer->ConvertPoint2Position( mPlayerPoint.x, mPlayerPoint.y ) );
				return;
			}

			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				playerMove( -1, 0 );
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				playerMove( 1, 0 );
				break;
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				playerMove( 0, 1 );
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				playerMove( 0, -1 );
				break;
			}
		}
	}
}
