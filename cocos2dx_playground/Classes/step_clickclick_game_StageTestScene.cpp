#include "step_clickclick_game_StageTestScene.h"

#include <cassert>
#include <functional>
#include <new>
#include <numeric>
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
			const int TAG_SelectedBlockTypeView = 20160528;
			const int TAG_ActiveBlockCountView = 9999;
		}

		StageTestScene::StageTestScene() :
			mKeyboardListener( nullptr )
			, mStage()
			, mStageView( nullptr )
			, mGridIndexConverter( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT )

			, mTestActionType( eTestActionType::Increase )
		{}

		Scene* StageTestScene::create()
		{
			auto ret = new ( std::nothrow ) StageTestScene();
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

		bool StageTestScene::init()
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
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 0, 41, 13, 255 ) );
				addChild( background_layer, -1 );
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
					, std::bind( &StageTestScene::onGameProcess, this, std::placeholders::_1 )
					, StageViewConfig{ true, true }
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
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 10 );
				label->setTag( TAG_TestActionView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateTestAction( mTestActionType );
			}

			//
			// Selected Block Type View
			//
			{
				auto label = Label::createWithTTF( "Block Type : -", "fonts/arial.ttf", 10 );
				label->setTag( TAG_SelectedBlockTypeView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Active Block Count View
			//
			{
				auto label = Label::createWithTTF( "Block", "fonts/arial.ttf", 10 );
				label->setTag( TAG_ActiveBlockCountView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.1f
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateActiveBlockCountView( mStage->GetActiveBlockCount() );
			}

			return true;
		}

		void StageTestScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( StageTestScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void StageTestScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void StageTestScene::onGameProcess( const int block_linear_index )
		{
			experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );

			const auto& target_block_data = mStage->GetBlockData( block_linear_index );
			updateSelectedBlockTypeView( target_block_data.GetType() );

			const int last_life = target_block_data.GetLife();
			switch( mTestActionType )
			{
			case eTestActionType::Increase:
				mStage->IncreaseBlockLife( block_linear_index );
				break;
			case eTestActionType::Decrease:
				mStage->DecreaseBlockLife( block_linear_index );
				break;
			case eTestActionType::Die:
				mStage->DieBlock( block_linear_index );
				break;
			default:
				assert( false );
			}

			mStageView->UpdateBlock( block_linear_index, last_life, target_block_data.GetLife() );
			updateActiveBlockCountView( mStage->GetActiveBlockCount() );
		}


		void StageTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
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

		void StageTestScene::updateTestAction( const eTestActionType test_action_type )
		{
			mTestActionType = test_action_type;

			auto label = static_cast<Label*>( getChildByTag( TAG_TestActionView ) );
			switch( mTestActionType )
			{
			case eTestActionType::Increase:
				label->setString( "Current Test Action : Increase" );
				break;
			case eTestActionType::Decrease:
				label->setString( "Current Test Action : Decrease" );
				break;
			case eTestActionType::Die:
				label->setString( "Current Test Action : Die" );
				break;
			default:
				assert( false );
			}
		}
		void StageTestScene::updateSelectedBlockTypeView( const eBlockType block_type )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_SelectedBlockTypeView ) );
			switch( block_type )
			{
			case eBlockType::Single:
				label->setString( "Block Type : Single" );
				break;
			case eBlockType::Same:
				label->setString( "Block Type : Same" );
				break;
			case eBlockType::Different:
				label->setString( "Block Type : Different" );
				break;
			default:
				assert( false );
			}
		}
		void StageTestScene::updateActiveBlockCountView( const int count )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ActiveBlockCountView ) );
			label->setString( StringUtils::format( "Remain Block Count : %d", count ) );
		}
	} // namespace game
} // namespace step_clickclick
