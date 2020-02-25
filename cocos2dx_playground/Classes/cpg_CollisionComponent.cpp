#include "cpg_CollisionComponent.h"

#include <new>
#include <numeric>

#include "cocos/base/ccUTF8.h"

USING_NS_CC;

namespace cpg
{
	CollisionComponent::CollisionComponent( const float radius ) : mRadius( radius ), mLabel( nullptr )
	{
		setName( "CPG_COLLISION" );
	}
	CollisionComponent::~CollisionComponent()
	{
		mLabel->release();
	}

	CollisionComponent* CollisionComponent::create( const float radius )
	{
		auto ret = new ( std::nothrow ) CollisionComponent( radius );
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
			return nullptr;
		}
		else
		{
			ret->autorelease();
		}

		return ret;
	}

	bool CollisionComponent::init()
	{
		if( !ParentT::init() )
		{
			return false;
		}

		mLabel = Label::createWithTTF( StringUtils::format( "%.2f", mRadius ), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
		mLabel->setAnchorPoint( Vec2( 0.f, 0.5f ) );
		mLabel->setPositionX( mRadius );
		mLabel->retain();

		return true;
	}

	void CollisionComponent::onAdd()
	{
		_owner->addChild( mLabel, std::numeric_limits<int>::max() );
	}
	void CollisionComponent::onRemove()
	{
		_owner->removeChild( mLabel );
	}
}
