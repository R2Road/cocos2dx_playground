#include "step_rain_of_chaos_game_PlayerNode.h"

#include <new>
#include <numeric>

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
		PlayerNode::PlayerNode() {}

		PlayerNode* PlayerNode::create(
			const float radius
			, const DebugConfig debug_config
			, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
		)
		{
			auto ret = new ( std::nothrow ) PlayerNode();
			if( !ret || !ret->init( radius, debug_config, circle_collision_component_config ) )
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

		bool PlayerNode::init(
			const float radius
			, const DebugConfig debug_config
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
				pivot->setScale( _director->getContentScaleFactor() );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// View
			//
			{
				const auto& animation_info_container = step_rain_of_chaos::game::GetActorAnimationInfoContainer();

				auto view_node = Sprite::createWithSpriteFrameName( animation_info_container[0].SpriteFrameNames[0] );
				view_node->setScale( _director->getContentScaleFactor() );
				addChild( view_node );
				{
					// Animation Component
					auto animation_component = step_mole::AnimationComponent::create( animation_info_container );
					view_node->addComponent( animation_component );
					animation_component->PlayAnimation( cpg_animation::eIndex::idle );
				}
			}

			//
			// Collision
			//
			{
				// Collision Component
				auto circle_collision_component = step_mole::CircleCollisionComponent::create( radius, Vec2::ZERO, circle_collision_component_config );
				addComponent( circle_collision_component );
			}

			return true;
		}
	}
}

