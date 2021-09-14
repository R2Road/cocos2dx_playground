#include "step_rain_of_chaos_game_test_EnemyProcessorScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"
#include "ui/UILayout.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_rain_of_chaos_game_EnemyNode.h"
#include "step_rain_of_chaos_game_PlayerNode.h"
#include "step_rain_of_chaos_game_StageNode.h"

#include "step_rain_of_chaos_game_EnemyProcessor_Blink_CircularSector_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Fire_Single.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Fire_Chain.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_CircularSector_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_CircularSector_2Target_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_CircularSector_Random_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_CircularSector_Random_02.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_Linear_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_Linear_2Target_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_Linear_Random_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_Linear_Random_02.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_Orbit_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Sleep.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Tie.h"

#include "step_rain_of_chaos_game_SpawnProcessor_MultipleShot_02_Line.h"
#include "step_rain_of_chaos_game_SpawnProcessor_Sleep.h"

USING_NS_CC;

namespace
{
	const int BulletCachingAmount = 100;

	const int TAG_PackageIndicator = 10001;
}

namespace step_rain_of_chaos
{
	namespace game_test
	{
		EnemyProcessorScene::EnemyProcessorScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )

			, mStartButton_MoveOffset()
			, mPlayerLastTouchPosition()

			, mStageConfig()
			, mStageNode( nullptr )

			, mStartNode( nullptr )
			, mTargetNode( nullptr )

			, mPackgeContainer()
			, mCurrentPackage( nullptr )
		{}

