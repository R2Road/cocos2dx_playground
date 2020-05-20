#include "step_mole_game_PlayScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "audio/include/AudioEngine.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"

#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_mole_game_StageView.h"
#include "step_mole_game_TargetManager.h"

#include "step_mole_game_TitleScene.h"

USING_NS_CC;

namespace
{
	const step_mole::game::StageConfig STAGE_CONFIG{ 8, 6, Size( 40.f, 40.f ) };

	const std::size_t GetRandomObjectIndex()
	{
		static std::random_device rd;
		static std::mt19937 randomEngine( rd() );
		static std::uniform_int_distribution<> dist( 0, std::max( 0, ( STAGE_CONFIG.BlockCount_Vercital * STAGE_CONFIG.BlockCount_Horizontal ) - 1 ) );

		return dist( randomEngine );
	}
}

namespace step_mole
{
	namespace game
	{
		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mAudioID_forBGM( -1 )
			
			, mTargetManager()
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
				ss << "[A] : Random Start";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
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
				mTargetManager = TargetManager::create( STAGE_CONFIG );
			}

			//
			// Stage View
			//
			{
				mStageView = step_mole::game::StageView::create(
					STAGE_CONFIG
					, std::bind( &TargetManager::ComeHomeTarget, mTargetManager.get(), std::placeholders::_1 )
					, StageViewConfig{ true, true }
					, CircleCollisionComponentConfig{ true, true, true }
				);
				mStageView->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - mStageView->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - mStageView->getContentSize().height ) * 0.5f )
				) );
				addChild( mStageView );
			}

			return true;
		}

		void PlayScene::onEnter()
		{
			Scene::onEnter();

			mAudioID_forBGM = experimental::AudioEngine::play2d( "sounds/bgm/EmpySpace.ogg", true, 0.1f );

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void PlayScene::onExit()
		{
			experimental::AudioEngine::stop( mAudioID_forBGM );
			mAudioID_forBGM = -1;

			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}


		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_mole::game::TitleScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_A:
			{
				const auto target_index = mTargetManager->GetIdleTarget();
				if( -1 != target_index )
				{
					mStageView->RequestAction( target_index, 3.f );
				}

				return;
			}

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
