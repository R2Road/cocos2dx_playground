#include "cpg_CollisionComponent.h"

#include <new>
#include <numeric>

#include "cocos/2d/CCLabel.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/ccUTF8.h"

USING_NS_CC;

namespace cpg
{
	CollisionComponent::CollisionComponent( const float radius ) :
		mRadius( radius )
		, mLabel( nullptr )
		, mGuide( nullptr )
		, mIndicator( nullptr )
	{
		setName( GetStaticName() );
	}
	CollisionComponent::~CollisionComponent()
	{
		mLabel->release();
		mGuide->release();
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

		const float margin = 3.f;

		// Radius View
		mLabel = Label::createWithTTF( StringUtils::format( "%.2f", mRadius ), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
		mLabel->setAnchorPoint( Vec2( 0.f, 0.5f ) );
		mLabel->setPositionX( mRadius + margin );
		mLabel->retain();

		// Collision Indicator
		auto guide_node = Sprite::createWithSpriteFrameName( "guide_02_4.png" );
		guide_node->setScale( mRadius / ( guide_node->getContentSize().width * 0.5f ) );
		guide_node->retain();
		mGuide = guide_node;

		// Collision Indicator
		auto indicator_node = Sprite::createWithSpriteFrameName( "guide_02_7.png" );
		indicator_node->setScale( mRadius / ( indicator_node->getContentSize().width * 0.5f ) );
		indicator_node->setVisible( false );
		indicator_node->retain();
		mIndicator = indicator_node;
		

		return true;
	}

	void CollisionComponent::onAdd()
	{
		_owner->addChild( mLabel, std::numeric_limits<int>::max() );
		_owner->addChild( mGuide, std::numeric_limits<int>::max() - 2 );
		_owner->addChild( mIndicator, std::numeric_limits<int>::max() - 1 );

		ParentT::onAdd();
	}
	void CollisionComponent::onRemove()
	{
		ParentT::onRemove();

		_owner->removeChild( mLabel );
		_owner->removeChild( mGuide );
		_owner->removeChild( mIndicator );
	}

	bool CollisionComponent::Check( const CollisionComponent* const other ) const
	{
		const float distance = _owner->getPosition().distance( other->getOwner()->getPosition() );
		const float contact_limit_distance = mRadius + other->GetRadius();
		
		return distance <= contact_limit_distance;
	}
	void CollisionComponent::onContact( const bool contact )
	{
		mIndicator->setVisible( contact );
	}
}
