#include "step_typetype_game_PlayScene.h"

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

#include "step_typetype_game_ResultScene.h"
#include "step_typetype_game_StageView.h"
#include "step_typetype_game_TitleScene.h"

USING_NS_CC;

namespace
{
	const int TAG_NextStageIndicator = 20140416;
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
			, mElapsedTime( 0 )
			, mAudioID_forBGM( -1 )
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
				ret->scheduleUpdate();
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
				ss << "[ESC] : Return to Title";

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
				auto background_layer = LayerColor::create( Color4B( 79, 10, 5, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// BGM License
			//
			{
				auto label = Label::createWithTTF(
					"BGM : Empty Space\nAuthor : tcarisland\nLicense : CC-BY 4.0\nFrom : https://opengameart.org/"
					, "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::RIGHT
				);
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Stage View
			//
			{
				mStageView = StageView::create( STAGE_MAX_LENGTH, StageViewConfig{ false, true } );
				mStageView->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( mStageView );
			}

			//
			// Next Stage Indicator
			//
			{
				auto label = Label::createWithTTF( "ENTER", "fonts/arial.ttf", 9 );
				label->setTag( TAG_NextStageIndicator );
				label->setColor( Color3B::GREEN );
				label->setVisible( false );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( label, 1 );
			}

			mStage.Reset( mCurrentStageLength );
			mStageView->Reset( mStage );

			return true;
		}

		void PlayScene::onEnter()
		{
			Scene::onEnter();

			mAudioID_forBGM = experimental::AudioEngine::play2d( "sounds/bgm/EmpySpace.ogg", true, 0.1f );

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void PlayScene::update( float dt )
		{
			mElapsedTime += dt;
			Scene::update( dt );
		}
		void PlayScene::onExit()
		{
			experimental::AudioEngine::stop( mAudioID_forBGM );

			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( step_typetype::game::TitleScene::create() );
				return;
			}

			if( !mStage.IsStageClear() )
			{
				if( EventKeyboard::KeyCode::KEY_A <= keycode && EventKeyboard::KeyCode::KEY_Z >= keycode )
				{
					static const char offset = static_cast<char>( EventKeyboard::KeyCode::KEY_A ) - 65; // 65 == 'A'

					const auto target_letter_code = static_cast<char>( keycode ) - offset;
					const auto target_letter_pos = mStage.GetIndicator_Current();
					if( mStage.RequestLetterDie( target_letter_code ) )
					{
						mStageView->RequestLetterDie( target_letter_pos );
						experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );

						if( mStage.IsStageClear() )
						{
							getChildByTag( TAG_NextStageIndicator )->setVisible( true );
						}
					}
					else
					{
						experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.1f );
					}
				}
			}
			else if( EventKeyboard::KeyCode::KEY_ENTER == keycode )
			{
				mCurrentStageLength += 2;
				experimental::AudioEngine::play2d( "sounds/fx/powerup_001.ogg", false, 0.1f );

				if( mCurrentStageLength < mStage.GetLength_MAX() ) // go next stage
				{
					mStage.Reset( mCurrentStageLength );
					mStageView->Reset( mStage );

					getChildByTag( TAG_NextStageIndicator )->setVisible( false );
				}
				else // game clear
				{
					Director::getInstance()->replaceScene( step_typetype::game::ResultScene::create( mElapsedTime ) );
				}
			}
		}
	}
}
