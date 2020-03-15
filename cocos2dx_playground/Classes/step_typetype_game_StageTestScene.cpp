#include "step_typetype_game_StageTestScene.h"

#include <new>
#include <sstream>

#include "audio/include/AudioEngine.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_StageView = 20140416;
}

namespace step_typetype
{
	namespace game
	{
		StageTestScene::StageTestScene() :
			mCurrentStageLength( 1 )
			, mStage( 10 )
			, mKeyboardListener( nullptr )
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
				addChild( background_layer, 0 );
			}

			//
			// Stage View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 9 );
				label->setTag( TAG_StageView );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( label, 9999 );
			}

			updateStage();

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

		void StageTestScene::updateStage()
		{
			mStage.Reset( mCurrentStageLength );

			std::stringstream ss;
			for( std::size_t i = 0; i < mStage.GetIndicator_End(); ++i )
			{
				ss << mStage.GetLetter( i );
			}

			auto label = static_cast<Label*>( getChildByTag( TAG_StageView ) );
			label->setString( ss.str() );
		}

		void StageTestScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_typetype::RootScene::create() );
		}
		void StageTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( isScheduled( schedule_selector( StageTestScene::updateForExit ) ) )
				{
					return;
				}
				else
				{
					scheduleOnce( schedule_selector( StageTestScene::updateForExit ), 0.f );
				}
			}

			if( EventKeyboard::KeyCode::KEY_1 == keycode )
			{
				++mCurrentStageLength;
			}
			if( EventKeyboard::KeyCode::KEY_2 == keycode )
			{
				mCurrentStageLength = (
					mCurrentStageLength > 0
					? mCurrentStageLength - 1
					: 0
				);
			}

			if( EventKeyboard::KeyCode::KEY_R == keycode )
			{
				updateStage();
			}
		}
	}
}
