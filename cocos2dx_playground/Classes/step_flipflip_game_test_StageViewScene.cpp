#include "step_flipflip_game_test_StageViewScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_flipflip_game_Constant.h"
#include "step_flipflip_game_StageData.h"
#include "step_flipflip_game_StageViewNode.h"
#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace
{
	step_flipflip::game::StageConfig stage_config{ 6, 5, Size( 40.f, 54.f ) };
}

namespace step_flipflip
{
	namespace game_test
	{
		StageViewScene::StageViewScene() : mKeyboardListener( nullptr ), mStageViewNode( nullptr ) {}

		Scene* StageViewScene::create()
		{
			auto ret = new ( std::nothrow ) StageViewScene();
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

		bool StageViewScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();
			const Vec2 visibleCenter(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f ,visibleSize.height * 0.5f )
			);


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
				ss << "[SPACE] : Flip All";
				ss << std::endl;
				ss << std::endl;
				ss << "[R] : Reset";

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
				auto background_layer = LayerColor::create( Color4B( 29, 96, 96, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Setup
			//
			BuildStageView();

			return true;
		}

		void StageViewScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( StageViewScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void StageViewScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void StageViewScene::BuildStageView()
		{
			if( mStageViewNode )
			{
				removeChild( mStageViewNode );
				mStageViewNode = nullptr;
			}

			//
			// Stage Data
			//
			game::StageData stage_data;
			stage_data.Reset( stage_config.Width, stage_config.Height );

			//
			// Stage View Node
			//
			{
				mStageViewNode = game::StageViewNode::create( stage_config, stage_data, true );
				mStageViewNode->setPosition(
					_director->getVisibleOrigin()
					+ Vec2( _director->getVisibleSize().width * 0.5f, _director->getVisibleSize().height * 0.5f )
					- Vec2( mStageViewNode->getContentSize().width * 0.5f, mStageViewNode->getContentSize().height * 0.5f )
				);
				addChild( mStageViewNode );
			}
		}


		void StageViewScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_flipflip::RootScene::create() );
				return;
			}

			if( EventKeyboard::KeyCode::KEY_SPACE == keycode )
			{
				for( int current_h = 0; stage_config.Height > current_h; ++current_h )
				{
					for( int current_w = 0; stage_config.Width > current_w; ++current_w )
					{
						mStageViewNode->Flip( current_w, current_h );
					}
				}

				return;
			}

			if( EventKeyboard::KeyCode::KEY_R == keycode )
			{
				BuildStageView();
			}
		}
	}
}
