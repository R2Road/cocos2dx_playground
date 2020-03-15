#include "step_typetype_game_PlayScene.h"

#include <new>
#include <sstream>

#include "audio/include/AudioEngine.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "step_typetype_game_StageView.h"
#include "step_typetype_game_TitleScene.h"

USING_NS_CC;

namespace
{
	const int STAGE_MAX_LENGTH = 20;
}

namespace step_typetype
{
	namespace game
	{
		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mCurrentStageLength( 4 )
			, mStage( STAGE_MAX_LENGTH )
			, mStageView( nullptr )
		{}

		Scene* PlayScene::create()
		{
			auto ret = new ( std::nothrow ) PlayScene();
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

		bool PlayScene::init()
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
				ss << "[1] : Increase Stage Size And Reset";
				ss << std::endl;
				ss << "[2] : Decrease Stage Size And Reset";
				ss << std::endl;
				ss << "[R] : Stage Reset";
				ss << std::endl;
				ss << std::endl;
				ss << "[P] : Auto Play Once : Success";
				ss << std::endl;
				ss << "[O] : Auto Play Once : Failed";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 79, 10, 5, 255 ) );
				addChild( background_layer, -1 );
			}

			//
			// Stage View
			//
			{
				mStageView = StageView::create( STAGE_MAX_LENGTH, StageViewConfig{ true, true } );
				mStageView->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( mStageView );
			}

			mStage.Reset( mCurrentStageLength );
			mStageView->Reset( mStage );

			return true;
		}

		void PlayScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void PlayScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void PlayScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_typetype::game::TitleScene::create() );
		}
		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				if( !isScheduled( schedule_selector( PlayScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( PlayScene::updateForExit ), 0.f );
				}
				break;

			case EventKeyboard::KeyCode::KEY_1: // increase stage size + reset
				{
					++mCurrentStageLength;

					mStage.Reset( mCurrentStageLength );
					mStageView->Reset( mStage );
				}
				break;
			case EventKeyboard::KeyCode::KEY_2: // decrease stage size + reset
				{
					mCurrentStageLength = (
						mCurrentStageLength > 0
						? mCurrentStageLength - 1
						: 0
					);

					mStage.Reset( mCurrentStageLength );
					mStageView->Reset( mStage );
				}
				break;
				
			case EventKeyboard::KeyCode::KEY_R: // stage reset
				{
					mStage.Reset( mCurrentStageLength );
					mStageView->Reset( mStage );
				}
				break;

			case EventKeyboard::KeyCode::KEY_O: // auto play once : failed
				if( !mStage.IsGameClear() )
				{
					const auto target_letter_pos = mStage.GetIndicator_Current();
					const auto target_letter = mStage.GetLetter( target_letter_pos ) + 1;
					mStage.RequestRemoveLetter( target_letter );

					experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.2f );
					return;
				}
				break;
			case EventKeyboard::KeyCode::KEY_P: // auto play once : success
				if( !mStage.IsGameClear() )
				{
					const auto target_letter_pos = mStage.GetIndicator_Current();
					const auto target_letter = mStage.GetLetter( target_letter_pos );
					mStage.RequestRemoveLetter( target_letter );

					mStageView->RequestLetterDie( target_letter_pos );

					experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.2f );
					return;
				}
				break;
			}
		}
	}
}
