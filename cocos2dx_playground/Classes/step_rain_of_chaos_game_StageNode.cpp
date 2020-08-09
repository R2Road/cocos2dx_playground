#include "step_rain_of_chaos_game_StageNode.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_Clamp.h"

#include "step_mole_AnimationComponent.h"
#include "step_mole_CircleCollisionComponent.h"
#include "step_rain_of_chaos_game_AnimationInfoContainer.h"
#include "step_rain_of_chaos_game_BulletLifeComponent.h"
#include "step_rain_of_chaos_game_BulletManager.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		StageNode::StageNode(
			const StageConfig stage_config
			, const DebugConfig debug_config
			, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
		) :
			mStageConfig( stage_config )
			, mDebugConfig( debug_config )
			, mCircleCollisionComponentConfig( circle_collision_component_config )

			, mBulletManager( nullptr )
			, mBulletLifeComponentList()
			, mCollisionComponentList()
			, mBulletCount( 0 )

			, mPlayerNode( nullptr )
			, mPlayerCircleCollisionComponent( nullptr )
		{}

		StageNode* StageNode::create(
			const StageConfig stage_config
			, const DebugConfig debug_config
			, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
			, const int bullet_count
		)
		{
			auto ret = new ( std::nothrow ) StageNode( stage_config, debug_config, circle_collision_component_config );
			if( !ret || !ret->init( bullet_count ) )
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

		bool StageNode::init( const int bullet_count )
		{
			if( !Node::init() )
			{
				return false;
			}

			schedule( schedule_selector( StageNode::update4Collision ) );
			setContentSize( _director->getVisibleSize() );

			//
			// Pivot
			//
			if( mDebugConfig.bShowPivot )
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 4.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Stage Area Guide
			//
			if( mDebugConfig.bShowAreaGuide )
			{
				// Stage Area View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( mStageConfig.GetStageArea().size );
					sprite->setColor( Color3B::GREEN );
					sprite->setPosition( mStageConfig.GetStageArea().origin );
					addChild( sprite, std::numeric_limits<int>::min() );
					{
						auto label = Label::createWithTTF( "Stage Area", "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::RIGHT );
						label->setAnchorPoint( Vec2( 1.f, 1.f ) );
						label->setColor( Color3B::GREEN );
						label->setPosition( Vec2(
							sprite->getContentSize().width
							, sprite->getContentSize().height
						) );
						sprite->addChild( label );
					}
				}

				// Bullet Life Area View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( mStageConfig.GetBulletLifeArea().size );
					sprite->setColor( Color3B::RED );
					sprite->setPosition( mStageConfig.GetBulletLifeArea().origin );
					addChild( sprite, std::numeric_limits<int>::min() );
					{
						auto label = Label::createWithTTF( "Bullet Life Area", "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::RIGHT );
						label->setAnchorPoint( Vec2( 1.f, 1.f ) );
						label->setColor( Color3B::RED );
						label->setPosition( Vec2(
							sprite->getContentSize().width
							, sprite->getContentSize().height
						) );
						sprite->addChild( label );
					}
				}


				// Bullet Generate Area View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( mStageConfig.GetBulletGenerateArea().size );
					sprite->setColor( Color3B::WHITE );
					sprite->setPosition( mStageConfig.GetBulletGenerateArea().origin );
					addChild( sprite, std::numeric_limits<int>::min() );
					{
						auto label = Label::createWithTTF( "Bullet Generate Area", "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::RIGHT );
						label->setAnchorPoint( Vec2( 1.f, 1.f ) );
						label->setColor( Color3B::WHITE );
						label->setPosition( Vec2(
							sprite->getContentSize().width
							, sprite->getContentSize().height
						) );
						sprite->addChild( label );
					}
				}
			}

			//
			// Bullet
			//
			{
				mBulletManager = game::BulletManager::create();

				RequestGenerateBullet( bullet_count );
			}

			return true;
		}

		Node* StageNode::MakeBullet(
			const int index
			, const BulletProcessExitCallback& target_rest_callback
			, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
			, const bool bShowPivot
		)
		{
			auto root_node = Node::create();
			root_node->setVisible( false );
			root_node->setTag( index );
			{
				// Pivot
				if( bShowPivot )
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 2.f );
					root_node->addChild( pivot, std::numeric_limits<int>::max() );
				}

				// View
				auto view_node = Sprite::createWithSpriteFrameName( "step_rain_of_chaos_bullet_01_0.png" );
				view_node->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				view_node->setScale( _director->getContentScaleFactor() );
				root_node->addChild( view_node );

				// Animation Component
				auto animation_component = step_mole::AnimationComponent::create( step_rain_of_chaos::game::GetBulletAnimationInfoContainer() );
				view_node->addComponent( animation_component );

				// Collision Component
				auto circle_collision_component = step_mole::CircleCollisionComponent::create( 4.f, Vec2::ZERO, circle_collision_component_config );
				root_node->addComponent( circle_collision_component );

				// Object Component
				root_node->addComponent( BulletLifeComponent::create( mStageConfig.GetBulletLifeArea(), animation_component, circle_collision_component, target_rest_callback ) );
			}

			return root_node;
		}


		void StageNode::update4Collision( float /*dt*/ )
		{
			if( mBulletManager->GetLiveTargetContainer().empty() )
			{
				return;
			}

			for( auto cur = mBulletManager->GetLiveTargetContainer().begin(), end = mBulletManager->GetLiveTargetContainer().end(); end != cur; ++cur )
			{
				if( mCollisionComponentList[*cur]->Check( mPlayerCircleCollisionComponent ) )
				{
					CCLOG( "Col : %d", *cur );
				}
			}
		}


		void StageNode::AddPlayer( Node* player_node )
		{
			assert( nullptr == mPlayerNode );
			assert( nullptr != static_cast<step_mole::CircleCollisionComponent*>( player_node->getComponent( step_mole::CircleCollisionComponent::GetStaticName() ) ) );

			mPlayerNode = player_node;
			mPlayerNode->setPosition( getContentSize().width * 0.5f, getContentSize().height * 0.5f );
			addChild( mPlayerNode );

			mPlayerCircleCollisionComponent = static_cast<step_mole::CircleCollisionComponent*>( mPlayerNode->getComponent( step_mole::CircleCollisionComponent::GetStaticName() ) );
		}
		void StageNode::PlayerMoveRequest( const Vec2& move_vector )
		{
			const Vec2 new_player_position(
				cpg::clamp( mPlayerNode->getPosition().x + move_vector.x, mStageConfig.GetStageArea().getMinX(), mStageConfig.GetStageArea().getMaxX() )
				, cpg::clamp( mPlayerNode->getPosition().y + move_vector.y, mStageConfig.GetStageArea().getMinY(), mStageConfig.GetStageArea().getMaxY() )
			);

			mPlayerNode->setPosition( new_player_position );
		}

		void StageNode::RequestGenerateBullet( const int amount )
		{
			// Bullet Manager
			mBulletManager->RequestGenerate( amount );

			// Bullet Node
			const int result_amount = mBulletCount + amount;
			mBulletLifeComponentList.resize( result_amount );
			mCollisionComponentList.resize( result_amount );

			for( ; result_amount > mBulletCount; ++mBulletCount )
			{
				auto bullet_node = MakeBullet(
					mBulletCount
					, mBulletManager->GetComeHomeCallback()
					, mCircleCollisionComponentConfig
					, mDebugConfig.bShowPivot
				);
				bullet_node->setPosition( mBulletCount * 2, 100.f );
				addChild( bullet_node );

				mBulletLifeComponentList[mBulletCount] = static_cast<BulletLifeComponent*>( bullet_node->getComponent( BulletLifeComponent::GetStaticName() ) );
				mCollisionComponentList[mBulletCount] = static_cast<step_mole::CircleCollisionComponent*>( bullet_node->getComponent( step_mole::CircleCollisionComponent::GetStaticName() ) );
			}
		}
		void StageNode::RequestBulletAction( const Vec2 start_position, const Vec2 move_direction )
		{
			int bullet_index = mBulletManager->GetIdleTarget();
			if( -1 == bullet_index )
			{
				RequestGenerateBullet( 50 );

				bullet_index = mBulletManager->GetIdleTarget();
				if( -1 == bullet_index )
				{
					return;
				}
			}

			mBulletLifeComponentList[bullet_index]->ProcessStart( start_position, move_direction );
		}
	}
}

