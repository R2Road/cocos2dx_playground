#pragma once

#include "cocos/2d/CCNode.h"
#include "cocos/2d/CCComponent.h"
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
		static const char* GetStaticName() { return "CPG_COLLISION"; }
		static CollisionComponent* create( const float radius );

	private:
		bool init() override;
		void onAdd() override;
		void onRemove() override;

	public:
		float GetRadius() const { return mRadius; }

		bool Check( const CollisionComponent* const other ) const;
		void onContact( const bool contact );

	private:
		const float mRadius;
		cocos2d::RefPtr<cocos2d::Node> mLabel;
		cocos2d::RefPtr<cocos2d::Node> mGuide;
		cocos2d::RefPtr<cocos2d::Node> mIndicator;
	};
}
