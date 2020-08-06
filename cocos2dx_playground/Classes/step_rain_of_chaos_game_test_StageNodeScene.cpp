#include "step_rain_of_chaos_game_test_StageNodeScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "step_mole_AnimationComponent.h"
#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_rain_of_chaos_game_AnimationInfoContainer.h"
#include "step_rain_of_chaos_game_StageNode.h"
#include "step_rain_of_chaos_game_BulletManager.h"

USING_NS_CC;

namespace
{
	const int TAG_MoveSpeedView = 20140416;
	const int BulletCachingAmount = 1000;
	const int TAG_FireAmountView = 20160528;
}

namespace step_rain_of_chaos
{
	namespace game_test
	{
		StageNodeScene::StageNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mStageConfig()
			, mBulletManager( nullptr )
			, mStageNode( nullptr )
			, mCurrentMoveSpeed( 3 )
			, mCurrentFireAmount( 1 )

			, mKeyCodeCollector()
		{}

		Scene* StageNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) StageNodeScene( back_to_the_previous_scene_callback );
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

		bool StageNodeScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			schedule( schedule_selector( StageNodeScene::UpdateForInput ) );

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
				ss << "[SPACE] : Do Bullet";
				ss << std::endl;
				ss << std::endl;
				ss << "[Q] : Move Speed - Increase";
				ss << std::endl;
				ss << "[W] : Move Speed - Decrease";
				ss << std::endl;
				ss << std::endl;
				ss << "[A] : Fire Amount - Increase";
				ss << std::endl;
				ss << "[S] : Fire Amount - Decrease";

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
				auto background_layer = LayerColor::create( Color4B( 63, 23, 14, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Current Move Speed
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
				label->setTag( TAG_MoveSpeedView );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateMoveSpeedView();
			}

			//
			// Spawn Target Count
			//
			{
				auto label = Label::createWithTTF( "Temp", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
				label->setTag( TAG_FireAmountView );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height - label->getContentSize().height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateFireAmountView();
			}

			//
			// Target Manager
			//
			{
				mBulletManager = game::BulletManager::create( BulletCachingAmount );
			}

			//
			// Stage Node
			//
			{
				mStageConfig.Build(
					visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f
					, 120.f
				);

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
			// Player Node
			//
			//
			// Animation
			//
			{
				auto animation_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				animation_node->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				animation_node->setScale( _director->getContentScaleFactor() );
				{
					// Animation Component
					auto animation_component = step_mole::AnimationComponent::create( step_rain_of_chaos::game::GetActorAnimationInfoContainer() );
					animation_node->addComponent( animation_component );

					animation_component->PlayAnimation( cpg::animation::eIndex::run );
				}

				mStageNode->AddPlayer( animation_node );
			}

			return true;
		}

		void StageNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( StageNodeScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( StageNodeScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void StageNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void StageNodeScene::UpdateForInput( float dt )
		{
			Vec2 move_vector;
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_UP_ARROW ) )
			{
				move_vector.y += 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_DOWN_ARROW ) )
			{
				move_vector.y -= 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) )
			{
				move_vector.x += 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_LEFT_ARROW ) )
			{
				move_vector.x -= 1.f;
			}

			if( 0.f != move_vector.x || 0.f != move_vector.y )
			{
				move_vector.normalize();
				move_vector.scale( 3.f );

				mStageNode->PlayerMoveRequest( move_vector );
			}
		}

		void StageNodeScene::updateMoveSpeedView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_MoveSpeedView ) );
			label->setString( StringUtils::format( "Move Speed : %d", mCurrentMoveSpeed ) );
		}
		void StageNodeScene::updateFireAmountView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_FireAmountView ) );
			label->setString( StringUtils::format( "Fire Count : %d", mCurrentFireAmount ) );
		}
		void StageNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_SPACE:
			{
				Vec2 offset;

				int target_index = -1;
				for( int i = 0; i < mCurrentFireAmount; ++i )
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

					Vec2 dir = Vec2( mStageConfig.GetStageArea().getMaxX(), mStageConfig.GetStageArea().getMaxY() ) - mStageConfig.GetStageArea().origin;
					dir.normalize();
					dir.scale( mCurrentMoveSpeed );
					mStageNode->RequestAction( target_index, Vec2( mStageConfig.GetStageArea().origin ) + offset, dir );

					offset.y += 0.5;
				}
			}
			break;

			case EventKeyboard::KeyCode::KEY_Q:
				mCurrentMoveSpeed += 1;
				updateMoveSpeedView();
				break;
			case EventKeyboard::KeyCode::KEY_W:
				mCurrentMoveSpeed = std::max( 1, mCurrentMoveSpeed - 1 );
				updateMoveSpeedView();
				break;

			case EventKeyboard::KeyCode::KEY_A:
				mCurrentFireAmount += 1;
				updateFireAmountView();
				break;
			case EventKeyboard::KeyCode::KEY_S:
				mCurrentFireAmount = std::max( 1, mCurrentFireAmount - 1 );
				updateFireAmountView();
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}

			mKeyCodeCollector.onKeyPressed( keycode );
		}

		void StageNodeScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			mKeyCodeCollector.onKeyReleased( keycode );
		}
	}
}
