#include "step_mole_ObjectComponent.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/ccMacros.h"

#include "cpg_AnimationComponent.h"

USING_NS_CC;

namespace step_mole
{
	ObjectComponent::ObjectComponent(
		cpg::AnimationComponent* const animation_component
		, cocos2d::Component* const circle_collision_component
	) :
		mLastState( eState::Hide )
		, mAnimationComponent( animation_component )
		, mCircleCollisionComponent( circle_collision_component )
	{
		setName( GetStaticName() );
	}

	ObjectComponent* ObjectComponent::create(
		cpg::AnimationComponent* const animation_component
		, cocos2d::Component* const circle_collision_component
	)
	{
		auto ret = new ( std::nothrow ) ObjectComponent( animation_component, circle_collision_component );
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

	bool ObjectComponent::init()
	{
		if( !ParentT::init() )
		{
			return false;
		}

		return true;
	}

	void ObjectComponent::ProcessStart()
	{
		_owner->setVisible( true );
		ChangeState( eState::Wakeup );
	}
	void ObjectComponent::ProcessDamage()
	{
		ChangeState( eState::Damaged );
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
		case eState::Wakeup:
		{
			mCircleCollisionComponent->setEnabled( true );
			mAnimationComponent->PlayAnimationWithCallback( cpg::animation::eIndex::run, std::bind( &ObjectComponent::ChangeState, this, eState::Action ) );
		}
		break;

		case eState::Action:
		{
			mAnimationComponent->PlayAnimationWithCallback( cpg::animation::eIndex::win, std::bind( &ObjectComponent::ChangeState, this, eState::Sleep ) );
		}
		break;

		case eState::Sleep:
		{
			mAnimationComponent->PlayAnimationWithCallback( cpg::animation::eIndex::run, std::bind( &ObjectComponent::ChangeState, this, eState::Hide ) );
		}
		break;

		case eState::Damaged:
		{
			mCircleCollisionComponent->setEnabled( false );
			mAnimationComponent->PlayAnimationWithCallback( cpg::animation::eIndex::idle, std::bind( &ObjectComponent::ChangeState, this, eState::Hide ) );
		}
		break;

		case eState::Hide:
		{
			mAnimationComponent->StopAnimation();
			_owner->setVisible( false );
		}
		break;

		default:
			CCASSERT( false, "Invalid Next State" );
		}

		mLastState = next_state;
	}
}
