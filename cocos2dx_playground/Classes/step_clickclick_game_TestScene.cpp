#include "step_clickclick_game_TestScene.h"

#include <cassert>
#include <functional>
#include <new>
#include <sstream>

#include "audio/include/AudioEngine.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "step_clickclick_RootScene.h"

#include "step_clickclick_game_Stage.h"
#include "step_clickclick_game_StageView.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		namespace
		{
			const int MAX_STAGE_WIDTH = 7;
			const int MAX_STAGE_HEIGHT = 7;

			const int TAG_TestActionView = 20140416;
			const int TAG_SelectedPannelTypeView = 20160528;
			const int TAG_ActiveBlockCountView = 9999;
		}

		TestScene::TestScene() :
			mKeyboardListener( nullptr )
			, mStage()
			, mStageView( nullptr )
			, mGridIndexConverter( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT )

			, mTestActionType( eTestActionType::Increase )
		{}

		Scene* TestScene::create()
		{
			auto ret = new ( std::nothrow ) TestScene();
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

		bool TestScene::init()
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
				ss << "[Mouse] : Click";
				ss << std::endl;
				ss << "[F1] : Reset";
				ss << std::endl;
				ss << std::endl;
				ss << "[1] : Test Action : Increase";
				ss << std::endl;
				ss << "[2] : Test Action : Decrease";
				ss << std::endl;
				ss << "[3] : Test Action : Die";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 0, 41, 13, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// Stage
			//
			{
				mStage = step_clickclick::game::Stage::create( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT );
				mStage->Setup( 5, 5 );
			}

			//
			// StageView
			//
			{
				mStageView = step_clickclick::game::StageView::create(
					MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT
					, std::bind( &TestScene::onGameProcess, this, std::placeholders::_1 )
				);
				mStageView->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( mStageView );

				mStageView->Setup( *mStage );
			}

			//
			// Test Action
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 14 );
				label->setTag( TAG_TestActionView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );

				updateTestAction( mTestActionType );
			}

			//
			// Selected Pannel Type View
			//
			{
				auto label = Label::createWithTTF( "Pannel Type : -", "fonts/arial.ttf", 14 );
				label->setTag( TAG_SelectedPannelTypeView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y
				) );
				addChild( label, 9999 );
			}

			//
			// Active Block Count View
			//
			{
				auto label = Label::createWithTTF( "Block", "fonts/arial.ttf", 14 );
				label->setTag( TAG_ActiveBlockCountView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.1f
				) );
				addChild( label, 9999 );

				updateActiveBlockCountView( mStage->GetActiveBlockCount() );
			}

			return true;
		}

		void TestScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TestScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void TestScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void TestScene::onGameProcess( const int pannel_linear_index )
		{
			experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg" );

			const auto& target_pannel_data = mStage->GetBlockData( pannel_linear_index );
			updateSelectedPannelTypeView( target_pannel_data.GetType() );

			const int last_life = target_pannel_data.GetLife();
			switch( mTestActionType )
			{
			case eTestActionType::Increase:
				mStage->IncreaseBlockLife( pannel_linear_index );
				break;
			case eTestActionType::Decrease:
				mStage->DecreaseBlockLife( pannel_linear_index );
				break;
			case eTestActionType::Die:
				mStage->DieBlock( pannel_linear_index );
				break;
			default:
				assert( false );
			}

			mStageView->UpdatePannel( pannel_linear_index, last_life, target_pannel_data.GetLife() );
			updateActiveBlockCountView( mStage->GetActiveBlockCount() );
		}


		void TestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_clickclick::RootScene::create() );
				break;

			case EventKeyboard::KeyCode::KEY_F1:
				mStage->Setup( 5, 5 );
				mStageView->Setup( *mStage );
				updateActiveBlockCountView( mStage->GetActiveBlockCount() );
				break;

			case EventKeyboard::KeyCode::KEY_1:
				updateTestAction( eTestActionType::Increase );
				break;
			case EventKeyboard::KeyCode::KEY_2:
				updateTestAction( eTestActionType::Decrease );
				break;
			case EventKeyboard::KeyCode::KEY_3:
				updateTestAction( eTestActionType::Die );
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void TestScene::updateTestAction( const eTestActionType test_action_type )
		{
			mTestActionType = test_action_type;

			auto label = static_cast<Label*>( getChildByTag( TAG_TestActionView ) );
			switch( mTestActionType )
			{
			case eTestActionType::Increase:
				label->setString( "Test Action : Increase" );
				break;
			case eTestActionType::Decrease:
				label->setString( "Test Action : Decrease" );
				break;
			case eTestActionType::Die:
				label->setString( "Test Action : Die" );
				break;
			default:
				assert( false );
			}
		}
		void TestScene::updateSelectedPannelTypeView( const eBlockType pannel_type )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_SelectedPannelTypeView ) );
			switch( pannel_type )
			{
			case eBlockType::Single:
				label->setString( "Pannel Type : Single" );
				break;
			case eBlockType::Same:
				label->setString( "Pannel Type : Same" );
				break;
			case eBlockType::Different:
				label->setString( "Pannel Type : Different" );
				break;
			default:
				assert( false );
			}
		}
		void TestScene::updateActiveBlockCountView( const int count )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ActiveBlockCountView ) );
			label->setString( StringUtils::format( "Remain Block Count : %d", count ) );
		}
	} // namespace game
} // namespace step_clickclick
