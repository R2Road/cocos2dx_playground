#include "step_mole_AnimationComponent.h"

#include <new>

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCAnimation.h"
#include "cocos/2d/CCSpriteFrameCache.h"

USING_NS_CC;

namespace step_mole
{
	AnimationComponent::AnimationComponent() : mAnimationActions()
	{
		setName( GetStaticName() );
	}
	AnimationComponent::~AnimationComponent()
	{
		for( auto a : mAnimationActions )
		{
			a.second->release();
		}
		mAnimationActions.clear();
	}


	AnimationComponent* AnimationComponent::create( const cpg_animation::InfoContainerT& animation_info_container )
	{
		auto ret = new ( std::nothrow ) AnimationComponent();
		if( !ret || !ret->init( animation_info_container ) )
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

	void AnimationComponent::PlayAnimation( const cpg_animation::eIndex animation_index )
	{
		auto animation_action = getAnimationAction( animation_index );
		if( !animation_action )
		{
			CCLOG( "Failed : Animation Index Not Found %d : ", static_cast<int>( animation_index ) );
			return;
		}

		StopAnimation();

		auto repeat_action = RepeatForever::create( animation_action );
		repeat_action->setTag( static_cast<int>( animation_index ) );
		getOwner()->runAction( repeat_action );
	}
	void AnimationComponent::PlayAnimationWithCallback( const cpg_animation::eIndex animation_index, const AnimationCallback animation_callback )
	{
		auto animation_action = getAnimationAction( animation_index );
		if( !animation_action )
		{
			CCLOG( "Failed : Animation Index Not Found %d : ", static_cast<int>( animation_index ) );
			return;
		}

		StopAnimation();

		auto sequence_action = Sequence::create( animation_action, CallFunc::create( animation_callback ), nullptr );
		sequence_action->setTag( static_cast<int>( animation_index ) );
		getOwner()->runAction( sequence_action );
	}
	void AnimationComponent::StopAnimation()
	{
		getOwner()->stopAllActions();
	}

	bool AnimationComponent::init( const cpg_animation::InfoContainerT& animation_info_container )
	{
		if( !Component::init() )
		{
			return false;
		}

		mAnimationActions.reserve( animation_info_container.size() );
		for( const auto& animation_info : animation_info_container )
		{
			auto animation_object = Animation::create();
			animation_object->setDelayPerUnit( animation_info.delay );
			for( const auto& sprite_frame_name : animation_info.SpriteFrameNames )
			{
				animation_object->addSpriteFrame( cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName( sprite_frame_name ) );
			}

			auto animate_action = Animate::create( animation_object );			
			animate_action->retain();

			mAnimationActions.emplace_back( animation_info.Index, animate_action );
		}

		return true;
	}
	Animate* AnimationComponent::getAnimationAction( const cpg_animation::eIndex animation_index )
	{
		for( auto a : mAnimationActions )
		{
			if( animation_index == a.first )
			{
				return a.second;
			}
		}

		return nullptr;
	}
}