		Scene* EnemyProcessorScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) EnemyProcessorScene( back_to_the_previous_scene_callback );
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

		bool EnemyProcessorScene::init()
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
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[SPACE] : Start Process";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 8, Size::ZERO, TextHAlignment::LEFT );
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
			// Stage Node
			//
			{
				mStageNode = game::StageNode::create(
					mStageConfig
					, game::StageNode::DebugConfig{ false, true, true, true }
					, step_mole::CircleCollisionComponentConfig { false, false, false }
					, BulletCachingAmount
				);
				addChild( mStageNode );
			}

			//
			// Player Node
			//
			{
				auto player_node = game::PlayerNode::create( 1.f, game::PlayerNode::DebugConfig{ true }, step_mole::CircleCollisionComponentConfig{ true, true, true } );
				player_node->setPosition( Vec2(
					static_cast<int>( visibleOrigin.x + ( visibleSize.width * 0.5f ) )
					, static_cast<int>( visibleOrigin.y + ( visibleSize.height * 0.5f ) )
				) );
				mStageNode->AddPlayer( player_node );

				// Move Helper
				{
					auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
					button->addTouchEventListener( CC_CALLBACK_2( EnemyProcessorScene::onTargetButton, this ) );
					player_node->addChild( button, std::numeric_limits<int>::max() - 1 );
				}

				mTargetNode = player_node;
			}

			//
			// Enemy Node
			//
			{
				Vec2 enemy_position = mStageConfig.GetCenter();
				enemy_position.y += ( mStageConfig.GetBulletGenerateRadiusMax() );

				auto enemy_node = game::EnemyNode::create(
					3.f
					, game::EnemyNode::DebugConfig{ true }
					, step_mole::CircleCollisionComponentConfig{ true, true, true }
					, []() {}
					, std::bind( &game::StageNode::RequestBulletAction, mStageNode, std::placeholders::_1, std::placeholders::_2 )
				);
				enemy_node->setPosition( enemy_position );
				mStageNode->AddEnemy( enemy_node );

				// Move Helper
				{
					auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
					button->addTouchEventListener( CC_CALLBACK_2( EnemyProcessorScene::onStartButton, this ) );
					enemy_node->addChild( button, std::numeric_limits<int>::max() - 1 );
				}

				mStartNode = enemy_node;
			}

			//
			// Processor
			//
			{
				auto enemy_node = static_cast<game::EnemyNode*>( mStartNode );

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Move_CircularSector_01";

					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, true, 180.f ) );

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Move_CircularSector_2Target_02";

					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_CircularSector_2Target_01::Create( mStageConfig, mStartNode, mTargetNode, 0.2f ) );

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Move_CircularSector_Random_01";

					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_01::Create( mStageConfig, mStartNode, mTargetNode, 0.2f, true, 30.f, 60.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_01::Create( mStageConfig, mStartNode, mTargetNode, 0.2f, true, 30.f, 60.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_01::Create( mStageConfig, mStartNode, mTargetNode, 0.2f, true, 30.f, 60.f ) );

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Move_CircularSector_Random_02";

					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_02::Create( mStageConfig, mStartNode, mTargetNode, 0.2f, 30.f, 40.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_02::Create( mStageConfig, mStartNode, mTargetNode, 0.2f, 30.f, 40.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_02::Create( mStageConfig, mStartNode, mTargetNode, 0.2f, 30.f, 40.f ) );

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Move_Linear_01";

					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Linear_01::Create( mStageConfig, mStartNode, mTargetNode, 0.4f, true, 180.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Linear_01::Create( mStageConfig, mStartNode, mTargetNode, 0.4f, true, 90.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Linear_01::Create( mStageConfig, mStartNode, mTargetNode, 0.4f, true, 180.f ) );
					

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Move_Linear_2Target_01";

					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Linear_2Target_01::Create( mStageConfig, mStartNode, mTargetNode, 0.4f ) );


					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Move_Linear_Random_01";

					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Linear_Random_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, true, 90.f, 180.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Linear_Random_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, true, 90.f, 180.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Linear_Random_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, true, 90.f, 180.f ) );

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Move_Linear_Random_02";

					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Linear_Random_02::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, 90.f, 180.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Linear_Random_02::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, 90.f, 180.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Linear_Random_02::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, 90.f, 180.f ) );

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Move_Orbit_01";

					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Orbit_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, 0.3f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Orbit_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, 1.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Orbit_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, 0.5f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Orbit_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, -1.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.05f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Move_Orbit_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, -0.5f ) );

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Blink_CircularSector_01";

					name_n_package.Package.emplace_back( game::EnemyProcessor_Blink_CircularSector_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, true, 60.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.5f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Blink_CircularSector_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, true, 60.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.5f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Blink_CircularSector_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, true, 60.f ) );
					name_n_package.Package.emplace_back( game::EnemyProcessor_Sleep::Create( 0.5f ) );

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Fire Single";

					{
						auto spawn_processor = game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 14.f, 1, 4, 0.1f );

						name_n_package.Package.emplace_back( game::EnemyProcessor_Fire_Single::Create(
							mStageConfig
							, mStartNode
							, mTargetNode
							, std::move( spawn_processor )
							, enemy_node->GetSpawnInfoContainer()
						) );
					}

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Fire Chain";

					{
						game::SpawnProcessorPackage spawn_processor_package;
						spawn_processor_package.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 98.f, 8, 4, 0.1f ) );
						spawn_processor_package.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
						spawn_processor_package.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 52.f, 4, 3, 0.1f ) );
						spawn_processor_package.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
						spawn_processor_package.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 14.f, 1, 4, 0.1f ) );

						name_n_package.Package.emplace_back( game::EnemyProcessor_Fire_Chain::Create(
							mStageConfig
							, mStartNode
							, mTargetNode
							, std::move( spawn_processor_package )
							, enemy_node->GetSpawnInfoContainer()
						) );
					}

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				{
					NameNPackage name_n_package;
					name_n_package.Name = "Tie";

					{
						game::SpawnProcessorPackage spawn_processor_container;
						spawn_processor_container.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 98.f, 8, 4, 0.1f ) );
						spawn_processor_container.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
						spawn_processor_container.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 52.f, 3, 3, 0.1f ) );
						spawn_processor_container.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
						spawn_processor_container.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 14.f, 2, 4, 0.1f ) );

						auto fire_processor = game::EnemyProcessor_Fire_Chain::Create(
							mStageConfig
							, mStartNode
							, mTargetNode
							, std::move( spawn_processor_container )
							, enemy_node->GetSpawnInfoContainer()
						);

						auto move_processor = game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, mStartNode, mTargetNode, 2.f, true, 180.f );

						name_n_package.Package.emplace_back( game::EnemyProcessor_Tie::Create(
							mStageConfig
							, mStartNode
							, mTargetNode
							, std::move( move_processor )
							, std::move( fire_processor )
						) );
					}

					mPackgeContainer.emplace_back( std::move( name_n_package ) );
				}

				mCurrentPackage = &mPackgeContainer[0u];
			}

			//
			// Package List
			//
			{
				const Size ItemSize( 120, 18 );
				const ui::Margin ItemMargin( 0.f, 0.f, 0.f, 0.f );
				const Size LayoutSize(
					ItemMargin.left + ItemSize.width + ItemMargin.right
					, ( ItemMargin.top + ItemSize.height + ItemMargin.bottom ) * mPackgeContainer.size()
				);

				auto layout_node = ui::Layout::create();
				layout_node->setContentSize( LayoutSize );
				layout_node->setLayoutType( ui::Layout::Type::VERTICAL );
				layout_node->setBackGroundColor( Color3B( 0, 148, 255 ) );
				layout_node->setBackGroundColorOpacity( 150u );
				layout_node->setBackGroundColorType( cocos2d::ui::Layout::BackGroundColorType::SOLID );
				layout_node->setPosition( Vec2(
					visibleOrigin
					+ Vec2( visibleSize.width, visibleSize.height * 0.5f )
					- Vec2( layout_node->getContentSize().width, layout_node->getContentSize().height * 0.5f )
				) );
				addChild( layout_node );

				for( std::size_t i = 0; mPackgeContainer.size() > i; ++i )
				{
					auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
					button->setTag( i );
					button->setScale9Enabled( true );
					button->setContentSize( ItemSize );
					button->addTouchEventListener( CC_CALLBACK_2( EnemyProcessorScene::onPackageSelect, this ) );

					// Label
					{
						auto label = Label::createWithTTF( mPackgeContainer[i].Name, cpg::StringTable::GetFontPath(), 7 );
						label->getFontAtlas()->setAliasTexParameters();
						button->setTitleLabel( label );
					}

					// Align
					{
						auto param = ui::LinearLayoutParameter::create();
						param->setMargin( ItemMargin );
						button->setLayoutParameter( param );
					}

					// Pivot
					{
						auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
						button->addChild( pivot, std::numeric_limits<int>::max() );
					}

					// Indicator
					{
						auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_4.png" );
						sprite->setTag( TAG_PackageIndicator );
						sprite->setAnchorPoint( Vec2::ZERO );
						sprite->setContentSize( ItemSize );
						sprite->setVisible( false );
						button->addChild( sprite, 1 );
					}

					layout_node->addChild( button );
				}

				//
				// Select
				//
				{
					onPackageSelect( layout_node->getChildByTag( 0 ), ui::Widget::TouchEventType::ENDED );
				}
			}

			return true;
		}

		void EnemyProcessorScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( EnemyProcessorScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void EnemyProcessorScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void EnemyProcessorScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_SPACE:
			{
				auto enemy_node = static_cast<game::EnemyNode*>( mStartNode );
				enemy_node->StartProcess( &mCurrentPackage->Package );
			}
			return;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void EnemyProcessorScene::onStartButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				mStartButton_MoveOffset = button->getParent()->getPosition() - button->getTouchBeganPosition();
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				button->getParent()->setPosition( button->getTouchMovePosition() + mStartButton_MoveOffset );
			}
		}
		void EnemyProcessorScene::onTargetButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				mPlayerLastTouchPosition = button->getTouchBeganPosition();
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				mStageNode->PlayerMoveRequest( button->getTouchMovePosition() - mPlayerLastTouchPosition );

				mPlayerLastTouchPosition = button->getTouchMovePosition();
			}
		}

		void EnemyProcessorScene::onPackageSelect( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::ENDED == touch_event_type )
			{
				auto enemy_node = static_cast<game::EnemyNode*>( mStartNode );
				enemy_node->StopProcess();

				auto node = static_cast<Node*>( sender );
				auto indicator_node = node->getChildByTag( TAG_PackageIndicator );
				if( !indicator_node->isVisible() )
				{
					for( auto c : node->getParent()->getChildren() )
					{
						c->getChildByTag( TAG_PackageIndicator )->setVisible( false );
					}

					indicator_node->setVisible( true );
				}

				mCurrentPackage = &mPackgeContainer[node->getTag()];
			}
		}
	}
}
