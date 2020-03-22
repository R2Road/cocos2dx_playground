#include "step_clickclick_game_PlayScene.h"

#include <functional>
#include <new>
#include <numeric>
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
			, mAudioID_forBGM( -1 )

			, mStage()
			, mStageView( nullptr )

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
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << "[Mouse] : Click";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 8 );
				label->setColor( Color3B::WHITE );
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
				auto background_layer = LayerColor::create( Color4B::BLACK );
				addChild( background_layer, -1 );
			}

			//
			// BGM License
			//
			{
				auto label = Label::createWithTTF(
					"BGM : Somewhere in the Elevator\nAuthor : Peachtea@You're Perfect Studio\nLicense : CC-BY 4.0\nFrom : https://opengameart.org/"
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
					, StageViewConfig{ false, false }
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
				label->setColor( Color3B::RED );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateScoreView();
			}

			//
			// Clear View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 16 );
				label->setTag( TAG_ClearView );
				label->setVisible( false );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.6f
				) );
				addChild( label, 1 );

				updateScoreView();
			}

			//
			// Count View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 16 );
				label->setTag( TAG_CountView );
				label->setVisible( false );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.4f
				) );
				addChild( label, 1 );

				updateCountView( mNextStepData.LimitTime_forCount );
			}

			return true;
		}

		void PlayScene::onEnter()
		{
			Scene::onEnter();

			mAudioID_forBGM = experimental::AudioEngine::play2d( "sounds/bgm/Somewhere_in_the_Elevator.ogg", true, 0.1f );

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void PlayScene::onExit()
		{
			experimental::AudioEngine::stop( mAudioID_forBGM );

			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void PlayScene::onGameProcess( const int block_linear_index )
		{
			const auto& block_data = mStage->GetBlockData( block_linear_index );
			const auto block_point_index = mStage->ConvertLinearIndex2PointIndex( block_data.GetIndex() );
			int last_life = 0;

			if( eBlockType::Single == block_data.GetType() )
			{
				experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.2f );

				bool has_neighbor = false;
				const int current_pivot_x = block_point_index.x - 1;
				const int current_pivot_y = block_point_index.y - 1;
				for( int ty = current_pivot_y; ty < current_pivot_y + 3; ++ty )
				{
					for( int tx = current_pivot_x; tx < current_pivot_x + 3; ++tx )
					{
						if( 0 > tx || mStage->GetWidth() <= tx
							|| 0 > ty || mStage->GetHeight() <= ty )
						{
							continue;
						}

						if( tx != block_point_index.x && ty != block_point_index.y )
						{
							continue;
						}

						const auto& target_block_data = mStage->GetBlockData( tx, ty );
						if( block_linear_index == target_block_data.GetIndex() )
						{
							continue;
						}

						if( !target_block_data.IsActive() )
						{
							continue;
						}

						has_neighbor = true;
						break;
					}
				}

				last_life = block_data.GetLife();
				if( has_neighbor )
				{
					++mScore;

					mStage->DecreaseBlockLife( block_data.GetIndex() );
					mStageView->UpdateBlock( block_data.GetIndex(), last_life, block_data.GetLife() );
				}
				else
				{
					mScore = std::max( 0, mScore - block_data.GetLife() );

					mStage->DieBlock( block_data.GetIndex() );
					mStageView->UpdateBlock( block_data.GetIndex(), last_life, block_data.GetLife() );
				}
			}
			else if( eBlockType::Same == block_data.GetType() )
			{
				experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.2f );

				const int pivot_count = block_data.GetLife();

				const int current_pivot_x = block_point_index.x - 1;
				const int current_pivot_y = block_point_index.y - 1;
				for( int ty = current_pivot_y; ty < current_pivot_y + 3; ++ty )
				{
					for( int tx = current_pivot_x; tx < current_pivot_x + 3; ++tx )
					{
						if( 0 > tx || mStage->GetWidth() <= tx
							|| 0 > ty || mStage->GetHeight() <= ty )
						{
							continue;
						}

						const auto& target_block_data = mStage->GetBlockData( tx, ty );
						if( !target_block_data.IsActive() )
						{
							continue;
						}

						if( eBlockType::Same == target_block_data.GetType() && pivot_count != target_block_data.GetLife() )
						{
							continue;
						}

						last_life = target_block_data.GetLife();
						if( pivot_count != target_block_data.GetLife() )
						{
							mStage->IncreaseBlockLife( target_block_data.GetIndex() );
						}
						else
						{
							mScore += 3;
							mStage->DecreaseBlockLife( target_block_data.GetIndex() );
						}

						mStageView->UpdateBlock( target_block_data.GetIndex(), last_life, target_block_data.GetLife() );
					}
				}
			}
			else if( eBlockType::Different == block_data.GetType() )
			{
				experimental::AudioEngine::play2d( "sounds/fx/powerup_001.ogg", false, 0.1f );

				const int pivot_count = block_data.GetLife();

				const int current_pivot_x = block_point_index.x - 1;
				const int current_pivot_y = block_point_index.y - 1;
				for( int ty = current_pivot_y; ty < current_pivot_y + 3; ++ty )
				{
					for( int tx = current_pivot_x; tx < current_pivot_x + 3; ++tx )
					{
						if( 0 > tx || mStage->GetWidth() <= tx
							|| 0 > ty || mStage->GetHeight() <= ty )
						{
							continue;
						}

						const auto& target_block_data = mStage->GetBlockData( tx, ty );
						if( !target_block_data.IsActive() )
						{
							continue;
						}

						last_life = target_block_data.GetLife();
						if( target_block_data.GetIndex() != block_data.GetIndex() && pivot_count == target_block_data.GetLife() )
						{
							mStage->IncreaseBlockLife( target_block_data.GetIndex() );
							mStage->IncreaseBlockLife( target_block_data.GetIndex() );
							mStage->IncreaseBlockLife( target_block_data.GetIndex() );
							mStage->IncreaseBlockLife( target_block_data.GetIndex() );
						}
						else
						{
							mScore += target_block_data.GetLife();
							mStage->DieBlock( target_block_data.GetIndex() );
						}

						mStageView->UpdateBlock( target_block_data.GetIndex(), last_life, target_block_data.GetLife() );
					}
				}
			}

			updateScoreView();

			//
			// Stage Clear
			//
			if( !mStage->HasActiveBlock() )
			{
				experimental::AudioEngine::play2d( "sounds/fx/powerup_001.ogg", false, 0.1f );
				schedule( SEL_SCHEDULE( &PlayScene::updateForNextStep ) );
			}
		}

		void PlayScene::updateScoreView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ScoreView ) );
			label->setString( StringUtils::format( "Score : %4d", mScore ) );
		}
		void PlayScene::updateCountView( const float count )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_CountView ) );
			label->setString( StringUtils::format( "%.1f", count ) );
		}

		void PlayScene::updateForNextStep( float dt )
		{
			switch( mNextStepData.Step )
			{
			case NextStepData::eStep::wait_for_entry:
				mNextStepData.ElapsedTime_forEntry += dt;
				if( mNextStepData.LimitTime_forEntry < mNextStepData.ElapsedTime_forEntry )
				{
					mStageView->setVisible( false );
					mNextStepData.ElapsedTime_forEntry = 0.f;

					++mNextStepData.Step;
				}
				break;
			case NextStepData::eStep::show_clear_indicator:
			{
				auto clear_view_label = static_cast<Label*>( getChildByTag( TAG_ClearView ) );
				clear_view_label->setString( "Stage Clear" );
				clear_view_label->setVisible( true );

				auto count_view_label = static_cast<Label*>( getChildByTag( TAG_CountView ) );
				count_view_label->setVisible( true );
				updateCountView( mNextStepData.LimitTime_forCount );

				mCurrentStageWidth += 2;
				mCurrentStageHeight += 2;
				if( MAX_STAGE_WIDTH >= mCurrentStageWidth )
				{
					++mNextStepData.Step;
				}
				else
				{
					mNextStepData.Step = NextStepData::eStep::game_clear;
				}
			}
			break;
			case NextStepData::eStep::wait_for_count:
				mNextStepData.ElapsedTime_forCount += dt;
				if( mNextStepData.LimitTime_forCount < mNextStepData.ElapsedTime_forCount )
				{
					mNextStepData.ElapsedTime_forCount = 0.f;
					updateCountView( 0.f );

					++mNextStepData.Step;
				}
				else
				{
					updateCountView( mNextStepData.LimitTime_forCount - mNextStepData.ElapsedTime_forCount );
				}
				break;
			case NextStepData::eStep::hide_clear_indicator:
				getChildByTag( TAG_ClearView )->setVisible( false );
				getChildByTag( TAG_CountView )->setVisible( false );
				mStage->Setup( mCurrentStageWidth, mCurrentStageHeight );
				mStageView->Setup( *mStage );
				++mNextStepData.Step;
				break;
			case NextStepData::eStep::reset:
				mStageView->setVisible( true );
				unschedule( SEL_SCHEDULE( &PlayScene::updateForNextStep ) );
				mNextStepData.Step = NextStepData::eStep::wait_for_entry;
				break;

			case NextStepData::eStep::game_clear:
				unschedule( SEL_SCHEDULE( &PlayScene::updateForNextStep ) );
				Director::getInstance()->replaceScene( step_clickclick::game::ResultScene::create( mScore ) );
				break;

			default:
				assert( false );
			}
		}

		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE != keycode )
			{
				return;
			}

			Director::getInstance()->replaceScene( step_clickclick::game::TitleScene::create() );
		}
	} // namespace game
} // namespace step_clickclick
