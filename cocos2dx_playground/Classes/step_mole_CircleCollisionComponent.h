#pragma once

#include "cocos/2d/CCNode.h"
#include "cocos/2d/CCComponent.h"
#include "cocos/base/CCRefPtr.h"

namespace step_mole
{
	struct CircleCollisionComponentConfig;

	class CircleCollisionComponent : public cocos2d::Component
	{
	private:
		using ParentT = cocos2d::Component;

		CircleCollisionComponent( const float radius );
		~CircleCollisionComponent();

	public:
		static const char* GetStaticName() { return "MOLE_CIRCLECOLLISION"; }
		static CircleCollisionComponent* create( const float radiuss, const CircleCollisionComponentConfig& config );

		void setEnabled( bool enabled ) override;

	private:
		bool init( const CircleCollisionComponentConfig& config );
		void onAdd() override;
		void onRemove() override;

	public:
		float GetRadius() const { return mRadius; }

		bool Check( const CircleCollisionComponent* const other ) const;
		void onContact( const bool contact );

	private:
		enum class eHelperNode
		{
			radius_view,
			guide,
			indicator,

			SIZE,
			FIRST = radius_view,
		};

		const float mRadius;
		std::pair<cocos2d::RefPtr<cocos2d::Node>, int> mHelper[static_cast<std::size_t>( eHelperNode::SIZE )];
	};
}
