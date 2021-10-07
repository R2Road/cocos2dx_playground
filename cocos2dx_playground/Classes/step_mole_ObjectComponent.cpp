#include "step_mole_ObjectComponent.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/ccMacros.h"

#include "step_mole_AnimationComponent.h"

USING_NS_CC;

namespace step_mole
{
	ObjectComponent::ObjectComponent(
		AnimationComponent* const animation_component
		, Component* const circle_collision_component
		, const ProcessExitCallback& process_end_callback
	) :
		mLastState( eState::Damaged_2 )
		, mAnimationComponent( animation_component )
		, mCircleCollisionComponent( circle_collision_component )
		, mProcessEndCallback( process_end_callback )

		, mActionTime( 0.f )
	{
		setName( GetStaticName() );
	}

	ObjectComponent* ObjectComponent::create(
		AnimationComponent* const animation_component
		, Component* const circle_collision_component
		, const ProcessExitCallback& process_end_callback
	)
	{
		auto ret = new ( std::nothrow ) ObjectComponent( animation_component, circle_collision_component, process_end_callback );
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

	bool ObjectComponent::init()
	{
		if( !ParentT::init() )
		{
			return false;
		}

		mCircleCollisionComponent->setEnabled( false );

		return true;
	}
	void ObjectComponent::onAdd()
	{
		ParentT::onAdd();

		std::random_device rd;
		std::mt19937 randomEngine( rd() );
		std::uniform_real_distribution<> dist( 0, 0.5f );

		_owner->scheduleOnce(
			[this]( float )
			{
				ChangeState( eState::Wait );
			}
			, dist( randomEngine )
			, "change_state"
		);
	}

	void ObjectComponent::ProcessStart( const float action_time )
	{
		mActionTime = action_time;
		ChangeState( eState::Wakeup );
	}
	void ObjectComponent::ProcessDamage()
	{
		ChangeState( eState::Damaged_1 );
	}

	void ObjectComponent::ChangeState( const eState next_state )
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
		case eState::Wait:
		{
			mCircleCollisionComponent->setEnabled( false );
			mAnimationComponent->PlayAnimation( cpg_animation::eIndex::wait );
		}
		break;

		case eState::Wakeup:
		{
			mCircleCollisionComponent->setEnabled( true );
			mAnimationComponent->PlayAnimationWithCallback( cpg_animation::eIndex::wakeup, std::bind( &ObjectComponent::ChangeState, this, eState::Action ) );
		}
		break;

		case eState::Action:
		{
			mAnimationComponent->PlayAnimation( cpg_animation::eIndex::idle );
			_owner->scheduleOnce(
				[this]( float )
				{
					ChangeState( eState::Sleep );
				}
				, mActionTime
				, "change_state"
			);
		}
		break;

		case eState::Sleep:
		{
			mAnimationComponent->PlayAnimationWithCallback( cpg_animation::eIndex::sleep, std::bind( &ObjectComponent::ChangeState, this, eState::Exit ) );
		}
		break;

		case eState::Damaged_1:
		{
			_owner->unschedule( "change_state" );

			mCircleCollisionComponent->setEnabled( false );
			mAnimationComponent->PlayAnimationWithCallback( cpg_animation::eIndex::damaged_1, std::bind( &ObjectComponent::ChangeState, this, eState::Damaged_2 ) );
		}
		break;
		case eState::Damaged_2:
		{
			mAnimationComponent->PlayAnimationWithCallback( cpg_animation::eIndex::damaged_2, std::bind( &ObjectComponent::ChangeState, this, eState::Exit ) );
		}
		break;

		case eState::Exit:
		{
			ChangeState( eState::Wait );
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
