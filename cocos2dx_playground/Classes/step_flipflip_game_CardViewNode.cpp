#include "step_flipflip_game_CardViewNode.h"

#include <new>

#include "2d/CCActionInterval.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"

USING_NS_CC;

namespace
{
	const int TAG_Action = 1;
}

namespace step_flipflip
{
	namespace game
	{
		CardViewNode::CardViewNode() :
			mbFrontSide( false )
			, mView( nullptr )
			, mBackSideSpriteFrame( nullptr )
			, mFrontSideSpriteFrame( nullptr )
			, mAction4FrontSide( nullptr )
			, mAction4BackSide( nullptr )
		{}

		CardViewNode::~CardViewNode()
		{
			mAction4FrontSide->release();
			mAction4BackSide->release();
		}

		CardViewNode* CardViewNode::create( const eCardType card_type )
		{
			auto ret = new ( std::nothrow ) CardViewNode();
			if( !ret || !ret->init( card_type ) )
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

		bool CardViewNode::init( const eCardType card_type )
		{
			if( !Node::init() )
			{
				return false;
			}

			mBackSideSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName( GetSpriteFrameName_CardBackSide() );
			mFrontSideSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName( GetSpriteFrameName_CardFrontSide( card_type ) );

			mView = Sprite::createWithSpriteFrame( mBackSideSpriteFrame );
			addChild( mView );

			//
			// Flip Action - Go Front Side
			//
			{
				auto scale_to_1 = ScaleTo::create( 0.1f, 0.f, 1.f );

				auto animation_object = Animation::create();
				animation_object->setDelayPerUnit( 0.01f );
				animation_object->addSpriteFrame( mFrontSideSpriteFrame );
				auto animate = Animate::create( animation_object );

				auto scale_to_2 = ScaleTo::create( 0.1f, 1.f, 1.f );

				auto sequence = Sequence::create( scale_to_1, animate, scale_to_2, nullptr );
				sequence->retain();

				mAction4FrontSide = sequence;
				mAction4FrontSide->setTag( TAG_Action );
			}

			//
			// Flip Action - Go Back Side
			//
			{
				auto scale_to_1 = ScaleTo::create( 0.1f, 0.f, 1.f );

				auto animation_object = Animation::create();
				animation_object->setDelayPerUnit( 0.01f );
				animation_object->addSpriteFrame( mBackSideSpriteFrame );
				auto animate = Animate::create( animation_object );

				auto scale_to_2 = ScaleTo::create( 0.1f, 1.f, 1.f );

				auto sequence = Sequence::create( scale_to_1, animate, scale_to_2, nullptr );
				sequence->retain();

				mAction4BackSide = sequence;
				mAction4FrontSide->setTag( TAG_Action );
			}

			return true;
		}

		void CardViewNode::Flip()
		{
			if( mView->getActionByTag( TAG_Action ) )
			{
				return;
			}

			mbFrontSide = !mbFrontSide;

			if( mbFrontSide )
			{
				mView->runAction( mAction4FrontSide );
			}
			else
			{
				mView->runAction( mAction4BackSide );
			}
		}
	}
}
