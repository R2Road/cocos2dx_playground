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
#include "base/ccUTF8.h"

#include "step_clickclick_game_TitleScene.h"
#include "step_clickclick_game_ResultScene.h"

#include "step_clickclick_game_Stage.h"
#include "step_clickclick_game_StageView.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		namespace
		{
			const int TAG_ScoreView = 20140416;
			const int TAG_ClearView = 20160528;
			const int TAG_CountView = 9999;

			const int MAX_STAGE_WIDTH = 9;
			const int MAX_STAGE_HEIGHT = 9;
		}

		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mStage()
			, mStageView( nullptr )
			, mGridIndexConverter( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT )

			, mScore( 0 )
			, mCurrentStageWidth( 3 )
			, mCurrentStageHeight( 3 )
			, mNextStepData()
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
				auto background_layer = LayerColor::create( Color4B::BLACK );
				addChild( background_layer, 0 );
			}

			//
			// Stage
			//
			{
				mStage = step_clickclick::game::Stage::create( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT );
				mStage->Setup( mCurrentStageWidth, mCurrentStageHeight );
			}

			//
			// StageView
			//
			{
				mStageView = step_clickclick::game::StageView::create(
					MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT
					, std::bind( &PlayScene::onGameProcess, this, std::placeholders::_1 )
				);
				mStageView->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( mStageView );

				mStageView->Setup( *mStage );
			}

			//
			// Score View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 12 );
				label->setTag( TAG_ScoreView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );

				updateScoreView();
			}

			//
			// Clear View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 16 );
				label->setTag( TAG_ClearView );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.6f
				) );
				addChild( label, 9999 );

				updateScoreView();
			}

			//
			// Count View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 16 );
				label->setTag( TAG_CountView );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.4f
				) );
				addChild( label, 9999 );

				updateScoreView();
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
			experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.5f );

			const auto& pannel_data = mStage->GetPannelData( pannel_linear_index );
			int last_life = 0;

			if( ePannelType::Single == pannel_data.GetType() )
			{
				bool has_neighbor = false;
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

						if( tx != point_index.x && ty != point_index.y )
						{
							continue;
						}

						const auto& target_pannel_data = mStage->GetPannelData( mGridIndexConverter.To_Linear( tx, ty ) );
						if( pannel_linear_index == target_pannel_data.GetIndex() )
						{
							continue;
						}

						if( !target_pannel_data.IsActive() )
						{
							continue;
						}

						has_neighbor = true;
						break;
					}
				}

				last_life = pannel_data.GetLife();
				if( has_neighbor )
				{
					++mScore;

					mStage->DecreasePannelLife( pannel_data.GetIndex() );
					mStageView->UpdatePannel( pannel_data.GetIndex(), last_life, pannel_data.GetLife() );
				}
				else
				{
					mScore = std::max( 0, mScore - pannel_data.GetLife() );

					mStage->DiePannel( pannel_data.GetIndex() );
					mStageView->UpdatePannel( pannel_data.GetIndex(), last_life, pannel_data.GetLife() );
				}
			}
			else if( ePannelType::Same == pannel_data.GetType() )
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

						if( ePannelType::Same == target_pannel_data.GetType() && pivot_count != target_pannel_data.GetLife() )
						{
							continue;
						}

						last_life = target_pannel_data.GetLife();
						if( pivot_count != target_pannel_data.GetLife() )
						{
							mStage->IncreasePannelLife( target_pannel_data.GetIndex() );
						}
						else
						{
							mScore += 3;
							mStage->DecreasePannelLife( target_pannel_data.GetIndex() );
						}

						mStageView->UpdatePannel( target_pannel_data.GetIndex(), last_life, target_pannel_data.GetLife() );
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

						last_life = target_pannel_data.GetLife();
						if( target_pannel_data.GetIndex() != pannel_data.GetIndex() && pivot_count == target_pannel_data.GetLife() )
						{
							mStage->IncreasePannelLife( target_pannel_data.GetIndex() );
						}
						else
						{
							mScore += target_pannel_data.GetLife();
							mStage->DiePannel( target_pannel_data.GetIndex() );
						}

						mStageView->UpdatePannel( target_pannel_data.GetIndex(), last_life, target_pannel_data.GetLife() );
					}
				}
			}

			updateScoreView();

			//
			// Game Over
			//
			if( !mStage->HasActivePannel() )
			{
				mStageView->setVisible( false );
				schedule( SEL_SCHEDULE( &PlayScene::updateForNextStep ) );
			}
		}

		void PlayScene::updateScoreView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ScoreView ) );
			label->setString( StringUtils::format( "Score : %4d", mScore ) );
		}

		void PlayScene::updateForNextStep( float dt )
		{
			switch( mNextStepData.step )
			{
			case 0: // show label - clear
			{
				auto label = static_cast<Label*>( getChildByTag( TAG_ClearView ) );
				label->setString( "Stage Clear" );
				label->setVisible( true );

				mCurrentStageWidth += 2;
				mCurrentStageHeight += 2;
				if( MAX_STAGE_WIDTH >= mCurrentStageWidth )
				{
					++mNextStepData.step;
				}
				else
				{
					mNextStepData.step = 7;
				}
			}
			break;
			case 1: // show label - count
			{
				auto label = static_cast<Label*>( getChildByTag( TAG_CountView ) );
				label->setString( std::to_string( mNextStepData.LimitTime ) );
				label->setVisible( true );

				++mNextStepData.step;
			}
			break;
			case 2: // wait
				mNextStepData.elapsedTime += dt;
				if( mNextStepData.LimitTime < mNextStepData.elapsedTime )
				{
					mNextStepData.elapsedTime = 0.f;
					auto label = static_cast<Label*>( getChildByTag( TAG_CountView ) );
					label->setString( "0" );

					++mNextStepData.step;
				}
				else
				{
					auto label = static_cast<Label*>( getChildByTag( TAG_CountView ) );
					label->setString( StringUtils::format( "%.1f", mNextStepData.LimitTime - mNextStepData.elapsedTime ) );
				}
				break;
			case 3: // hide label
				getChildByTag( TAG_ClearView )->setVisible( false );
				getChildByTag( TAG_CountView )->setVisible( false );
				mStage->Setup( mCurrentStageWidth, mCurrentStageHeight );
				mStageView->Setup( *mStage );
				++mNextStepData.step;
				break;
			case 4: // restart
				mStageView->setVisible( true );
				unschedule( SEL_SCHEDULE( &PlayScene::updateForNextStep ) );
				mNextStepData.step = 0;
				break;

			case 7:
				unschedule( SEL_SCHEDULE( &PlayScene::updateForNextStep ) );
				Director::getInstance()->replaceScene( step_clickclick::game::ResultScene::create( mScore ) );
				break;

			default:
				assert( false );
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
