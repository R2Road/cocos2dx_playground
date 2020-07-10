#include "step_rain_of_chaos_game_StageNode.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "ui/UIScale9Sprite.h"

#include "step_mole_animation_InfoContainer.h"
#include "step_mole_AnimationComponent.h"
#include "step_mole_CircleCollisionComponent.h"
#include "step_rain_of_chaos_game_BulletLifeComponent.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		StageNode::StageNode( const StageConfig stage_config, const int bullet_count ) :
			mStageConfig( stage_config )
			, mObjectComponentList( bullet_count, nullptr )
			, mCollisionComponentList( bullet_count, nullptr )
		{}

		StageNode* StageNode::create(
			const StageConfig stage_config, const DebugConfig debug_config
			, const int bullet_count
			, const BulletProcessExitCallback& bullet_process_exit_callback
			, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
		)
		{
			auto ret = new ( std::nothrow ) StageNode( stage_config, bullet_count );
			if( !ret || !ret->init( debug_config, bullet_count, bullet_process_exit_callback, circle_collision_component_config ) )
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

		bool StageNode::init(
			const DebugConfig debug_config
			, const int bullet_count
			, const BulletProcessExitCallback& bullet_process_exit_callback
			, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
		)
		{
			if( !Node::init() )
			{
				return false;
			}

			setContentSize( _director->getVisibleSize() );

			//
			// Pivot
			//
			if( debug_config.bShowPivot )
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 4.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Stage Area Guide
			//
			if( debug_config.bShowAreaGuide )
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
			// Objects
			//
			{
				for( int i = 0; bullet_count > i; ++i )
				{
					auto object_node = MakeBullet(
						i
						, bullet_process_exit_callback
						, circle_collision_component_config
						, debug_config.bShowPivot
					);
					object_node->setPosition( i * 2, 100.f );
					addChild( object_node );

					mObjectComponentList[i] = static_cast<BulletLifeComponent*>( object_node->getComponent( BulletLifeComponent::GetStaticName() ) );
					mCollisionComponentList[i] = static_cast<step_mole::CircleCollisionComponent*>( object_node->getComponent( step_mole::CircleCollisionComponent::GetStaticName() ) );
				}
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
			auto object_node = Node::create();
			object_node->setTag( index );
			{
				// Pivot
				if( bShowPivot )
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 2.f );
					object_node->addChild( pivot, std::numeric_limits<int>::max() );
				}

				// View
				auto view_node = Sprite::createWithSpriteFrameName( "step_mole_target_wait_0.png" );
				view_node->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				view_node->setScale( _director->getContentScaleFactor() );
				view_node->setPositionY( -18.f );
				object_node->addChild( view_node );

				// Animation Component
				auto animation_component = step_mole::AnimationComponent::create( step_mole::animation::GetObjectInfoContainer() );
				view_node->addComponent( animation_component );

				// Collision Component
				auto circle_collision_component = step_mole::CircleCollisionComponent::create( 16.f, Vec2( 0.f, -4.f ), circle_collision_component_config );
				object_node->addComponent( circle_collision_component );

				// Object Component
				object_node->addComponent( BulletLifeComponent::create( mStageConfig.GetBulletLifeArea(), animation_component, circle_collision_component, target_rest_callback ) );

			}

			return object_node;
		}
	}
}

