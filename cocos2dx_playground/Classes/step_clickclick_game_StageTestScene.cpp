#include "step_clickclick_game_StageTestScene.h"

#include <cassert>
#include <functional>
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

#include "step_clickclick_RootScene.h"

#include "step_clickclick_game_Stage.h"
#include "step_clickclick_game_StageView.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		namespace
		{
			const int MAX_STAGE_WIDTH = 7;
			const int MAX_STAGE_HEIGHT = 7;

			const int TAG_ActiveBlockCountView = 9999;
		}

		StageTestScene::StageTestScene() :
			mKeyboardListener( nullptr )
			, mStage()
			, mStageView( nullptr )
			, mGridIndexConverter( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT )
			, mScore( 0 )
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
				ss << "[Mouse] : Click : Play";
				ss << std::endl;
				ss << std::endl;
				ss << "[R] : Reset";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 0, 41, 13, 255 ) );
				addChild( background_layer, -1 );
			}

			//
			// Stage
			//
			{
				mStage = step_clickclick::game::Stage::create( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT );
				mStage->Setup( 5, 5 );
			}

			//
			// StageView
			//
			{
				mStageView = step_clickclick::game::StageView::create(
					MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT
					, std::bind( &StageTestScene::onGameProcess, this, std::placeholders::_1 )
					, StageViewConfig{ true, true }
				);
				mStageView->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( mStageView );

				mStageView->Setup( *mStage );
			}

			//
			// Active Block Count View
			//
			{
				auto label = Label::createWithTTF( "Block", "fonts/arial.ttf", 10 );
				label->setTag( TAG_ActiveBlockCountView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.1f
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateActiveBlockCountView( mStage->GetActiveBlockCount() );
			}

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

			Scene::onExit();
		}


		void StageTestScene::onGameProcess( const int block_linear_index )
		{
			const auto& block_data = mStage->GetBlockData( block_linear_index );
			const auto block_point_index = mStage->ConvertLinearIndex2PointIndex( block_data.GetIndex() );
			int last_life = 0;

			if( eBlockType::Single == block_data.GetType() )
			{
				experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.1f );

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
				experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );

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
				experimental::AudioEngine::play2d( "sounds/fx/coin_001.ogg", false, 0.2f );

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

			updateActiveBlockCountView( mScore );
		}


		void StageTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_clickclick::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_R:
				mStage->Setup( 5, 5 );
				mStageView->Setup( *mStage );
				mScore = 0;
				updateActiveBlockCountView( mScore );
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void StageTestScene::updateActiveBlockCountView( const int count )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ActiveBlockCountView ) );
			label->setString( StringUtils::format( "Remain Block Count : %d", count ) );
		}
	} // namespace game
} // namespace step_clickclick
