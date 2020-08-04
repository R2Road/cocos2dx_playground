#include "step_rain_of_chaos_game_test_SpawnProcessorScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"

#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_rain_of_chaos_game_BulletManager.h"
#include "step_rain_of_chaos_game_StageNode.h"
#include "step_rain_of_chaos_game_SpawnProcessor_Circle_01_OutToIn.h"
#include "step_rain_of_chaos_game_SpawnProcessor_CircularSector_01_1Direction.h"
#include "step_rain_of_chaos_game_SpawnProcessor_CircularSector_01_2Direction.h"
#include "step_rain_of_chaos_game_SpawnProcessor_MultipleShot_01_CircularSector.h"
#include "step_rain_of_chaos_game_SpawnProcessor_SingleShot_01.h"
#include "step_rain_of_chaos_game_SpawnProcessor_Sleep.h"

USING_NS_CC;

namespace
{
	const int BulletCachingAmount = 100;
	const int TAG_Button = 100;
}

namespace step_rain_of_chaos
{
	namespace game_test
	{
		SpawnProcessorScene::SpawnProcessorScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )

			, mStartButton_MoveOffset()
			, mTargetButton_MoveOffset()

			, mStageConfig()
			, mBulletManager( nullptr )
			, mStageNode( nullptr )
			, mTargetNode( nullptr )

			, mSpawnProcessorContainer()
			, mCurrentSpawnProcessor()
		{}

		Scene* SpawnProcessorScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) SpawnProcessorScene( back_to_the_previous_scene_callback );
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

		bool SpawnProcessorScene::init()
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
				ss << "[SPACE] : Start Process";

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
				auto background_layer = LayerColor::create( Color4B::BLACK );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			mStageConfig.Build(
				visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f
				, 120.f
			);

			//
			// Start Button
			//
			{
				Vec2 start_position = mStageConfig.GetCenter();
				start_position.y += ( mStageConfig.GetBulletGenerateArea().size.width * 0.5f );

				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setTag( TAG_Button );
				button->setColor( Color3B::BLUE );
				button->setPosition( start_position );
				button->addTouchEventListener( CC_CALLBACK_2( SpawnProcessorScene::onStartButton, this ) );
				addChild( button, std::numeric_limits<int>::max() - 1 );

				// Label
				{
					auto label = Label::createWithTTF( "S", "fonts/NanumSquareR.ttf", 10 );
					label->setPosition( Vec2(
						visibleOrigin.x
						, visibleOrigin.y + visibleSize.height
					) );
					button->setTitleLabel( label );
				}

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 2.f );
					pivot->setPosition( button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f );
					button->addChild( pivot, std::numeric_limits<int>::max() );
				}

				mStartNode = button;
			}

			//
			// Target Button
			//
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setTag( TAG_Button );
				button->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				button->addTouchEventListener( CC_CALLBACK_2( SpawnProcessorScene::onTargetButton, this ) );
				addChild( button, std::numeric_limits<int>::max() - 1 );

				// Label
				{
					auto label = Label::createWithTTF( "T", "fonts/NanumSquareR.ttf", 10 );
					label->setPosition( Vec2(
						visibleOrigin.x
						, visibleOrigin.y + visibleSize.height
					) );
					button->setTitleLabel( label );
				}

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 2.f );
					pivot->setPosition( button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f );
					button->addChild( pivot, std::numeric_limits<int>::max() );
				}

				mTargetNode = button;
			}

			//
			// Bullet Manager
			//
			{
				mBulletManager = game::BulletManager::create( BulletCachingAmount );
			}

			//
			// Stage Node
			//
			{
				mStageNode = game::StageNode::create(
					mStageConfig
					, game::StageNode::DebugConfig{ true, true }
					, mBulletManager->GetComeHomeCallback()
					, step_mole::CircleCollisionComponentConfig { false, false, false }
					, BulletCachingAmount
				);
				addChild( mStageNode );
			}

			//
			// Spawn Processor
			//
			{
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Circle_01_OutToIn::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, false, 50, 2.5f, 2 ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );

				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_CircularSector_01_1Direction::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, true, 60.f, 10, 4, 0.015f, 0.1f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_CircularSector_01_1Direction::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, true, 60.f, 10, 4, 0.015f, 0.1f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_CircularSector_01_1Direction::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, true, 60.f, 10, 4, 0.015f, 0.1f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );

				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_CircularSector_01_2Direction::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, false, 60.f, 10, 4, 0.025f, 0.3f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_CircularSector_01_2Direction::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, false, 60.f, 10, 4, 0.025f, 0.3f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_CircularSector_01_2Direction::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, false, 60.f, 10, 4, 0.025f, 0.3f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );

				mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_MultipleShot_01_CircularSector::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 90.f, 8, 4, 0.1f ) );
				mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
				mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_MultipleShot_01_CircularSector::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 45.f, 4, 3, 0.1f ) );
				mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
				mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_MultipleShot_01_CircularSector::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 45.f, 1, 4, 0.1f ) );
				mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );

				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 4, 0.1f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 3, 0.1f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 4, 0.1f ) );
				//mSpawnProcessorContainer.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
			}

			return true;
		}

		void SpawnProcessorScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SpawnProcessorScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void SpawnProcessorScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void SpawnProcessorScene::updateForSpawnProcessor( float dt )
		{
			if( mSpawnProcessorContainer.end() == mCurrentSpawnProcessor )
			{
				unschedule( schedule_selector( SpawnProcessorScene::updateForSpawnProcessor ) );
				return;
			}

			game::SpawnInfoContainer aaa;

			if( !( *mCurrentSpawnProcessor )->Update( dt, mStartNode->getPosition(), mTargetNode->getPosition(), &aaa ) )
			{
				++mCurrentSpawnProcessor;
				if( mSpawnProcessorContainer.end() != mCurrentSpawnProcessor )
				{
					( *mCurrentSpawnProcessor )->Enter( mStartNode->getPosition(), mTargetNode->getPosition() );
				}
			}

			if( !aaa.empty() )
			{
				int target_index = -1;
				for( const auto& s : aaa )
				{
					target_index = mBulletManager->GetIdleTarget();
					if( -1 == target_index )
					{
						mBulletManager->RequestGenerate( 50 );
						mStageNode->RequestGenerate( 50 );

						target_index = mBulletManager->GetIdleTarget();
						if( -1 == target_index )
						{
							break;
						}
					}

					Vec2 dir = s.MoveDirection;
					dir.normalize();
					dir.scale( 3.f );
					mStageNode->RequestAction( target_index, s.StartPosition, dir );
				}
			}
		}

		void SpawnProcessorScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_SPACE:
			{
				unschedule( schedule_selector( SpawnProcessorScene::updateForSpawnProcessor ) );

				mCurrentSpawnProcessor = mSpawnProcessorContainer.begin();
				( *mCurrentSpawnProcessor )->Enter( mStartNode->getPosition(), mTargetNode->getPosition() );
				schedule( schedule_selector( SpawnProcessorScene::updateForSpawnProcessor ) );
			}
			return;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void SpawnProcessorScene::onStartButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				mStartButton_MoveOffset = button->getPosition() - button->getTouchBeganPosition();
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				button->setPosition( button->getTouchMovePosition() + mStartButton_MoveOffset );
			}
		}
		void SpawnProcessorScene::onTargetButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				mTargetButton_MoveOffset = button->getPosition() - button->getTouchBeganPosition();
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				button->setPosition( button->getTouchMovePosition() + mTargetButton_MoveOffset );
			}
		}
	}
}
