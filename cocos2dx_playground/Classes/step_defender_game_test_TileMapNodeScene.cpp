#include "step_defender_game_test_TileMapNodeScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_defender_game_TileMapNode.h"

USING_NS_CC;

namespace step_defender
{
	namespace game_test
	{
		TileMapNodeScene::TileMapNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mTileMapNode( nullptr )

			, mCurrentTilePointX( 0 )
			, mCurrentTilePointY( 0 )
		{}

		Scene* TileMapNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) TileMapNodeScene( back_to_the_previous_scene_callback );
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

		bool TileMapNodeScene::init()
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
				ss << "[1] : Fill Ones";
				ss << cpg::linefeed;
				ss << "[2] : Fill All";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 7, 39, 43, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			//
			//
			{
				mTileMapNode = step_defender::game::TileMapNode::create(
					step_defender::game::TileMapNode::Config{ 10, 6 }
					, game::TileSheetConfig
				);
				mTileMapNode->setPosition(
					visibleCenter
					- Vec2( mTileMapNode->getContentSize().width * 0.5f, mTileMapNode->getContentSize().height * 0.5f )
				);
				addChild( mTileMapNode );

				mTileMapNode->FillAll( mCurrentTilePointX, mCurrentTilePointY );
			}

			return true;
		}

		void TileMapNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TileMapNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void TileMapNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void TileMapNodeScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			if( EventKeyboard::KeyCode::KEY_1 == key_code )
			{
				++mCurrentTilePointX;
				if( 0 < mCurrentTilePointX )
				{
					mCurrentTilePointX = 0;

					mCurrentTilePointY = mCurrentTilePointY + 1 > 2 ? 0 : mCurrentTilePointY + 1;
				}

				mTileMapNode->UpdateTile( 0, 0, mCurrentTilePointX, mCurrentTilePointY );
				return;
			}

			if( EventKeyboard::KeyCode::KEY_2 == key_code )
			{
				++mCurrentTilePointX;
				if( 0 < mCurrentTilePointX )
				{
					mCurrentTilePointX = 0;

					mCurrentTilePointY = mCurrentTilePointY + 1 > 2 ? 0 : mCurrentTilePointY + 1;
				}

				mTileMapNode->FillAll( mCurrentTilePointX, mCurrentTilePointY );
			}
		}
	}
}
