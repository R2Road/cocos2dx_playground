#include "step_rain_of_chaos_game_EnemyNode.h"

#include <new>
#include <numeric>
#include <utility>

#include "2d/CCSprite.h"
#include "base/CCDirector.h"

#include "step_mole_AnimationComponent.h"
#include "step_mole_CircleCollisionComponent.h"
#include "step_rain_of_chaos_game_AnimationInfoContainer.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyNode::EnemyNode() :
			mProcessorContainer()
			, mCurrentProcessor()
		{}

		EnemyNode* EnemyNode::create(
			const DebugConfig debug_config
			, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
		)
		{
			auto ret = new ( std::nothrow ) EnemyNode();
			if( !ret || !ret->init( debug_config, circle_collision_component_config ) )
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

		bool EnemyNode::init(
			const DebugConfig debug_config
			, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
		)
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// Pivot
			//
			if( debug_config.bShowPivot )
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 2.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// View
			//
			{
				auto view_node = Sprite::createWithSpriteFrameName( "step_rain_of_chaos_actor_01_idle_0.png" );
				view_node->setScale( _director->getContentScaleFactor() );
				addChild( view_node );
				{
					// Animation Component
					auto animation_component = step_mole::AnimationComponent::create( step_rain_of_chaos::game::GetEnemyAnimationInfoContainer() );
					view_node->addComponent( animation_component );
					animation_component->PlayAnimation( cpg::animation::eIndex::idle );
				}
			}

			//
			// Collision
			//
			{
				// Collision Component
				auto circle_collision_component = step_mole::CircleCollisionComponent::create( 2.f, Vec2::ZERO, circle_collision_component_config );
				addComponent( circle_collision_component );
			}

			return true;
		}

		void EnemyNode::update4Processor( float dt )
		{
			if( mProcessorContainer.end() == mCurrentProcessor )
			{
				unschedule( schedule_selector( EnemyNode::update4Processor ) );
				return;
			}

			if( !( *mCurrentProcessor )->Update( dt ) )
			{
				++mCurrentProcessor;
				if( mProcessorContainer.end() != mCurrentProcessor )
				{
					( *mCurrentProcessor )->Enter();
				}
			}
		}

		void EnemyNode::SetProcessor( EnemyProcessorContainer&& enemy_processor_container )
		{
			mProcessorContainer = std::move( enemy_processor_container );
		}

		void EnemyNode::StartProcess()
		{
			unschedule( schedule_selector( EnemyNode::update4Processor ) );

			mCurrentProcessor = mProcessorContainer.begin();
			( *mCurrentProcessor )->Enter();
			schedule( schedule_selector( EnemyNode::update4Processor ) );
		}
	}
}

