#include "cpg_SpriteFrameAction.h"

#include <new>

#include "2d/CCSprite.h"

USING_NS_CC;

namespace cpg
{
	SpriteFrameAction::SpriteFrameAction( SpriteFrame* sprite_frame ) :
		mSpriteFrame( sprite_frame )
		, mbFirstFrame( false )
	{}

	SpriteFrameAction* SpriteFrameAction::create( float duration, SpriteFrame* sprite_frame )
	{
		auto ret = new ( std::nothrow ) SpriteFrameAction( sprite_frame );

		if( ret && ret->initWithDuration( duration ) )
		{
			ret->autorelease();
		}
		else
		{
			delete ret;
			ret = nullptr;
		}
		
		return ret;
	}

	SpriteFrameAction* SpriteFrameAction::clone() const
	{
		// no copy constructor
		return SpriteFrameAction::create( _duration, mSpriteFrame );
	}
	SpriteFrameAction* SpriteFrameAction::reverse() const
	{
		CCASSERT( false, "reverse() not supported in SpriteFrameAction" );
		return nullptr;
	}
	void SpriteFrameAction::startWithTarget( Node *target )
	{
		ActionInterval::startWithTarget( target );
		mbFirstFrame = true;
	}
	void SpriteFrameAction::update( float /*time*/ )
	{
		if( mbFirstFrame )
		{
			mbFirstFrame = false;

			auto blend = static_cast<Sprite*>( _target )->getBlendFunc();
			static_cast<Sprite*>( _target )->setSpriteFrame( mSpriteFrame );
			static_cast<Sprite*>( _target )->setBlendFunc( blend );

			CCLOG( "SpriteFrameAction : Do" );
		}
	}
}