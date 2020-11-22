#include "step_pathfinder_game_test_PlayerMoveScene.h"

#include <new>
#include <numeric>
#include <random>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
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
		PlayerMoveScene::PlayerMoveScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) : helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mTerrainData()
			, mTerrainViewer( nullptr )
			, mPlayer( nullptr )
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
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), FontPath, 9, Size::ZERO, TextHAlignment::LEFT );
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
				mTerrainViewer = game::TerrainViewer::create( mTerrainData.GetWidth(), mTerrainData.GetHeight(), Size( 32, 32 ) );
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
				mPlayer = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				mPlayer->setAnchorPoint( Vec2( 0.5f, 0.2f ) );
				mPlayer->setScale( _director->getContentScaleFactor() );
				mTerrainViewer->addChild( mPlayer, 1 );
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

					auto animate_action = Animate::create( animation_object );

					auto repeat_action = RepeatForever::create( animate_action );

					mPlayer->runAction( repeat_action );
				}
			}

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


		void PlayerMoveScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
