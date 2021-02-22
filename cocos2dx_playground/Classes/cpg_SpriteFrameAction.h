#pragma once

#include <string>

#include "2d/CCActionInterval.h"

NS_CC_BEGIN
	class SpriteFrame;
NS_CC_END

namespace cpg
{
	class SpriteFrameAction : public cocos2d::ActionInterval
	{
	private:
		SpriteFrameAction( cocos2d::SpriteFrame* sprite_frame );

		//
		// Non-Copyable
		//
		SpriteFrameAction( const SpriteFrameAction& ) = delete;
		SpriteFrameAction( SpriteFrameAction&& ) = delete;

		//
		// Non-Assignable
		//
		SpriteFrameAction& operator =( const SpriteFrameAction& ) = delete;
		SpriteFrameAction& operator =( SpriteFrameAction&& ) = delete;

	public:
		static SpriteFrameAction* create( float duration, cocos2d::SpriteFrame* sprite_frame );

		//
		// Overrides
		//
		SpriteFrameAction* clone() const override;
		SpriteFrameAction* reverse() const  override;

		void startWithTarget( cocos2d::Node *target ) override;
		void update( float /*time*/ ) override;

	private:
		cocos2d::SpriteFrame* mSpriteFrame;
		bool mbFirstFrame;
	};
}
