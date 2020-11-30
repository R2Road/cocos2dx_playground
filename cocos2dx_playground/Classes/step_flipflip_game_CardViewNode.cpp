#include "step_flipflip_game_CardViewNode.h"

#include <new>

#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"

USING_NS_CC;

namespace
{
	const char* CardBackSideSpriteFrameName = "step_flipflip_card_back_0.png";

	const char* CardFrontSpriteFrameNames[static_cast<std::size_t>( step_flipflip::game::CardViewNode::eCardType::SIZE )] = {
		"step_flipflip_card_front_0.png"
		, "step_flipflip_card_front_1.png"
		, "step_flipflip_card_front_2.png"
		, "step_flipflip_card_front_3.png"
		, "step_flipflip_card_front_4.png"
	};
}

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

			mBackSideSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName( CardBackSideSpriteFrameName );
			mFrontSideSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName( CardFrontSpriteFrameNames[static_cast<std::size_t>( card_type )] );

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
