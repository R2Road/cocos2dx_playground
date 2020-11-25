#include "step_typetype_game_test_StageScene.h"

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
	namespace game_test
	{
		StageScene::StageScene() :
			mKeyboardListener( nullptr )
			, mCurrentStageLength( 4 )
			, mStage( STAGE_MAX_LENGTH )
			, mStageView( nullptr )
		{}

		Scene* StageScene::create()
		{
			auto ret = new ( std::nothrow ) StageScene();
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

		bool StageScene::init()
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
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow U] : Increase Stage Size And Reset";
				ss << std::endl;
				ss << "[Arrow D] : Decrease Stage Size And Reset";
				ss << std::endl;
				ss << std::endl;
				ss << "[A] : Auto Play Once : Success";
				ss << std::endl;
				ss << "[S] : Auto Play Once : Failed";
				ss << std::endl;
				ss << std::endl;
				ss << "[R] : Stage Reset";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
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
			// Stage Length View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 10 );
				label->setTag( TAG_StageLengthView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateStageLengthView();
			}

			//
			// Stage View
			//
			{
				mStageView = game::StageView::create( STAGE_MAX_LENGTH, game::StageViewConfig{ true, true } );
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

		void StageScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( StageScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void StageScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void StageScene::updateStageLengthView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_StageLengthView ) );
			label->setString( StringUtils::format( "Stage Length : %d", mCurrentStageLength ) );
		}

		void StageScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_typetype::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_UP_ARROW: // increase stage size + reset
				{
					++mCurrentStageLength;
					updateStageLengthView();

					mStage.Reset( mCurrentStageLength );
					mStageView->Reset( mStage );
				}
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW: // decrease stage size + reset
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

			case EventKeyboard::KeyCode::KEY_A: // auto play once : success
				if( !mStage.IsStageClear() )
				{
					const auto target_letter_pos = mStage.GetIndicator_Current();
					const auto target_letter = mStage.GetLetter( target_letter_pos );
					mStage.RequestLetterDie( target_letter );

					mStageView->RequestLetterDie( target_letter_pos );

					experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );
					return;
				}
				break;
			case EventKeyboard::KeyCode::KEY_S: // auto play once : failed
				if( !mStage.IsStageClear() )
				{
					const auto target_letter_pos = mStage.GetIndicator_Current();
					const auto target_letter = mStage.GetLetter( target_letter_pos ) + 1;
					mStage.RequestLetterDie( target_letter );

					experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.1f );
					return;
				}
				break;
			}
		}
	}
}
