#include "step_clickclick_game_EffectViewNode.h"

#include <new>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		EffectViewNode::EffectViewNode() :
			mSpriteNode( nullptr )
			, mAnimationActions()
		{}

		EffectViewNode::~EffectViewNode()
		{
			for( auto& a : mAnimationActions )
			{
				CC_SAFE_RELEASE_NULL( a );
			}
		}

		EffectViewNode* EffectViewNode::create()
		{
			auto ret = new ( std::nothrow ) EffectViewNode();
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

		bool EffectViewNode::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// View
			//
			{
				mSpriteNode = Sprite::create();
				mSpriteNode->setScale( _director->getContentScaleFactor() );
				addChild( mSpriteNode );
			}

			//
			// Action : Increase Animation
			//
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
				animate_action->setTag( eEffectIndex::Increase );
				animate_action->retain();

				mAnimationActions[eEffectIndex::Increase] = animate_action;
			}

			//
			// Action : Decrease Animation
			//
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
				animate_action->setTag( eEffectIndex::Decrease );
				animate_action->retain();

				mAnimationActions[eEffectIndex::Decrease] = animate_action;
			}

			//
			// Action : Die Animation
			//
			{
				auto animation_object = Animation::create();
				animation_object->setDelayPerUnit( 0.07f );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_die1.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_die2.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_die3.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_die4.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "empty_2x2.png" ) );
				
				auto animate_action = Animate::create( animation_object );
				animate_action->setTag( eEffectIndex::Die );
				animate_action->retain();

				mAnimationActions[eEffectIndex::Die] = animate_action;
			}

			return true;
		}

		void EffectViewNode::PlayEffect( const eEffectIndex effect_index )
		{
			for( auto& a : mAnimationActions )
			{
				if( effect_index == a->getTag() )
				{
					mSpriteNode->stopAllActions();
					mSpriteNode->runAction( a );
					break;
				}
			}
		}
	}
}
