#include "cpg_CollisionComponent.h"

#include <new>
#include <numeric>

#include "cocos/base/ccUTF8.h"

USING_NS_CC;

namespace cpg
{
	CollisionComponent::CollisionComponent( const float radius ) :
		mRadius( radius )
		, mLabel( nullptr )
		, mIndicator( nullptr )
	{
		setName( "CPG_COLLISION" );
	}
	CollisionComponent::~CollisionComponent()
	{
		mLabel->release();
		mIndicator->release();
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

		// Collision Indicator
		auto indicator_node = Sprite::createWithSpriteFrameName( "guide_02_7.png" );
		indicator_node->setScale( mRadius / ( indicator_node->getContentSize().width * 0.5f ) );
		indicator_node->setVisible( true );
		indicator_node->retain();
		mIndicator = indicator_node;
		

		return true;
	}

	void CollisionComponent::onAdd()
	{
		_owner->addChild( mLabel, std::numeric_limits<int>::max() );
		_owner->addChild( mIndicator, std::numeric_limits<int>::max() - 1 );
	}
	void CollisionComponent::onRemove()
	{
		_owner->removeChild( mLabel );
		_owner->removeChild( mIndicator );
	}
}
