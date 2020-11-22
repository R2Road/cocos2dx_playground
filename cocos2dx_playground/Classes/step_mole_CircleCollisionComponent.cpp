#include "step_mole_CircleCollisionComponent.h"

#include <new>
#include <numeric>

#include "cocos/2d/CCLabel.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/ccUTF8.h"

#include "cpg_StringTable.h"

#include "step_mole_CircleCollisionComponentConfig.h"

USING_NS_CC;

namespace step_mole
{
	CircleCollisionComponent::CircleCollisionComponent( const float radius, const cocos2d::Vec2 offset ) :
		mRadius( radius )
		, mOffset( offset )
		, mHelper()
	{
		setName( GetStaticName() );
	}
	CircleCollisionComponent::~CircleCollisionComponent()
	{
		for( auto h : mHelper )
		{
			if( !h.first )
			{
				continue;
			}

			h.first->release();
			h.first.reset();
		}
	}

	CircleCollisionComponent* CircleCollisionComponent::create( const float radius, const cocos2d::Vec2 offset, const CircleCollisionComponentConfig& config )
	{
		auto ret = new ( std::nothrow ) CircleCollisionComponent( radius, offset );
		if( !ret || !ret->init( config ) )
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

	bool CircleCollisionComponent::init( const CircleCollisionComponentConfig& config )
	{
		if( !ParentT::init() )
		{
			return false;
		}

		const float margin = 3.f;

		// Radius View
		if( config.bUseRadiusHelper )
		{
			auto label = Label::createWithTTF( StringUtils::format( "%.2f", mRadius ), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
			label->setPositionX( mRadius + margin );
			label->setVisible( false );
			label->retain();

			auto& target_node = mHelper[static_cast<std::size_t>( eHelperNode::radius_view )];
			target_node.first = label;
			target_node.second = std::numeric_limits<int>::max();
		}

		// Collision Guide
		if( config.bUseGuideHelper )
		{
			auto guide = Sprite::createWithSpriteFrameName( "guide_02_4.png" );
			guide->setScale( mRadius / ( guide->getContentSize().width * 0.5f ) );
			guide->setPosition( mOffset );
			guide->setVisible( false );
			guide->retain();

			auto& target_node = mHelper[static_cast<std::size_t>( eHelperNode::guide )];
			target_node.first = guide;
			target_node.second = std::numeric_limits<int>::max() - 2;
		}

		// Collision Indicator
		if( config.bUseIndicatorHelper )
		{
			auto indicator = Sprite::createWithSpriteFrameName( "guide_02_7.png" );
			indicator->setScale( mRadius / ( indicator->getContentSize().width * 0.5f ) );
			indicator->setPosition( mOffset );
			indicator->setVisible( false );
			indicator->retain();

			auto& target_node = mHelper[static_cast<std::size_t>( eHelperNode::indicator )];
			target_node.first = indicator;
			target_node.second = std::numeric_limits<int>::max() - 1;
		}

		setEnabled( true );

		return true;
	}

	void CircleCollisionComponent::setEnabled( bool enabled )
	{
		if( enabled )
		{
			for( std::size_t i = static_cast<int>( eHelperNode::FIRST ), end = static_cast<int>( eHelperNode::SIZE ); i < end; ++i )
			{
				auto& h = mHelper[i];
				if( h.first )
				{
					if( static_cast<std::size_t>( eHelperNode::indicator ) == i )
					{
						h.first->setVisible( false );
					}
					else
					{
						h.first->setVisible( true );
					}
				}
			}
		}
		else
		{
			for( auto h : mHelper )
			{
				if( h.first )
				{
					h.first->setVisible( enabled );
				}
			}
		}

		Component::setEnabled( enabled );
	}

	void CircleCollisionComponent::onAdd()
	{
		for( std::size_t i = static_cast<int>( eHelperNode::FIRST ), end = static_cast<int>( eHelperNode::SIZE ); i < end; ++i )
		{
			auto& helper_node = mHelper[i];
			if( !helper_node.first )
			{
				continue;
			}

			_owner->addChild( helper_node.first, helper_node.second );
		}

		ParentT::onAdd();
	}
	void CircleCollisionComponent::onRemove()
	{
		ParentT::onRemove();

		for( auto h : mHelper )
		{
			if( !h.first )
			{
				continue;
			}

			_owner->removeChild( h.first );
		}
	}

	bool CircleCollisionComponent::Check( const CircleCollisionComponent* const other ) const
	{
		if( !isEnabled() || !other->isEnabled() )
		{
			return false;
		}

		const float distance = GetFixedPosition().distance( other->GetFixedPosition() );
		const float contact_limit_distance = mRadius + other->GetRadius();
		
		return distance <= contact_limit_distance;
	}
	void CircleCollisionComponent::onContact( const bool contact )
	{
		auto& indicator = mHelper[static_cast<std::size_t>( eHelperNode::indicator )];
		if( indicator.first )
		{
			indicator.first->setVisible( contact );
		}
	}
}
