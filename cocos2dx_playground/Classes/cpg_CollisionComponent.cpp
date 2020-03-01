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
		, mHelper()
	{
		setName( GetStaticName() );
	}
	CollisionComponent::~CollisionComponent()
	{
		for( auto h : mHelper )
		{
			h.first->release();
			h.first.reset();
		}
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
		{
			auto label = Label::createWithTTF( StringUtils::format( "%.2f", mRadius ), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
			label->setPositionX( mRadius + margin );
			label->retain();

			auto& target_node = mHelper[static_cast<std::size_t>( eHelperNode::radius_view )];
			target_node.first = label;
			target_node.second = std::numeric_limits<int>::max();
		}

		// Collision Guide
		{
			auto guide = Sprite::createWithSpriteFrameName( "guide_02_4.png" );
			guide->setScale( mRadius / ( guide->getContentSize().width * 0.5f ) );
			guide->retain();

			auto& target_node = mHelper[static_cast<std::size_t>( eHelperNode::guide )];
			target_node.first = guide;
			target_node.second = std::numeric_limits<int>::max() - 2;
		}

		// Collision Indicator
		{
			auto indicator = Sprite::createWithSpriteFrameName( "guide_02_7.png" );
			indicator->setScale( mRadius / ( indicator->getContentSize().width * 0.5f ) );
			indicator->setVisible( false );
			indicator->retain();

			auto& target_node = mHelper[static_cast<std::size_t>( eHelperNode::indicator )];
			target_node.first = indicator;
			target_node.second = std::numeric_limits<int>::max() - 1;
		}
		

		return true;
	}

	void CollisionComponent::onAdd()
	{
		for( std::size_t i = static_cast<int>( eHelperNode::FIRST ), end = static_cast<int>( eHelperNode::SIZE ); i < end; ++i )
		{
			auto& helper_node = mHelper[i];
			_owner->addChild( helper_node.first, helper_node.second );
		}

		ParentT::onAdd();
	}
	void CollisionComponent::onRemove()
	{
		ParentT::onRemove();

		for( auto h : mHelper )
		{
			_owner->removeChild( h.first );
		}
	}

	bool CollisionComponent::Check( const CollisionComponent* const other ) const
	{
		const float distance = _owner->getPosition().distance( other->getOwner()->getPosition() );
		const float contact_limit_distance = mRadius + other->GetRadius();
		
		return distance <= contact_limit_distance;
	}
	void CollisionComponent::onContact( const bool contact )
	{
		auto& indicator = mHelper[static_cast<std::size_t>( eHelperNode::indicator )];
		indicator.first->setVisible( contact );
	}
}
