#include "step_clickclick_game_EffectView.h"

#include <new>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/ccMacros.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		EffectView::EffectView() :
			mEffectNode( nullptr )
			, mAnimationActions()
		{}

		EffectView::~EffectView()
		{
			for( auto& a : mAnimationActions )
			{
				CC_SAFE_RELEASE_NULL( a );
			}
		}

		EffectView* EffectView::create()
		{
			auto ret = new ( std::nothrow ) EffectView();
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

		bool EffectView::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// View
			//
			mEffectNode = Sprite::create();
			mEffectNode->setScale( _director->getContentScaleFactor() );
			addChild( mEffectNode );

			// increase animation
			{
				auto animation_object = Animation::create();
				animation_object->setDelayPerUnit( 0.07f );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_increase1.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_increase2.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_increase3.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_increase4.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_increase5.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "empty_2x2.png" ) );

				auto animate_action = Animate::create( animation_object );
				animate_action->setTag( eAnimationIndex::Increase );
				animate_action->retain();

				mAnimationActions[eAnimationIndex::Increase] = animate_action;
			}

			// decrease animation
			{
				auto animation_object = Animation::create();
				animation_object->setDelayPerUnit( 0.07f );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_decrease1.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_decrease2.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_decrease3.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_decrease4.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_decrease5.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "empty_2x2.png" ) );
				
				auto animate_action = Animate::create( animation_object );
				animate_action->setTag( eAnimationIndex::Decrease );
				animate_action->retain();

				mAnimationActions[eAnimationIndex::Decrease] = animate_action;
			}

			// die animation
			{
				auto animation_object = Animation::create();
				animation_object->setDelayPerUnit( 0.07f );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_die1.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_die2.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_die3.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_die4.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "empty_2x2.png" ) );
				
				auto animate_action = Animate::create( animation_object );
				animate_action->setTag( eAnimationIndex::Die );
				animate_action->retain();

				mAnimationActions[eAnimationIndex::Die] = animate_action;
			}

			return true;
		}

		void EffectView::PlayEffect( const eAnimationIndex animation_index )
		{
			for( auto& a : mAnimationActions )
			{
				if( animation_index == a->getTag() )
				{
					mEffectNode->stopAllActions();
					mEffectNode->runAction( a );
					break;
				}
			}
		}
	}
}
