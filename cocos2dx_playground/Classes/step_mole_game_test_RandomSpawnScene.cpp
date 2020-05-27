#include "step_mole_game_test_RandomSpawnScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_mole_game_StageNode.h"
#include "step_mole_game_TargetManager.h"

#include "step_mole_RootScene.h"

USING_NS_CC;

namespace
{
	const step_mole::game::StageConfig STAGE_CONFIG{ 8, 6, Size( 40.f, 40.f ) };
}

namespace step_mole
{
	namespace game_test
	{
		RandomSpawnScene::RandomSpawnScene() : mKeyboardListener( nullptr ), mTargetManager(), mStageNode( nullptr )
		{}

		Scene* RandomSpawnScene::create()
		{
			auto ret = new ( std::nothrow ) RandomSpawnScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool RandomSpawnScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

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
				ss << "[A] : Random Spawn";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
				addChild( background_layer, -1 );
			}

			//
			// Target Manager
			//
			{
				mTargetManager = game::TargetManager::create( STAGE_CONFIG );
			}

			//
			// Stage Node
			//
			{
				mStageNode = step_mole::game::StageNode::create(
					STAGE_CONFIG
					, std::bind( &game::TargetManager::ComeHomeTarget, mTargetManager.get(), std::placeholders::_1 )
					, game::StageNodeConfig{ true, true }
					, CircleCollisionComponentConfig{ true, true, true }
				);
				mStageNode->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - mStageNode->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - mStageNode->getContentSize().height ) * 0.5f )
				) );
				addChild( mStageNode );
			}

			return true;
		}

		void RandomSpawnScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( RandomSpawnScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void RandomSpawnScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}


		void RandomSpawnScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_mole::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_A:
			{
				const auto target_index = mTargetManager->GetIdleTarget();
				if( -1 != target_index )
				{
					mStageNode->RequestAction( target_index, 3.f );
				}

				return;
			}

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
