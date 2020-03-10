#include "step_clickclick_game_PlayScene.h"

#include <functional>
#include <new>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "audio/include/AudioEngine.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_clickclick_game_TitleScene.h"

#include "step_clickclick_game_Stage.h"
#include "step_clickclick_game_StageView.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		namespace
		{
			const int stage_width = 7;
			const int stage_height = 7;
		}

		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mStage()
			, mStageView( nullptr )
			, mGridIndexConverter( stage_width, stage_height )
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
				ss << "[Mouse] : Click";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
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
				auto background_layer = LayerColor::create( Color4B( 0, 41, 13, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// Stage
			//
			{
				mStage = step_clickclick::game::Stage::create( stage_width, stage_height );
				mStage->Setup( 5, 5 );
			}

			//
			// StageView
			//
			{
				mStageView = step_clickclick::game::StageView::create(
					stage_width, stage_height
					, std::bind( &PlayScene::onGameProcess, this, std::placeholders::_1 )
				);
				mStageView->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( mStageView );

				mStageView->Setup( *mStage );
			}

			return true;
		}

		void PlayScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void PlayScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void PlayScene::onGameProcess( const int pannel_linear_index )
		{
			experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg" );

			const auto& pannel_data = mStage->GetPannelData( pannel_linear_index );

			if( ePannelType::Single == pannel_data.GetType() )
			{
				mStage->DecreasePannelLife( pannel_data.GetIndex() );
				mStageView->UpdatePannel( pannel_data.GetIndex(), pannel_data.GetLife() );
			}
			else if( ePannelType::Together == pannel_data.GetType() )
			{
				const int pivot_count = pannel_data.GetLife();
				const auto point_index = mGridIndexConverter.To_Point( pannel_data.GetIndex() );

				const int current_pivot_x = point_index.x - 1;
				const int current_pivot_y = point_index.y - 1;
				for( int ty = current_pivot_y; ty < current_pivot_y + 3; ++ty )
				{
					for( int tx = current_pivot_x; tx < current_pivot_x + 3; ++tx )
					{
						if( 0 > tx || mStage->GetWidth() <= tx
							|| 0 > ty || mStage->GetHeight() <= ty )
						{
							continue;
						}

						const auto& target_pannel_data = mStage->GetPannelData( mGridIndexConverter.To_Linear( tx, ty ) );
						if( !target_pannel_data.IsActive() )
						{
							continue;
						}

						if( ePannelType::Together == target_pannel_data.GetType() && pivot_count != target_pannel_data.GetLife() )
						{
							continue;
						}

						if( pivot_count != target_pannel_data.GetLife() )
						{
							mStage->IncreasePannelLife( target_pannel_data.GetIndex() );
						}
						else
						{
							mStage->DecreasePannelLife( target_pannel_data.GetIndex() );
						}

						mStageView->UpdatePannel( target_pannel_data.GetIndex(), target_pannel_data.GetLife() );
					}
				}
			}
			else if( ePannelType::Different == pannel_data.GetType() )
			{
				const int pivot_count = pannel_data.GetLife();
				const auto point_index = mGridIndexConverter.To_Point( pannel_data.GetIndex() );

				const int current_pivot_x = point_index.x - 1;
				const int current_pivot_y = point_index.y - 1;
				for( int ty = current_pivot_y; ty < current_pivot_y + 3; ++ty )
				{
					for( int tx = current_pivot_x; tx < current_pivot_x + 3; ++tx )
					{
						if( 0 > tx || mStage->GetWidth() <= tx
							|| 0 > ty || mStage->GetHeight() <= ty )
						{
							continue;
						}

						const auto& target_pannel_data = mStage->GetPannelData( mGridIndexConverter.To_Linear( tx, ty ) );
						if( !target_pannel_data.IsActive() )
						{
							continue;
						}

						if( target_pannel_data.GetIndex() != pannel_data.GetIndex() && pivot_count == target_pannel_data.GetLife() )
						{
							mStage->IncreasePannelLife( target_pannel_data.GetIndex() );
						}
						else
						{
							mStage->DiePannelLife( target_pannel_data.GetIndex() );
						}

						mStageView->UpdatePannel( target_pannel_data.GetIndex(), target_pannel_data.GetLife() );
					}
				}
			}
		}


		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_clickclick::game::TitleScene::create() );
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	} // namespace game
} // namespace step_clickclick
