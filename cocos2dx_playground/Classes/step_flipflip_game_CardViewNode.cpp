#include "step_flipflip_game_CardViewNode.h"

#include <new>

#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		CardViewNode::CardViewNode() : mbFrontSide( false ), mView( nullptr ), mBackSideSpriteFrame( nullptr ), mFrontSideSpriteFrame( nullptr )
		{}

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

			return true;
		}

		void CardViewNode::Flip()
		{
			mbFrontSide = !mbFrontSide;

			if( mbFrontSide )
			{
				mView->setSpriteFrame( mFrontSideSpriteFrame );
			}
			else
			{
				mView->setSpriteFrame( mBackSideSpriteFrame );
			}
		}
	}
}
