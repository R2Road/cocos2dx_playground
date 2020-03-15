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
				ss << "[ESC] : Return to Root";

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
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( PlayScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( PlayScene::updateForExit ), 0.f );
					return;
				}
			}

			if( EventKeyboard::KeyCode::KEY_A <= keycode && EventKeyboard::KeyCode::KEY_Z >= keycode )
			{
				static const char offset = static_cast<char>( EventKeyboard::KeyCode::KEY_A ) - 65; // 65 == 'A'

				const auto target_letter_code = static_cast<char>( keycode ) - offset;
				const auto target_letter_pos = mStage.GetIndicator_Current();
				if( mStage.RequestRemoveLetter( target_letter_code ) )
				{
					mStageView->RequestLetterDie( target_letter_pos );
					experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.2f );
				}
				else
				{
					experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.2f );
				}
			}
			else
			{
				experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.2f );
			}
		}
	}
}
