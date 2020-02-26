#pragma once

#include "cocos/2d/CCComponent.h"
#include "cocos/2d/CCLabel.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCRefPtr.h"

namespace cpg
{
	class CollisionComponent : public cocos2d::Component
	{
	private:
		using ParentT = cocos2d::Component;

		CollisionComponent( const float radius );
		~CollisionComponent();

	public:
		static CollisionComponent* create( const float radius );

		float GetRadius() const { return mRadius; }

	private:
		bool init() override;
		void onAdd() override;
		void onRemove() override;

	private:
		const float mRadius;
		cocos2d::RefPtr<cocos2d::Label> mLabel;
		cocos2d::RefPtr<cocos2d::Sprite> mIndicator;
	};
}
