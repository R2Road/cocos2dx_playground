#include "cpg_AnimationComponent.h"

#include <new>

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCAnimation.h"
#include "cocos/2d/CCSpriteFrameCache.h"

USING_NS_CC;

namespace cpg
{
	AnimationComponent::AnimationComponent() : mAnimationActions(), mAnimationCallback()
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


	AnimationComponent* AnimationComponent::create( const cpg::animation::InfoContainer& animation_info_container )
	{
		auto ret = new ( std::nothrow ) AnimationComponent();
		if( !ret || !ret->init( animation_info_container ) )
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

	void AnimationComponent::PlayAnimation( const cpg::animation::eIndex animation_index )
	{
		auto animation_action = getAnimationAction( animation_index );
		if( !animation_action )
		{
			cocos2d::log( "" );
			return;
		}

		StopAnimation();

		auto repeat_action = RepeatForever::create( animation_action );
		repeat_action->setTag( static_cast<int>( animation_index ) );
		getOwner()->runAction( repeat_action );
	}
	void AnimationComponent::PlayAnimationWithCallback( const cpg::animation::eIndex animation_index, const AnimationCallback animation_callback )
	{
		auto animation_action = getAnimationAction( animation_index );
		if( !animation_action )
		{
			cocos2d::log( "" );
			return;
		}

		StopAnimation();

		mAnimationCallback = animation_callback;

		auto sequence_action = Sequence::create( animation_action, CallFunc::create( std::bind( &AnimationComponent::AnimationEndCallback, this ) ), nullptr );
		sequence_action->setTag( static_cast<int>( animation_index ) );
		getOwner()->runAction( sequence_action );
	}
	void AnimationComponent::StopAnimation()
	{
		mAnimationCallback = nullptr;
		getOwner()->stopAllActions();
	}

	bool AnimationComponent::init( const cpg::animation::InfoContainer& animation_info_container )
	{
		if( !Component::init() )
		{
			return false;
		}

		mAnimationActions.reserve( animation_info_container.Get().size() );
		for( const auto& animation_info : animation_info_container.Get() )
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
	Animate* AnimationComponent::getAnimationAction( const cpg::animation::eIndex animation_index )
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
	void AnimationComponent::AnimationEndCallback()
	{
		if( mAnimationCallback )
		{
			mAnimationCallback();
			mAnimationCallback = nullptr;
		}
	}
}
