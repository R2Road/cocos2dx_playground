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
	ObjectComponent::ObjectComponent( cpg::AnimationComponent* const animation_component ) :
		mLastState( eState::Wakeup )
		, mAnimationComponent( animation_component )
	{
		setName( GetStaticName() );
	}

	ObjectComponent* ObjectComponent::create( cpg::AnimationComponent* const animation_component )
	{
		auto ret = new ( std::nothrow ) ObjectComponent( animation_component );
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
	void ObjectComponent::setEnabled( bool enabled )
	{
		if( enabled )
		{
			ChangeState( eState::Wakeup );
		}
		else
		{
			mAnimationComponent->StopAnimation();
		}

		_owner->setVisible( enabled );
		ParentT::setEnabled( enabled );
	}

	void ObjectComponent::ChangeState( const eState next_state )
	{
		switch( next_state )
		{
		case eState::Wakeup:
		{
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
			mAnimationComponent->PlayAnimationWithCallback( cpg::animation::eIndex::idle, std::bind( &ObjectComponent::ChangeState, this, eState::Hide ) );
		}
		break;

		case eState::Damaged:
		{
			mAnimationComponent->PlayAnimationWithCallback( cpg::animation::eIndex::idle, std::bind( &ObjectComponent::ChangeState, this, eState::Hide ) );
		}
		break;

		case eState::Hide:
		{
			setEnabled( false );
		}
		break;

		default:
			CCASSERT( false, "Invalid Next State" );
		}

		mLastState = next_state;
	}
}
