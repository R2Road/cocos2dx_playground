#include "step_typetype_game_test_GameProcessScene.h"

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

#include "step_typetype_game_IndicatorViewNode.h"
#include "step_typetype_game_StageViewNode.h"
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
		GameProcessScene::GameProcessScene() :
			mKeyboardListener( nullptr )
			, mCurrentStageLength( 4 )
			, mStage( STAGE_MAX_LENGTH )
			, mStageViewNode( nullptr )
			, mIndicatorViewNode( nullptr )
		{}

		Scene* GameProcessScene::create()
		{
			auto ret = new ( std::nothrow ) GameProcessScene();
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

		bool GameProcessScene::init()
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
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );

				updateStageLengthView();
			}

			//
			// Stage View
			//
			{
				mStageViewNode = game::StageViewNode::create( STAGE_MAX_LENGTH, game::StageViewNode::Config{ true } );
				mStageViewNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mStageViewNode );
			}

			//
			// Indicator View
			//
			{
				mIndicatorViewNode = game::IndicatorViewNode::create( game::IndicatorViewNode::Config{ false, false } );
				mIndicatorViewNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mIndicatorViewNode, 1 );
			}

			mStage.Reset( mCurrentStageLength );
			mStageViewNode->Reset( mStage );
			mIndicatorViewNode->Reset( mCurrentStageLength );

			return true;
		}

		void GameProcessScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GameProcessScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void GameProcessScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void GameProcessScene::updateStageLengthView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_StageLengthView ) );
			label->setString( StringUtils::format( "Stage Length : %d/%d", mCurrentStageLength, STAGE_MAX_LENGTH ) );
		}

		void GameProcessScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
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
					mStageViewNode->Reset( mStage );
					mIndicatorViewNode->Reset( mCurrentStageLength );
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
					mStageViewNode->Reset( mStage );
					mIndicatorViewNode->Reset( mCurrentStageLength );
				}
				break;
				
			case EventKeyboard::KeyCode::KEY_R: // stage reset
				{
					mStage.Reset( mCurrentStageLength );
					mStageViewNode->Reset( mStage );
					mIndicatorViewNode->Reset( mCurrentStageLength );
				}
				break;

			case EventKeyboard::KeyCode::KEY_A: // auto play once : success
				if( !mStage.IsStageClear() )
				{
					const auto target_letter_pos = mStage.GetIndicator_Current();
					const auto target_letter = mStage.GetLetter( target_letter_pos );
					mStage.RequestLetterDie( target_letter );

					mStageViewNode->RequestLetterDie( target_letter_pos );
					mIndicatorViewNode->SetIndicatorPosition( target_letter_pos + 1u );

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
