#include "step_rain_of_chaos_game_BulletLifeComponent.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/ccMacros.h"

#include "step_mole_AnimationComponent.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		BulletLifeComponent::BulletLifeComponent(
			const cocos2d::Rect allowed_area
			, step_mole::AnimationComponent* const animation_component
			, Component* const circle_collision_component
			, const ProcessExitCallback& process_end_callback
		) :
			mAllowedArea( allowed_area )

			, mLastState( eState::Boom_2 )
			, mMoveDirection( Vec2::ONE )

			, mAnimationComponent( animation_component )
			, mCircleCollisionComponent( circle_collision_component )
			, mProcessEndCallback( process_end_callback )
		{
			setName( GetStaticName() );
		}

		BulletLifeComponent* BulletLifeComponent::create(
			const cocos2d::Rect allowed_area
			, step_mole::AnimationComponent* const animation_component
			, Component* const circle_collision_component
			, const ProcessExitCallback& process_end_callback
		)
		{
			auto ret = new ( std::nothrow ) BulletLifeComponent( allowed_area, animation_component, circle_collision_component, process_end_callback );
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

		bool BulletLifeComponent::init()
		{
			if( !ParentT::init() )
			{
				return false;
			}

			mCircleCollisionComponent->setEnabled( false );

			return true;
		}

		void BulletLifeComponent::ProcessStart( const cocos2d::Vec2 new_position, const cocos2d::Vec2 move_direction )
		{
			_owner->unschedule( "Move" );
			_owner->setPosition( new_position );
			_owner->setVisible( true );
			mMoveDirection = move_direction;
			ChangeState( eState::Wakeup );
		}
		void BulletLifeComponent::ProcessBoom()
		{
			ChangeState( eState::Boom_1 );
		}

		void BulletLifeComponent::ChangeState( const eState next_state )
		{
			if( !_owner->isVisible() )
			{
				return;
			}

			if( next_state == mLastState )
			{
				return;
			}

			switch( next_state )
			{
			case eState::Sleep:
			{
				mAnimationComponent->StopAnimation();
				_owner->setVisible( false );
			}
			break;

			case eState::Wakeup:
			{
				mCircleCollisionComponent->setEnabled( true );
				mAnimationComponent->PlayAnimationWithCallback( cpg_animation::eIndex::wakeup, std::bind( &BulletLifeComponent::ChangeState, this, eState::Move ) );
			}
			break;

			case eState::Move:
			{
				mAnimationComponent->PlayAnimation( cpg_animation::eIndex::idle );
				_owner->schedule(
					[this]( float delta_time )
					{
						_owner->setPosition( _owner->getPosition() + ( mMoveDirection * delta_time ) );
						if( !mAllowedArea.containsPoint( _owner->getPosition() ) )
						{
							ChangeState( eState::Disappear );
						}
					}
					, "Move"
				);
			}
			break;

			case eState::Disappear:
			{
				_owner->unschedule( "Move" );

				mCircleCollisionComponent->setEnabled( false );
				mAnimationComponent->PlayAnimationWithCallback( cpg_animation::eIndex::sleep, std::bind( &BulletLifeComponent::ChangeState, this, eState::Exit ) );
			}
			break;

			case eState::Boom_1:
			{
				_owner->unschedule( "Move" );

				mCircleCollisionComponent->setEnabled( false );
				mAnimationComponent->PlayAnimationWithCallback( cpg_animation::eIndex::damaged_1, std::bind( &BulletLifeComponent::ChangeState, this, eState::Boom_2 ) );
			}
			break;
			case eState::Boom_2:
			{
				mAnimationComponent->PlayAnimationWithCallback( cpg_animation::eIndex::damaged_2, std::bind( &BulletLifeComponent::ChangeState, this, eState::Exit ) );
			}
			break;

			case eState::Exit:
			{
				ChangeState( eState::Sleep );
				if( mProcessEndCallback )
				{
					mProcessEndCallback( _owner->getTag() );
				}
			}
			break;

			default:
				CCASSERT( false, "Invalid Next State" );
			}

			mLastState = next_state;
		}
	}
}
