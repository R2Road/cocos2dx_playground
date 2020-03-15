#include "step_typetype_game_StageTestScene.h"

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
#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_StageLengthView = 20140416;

	const int STAGE_MAX_LENGTH = 20;
}

namespace step_typetype
{
	namespace game
	{
		StageTestScene::StageTestScene() :
			mKeyboardListener( nullptr )
			, mCurrentStageLength( 4 )
			, mStage( STAGE_MAX_LENGTH )
			, mStageView( nullptr )
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
			// Stage Length View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 10 );
				label->setTag( TAG_StageLengthView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );

				updateStageLengthView();
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

			Node::onExit();
		}

		void StageTestScene::updateStageLengthView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_StageLengthView ) );
			label->setString( StringUtils::format( "Stage Length : %d", mCurrentStageLength ) );
		}

		void StageTestScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_typetype::RootScene::create() );
		}
		void StageTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				if( !isScheduled( schedule_selector( StageTestScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( StageTestScene::updateForExit ), 0.f );
				}
				break;

			case EventKeyboard::KeyCode::KEY_1: // increase stage size + reset
				{
					++mCurrentStageLength;
					updateStageLengthView();

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
					updateStageLengthView();

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
				if( !mStage.IsStageClear() )
				{
					const auto target_letter_pos = mStage.GetIndicator_Current();
					const auto target_letter = mStage.GetLetter( target_letter_pos ) + 1;
					mStage.RequestLetterDie( target_letter );

					experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.2f );
					return;
				}
				break;
			case EventKeyboard::KeyCode::KEY_P: // auto play once : success
				if( !mStage.IsStageClear() )
				{
					const auto target_letter_pos = mStage.GetIndicator_Current();
					const auto target_letter = mStage.GetLetter( target_letter_pos );
					mStage.RequestLetterDie( target_letter );

					mStageView->RequestLetterDie( target_letter_pos );

					experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.2f );
					return;
				}
				break;
			}
		}
	}
}
