#include "algorithm_practice_astar_CostNode.h"

#include <new>
#include <numeric>

#include "2d/CCDrawNode.h"
#include "2d/CCLabel.h"

#include "cpg_node_PivotNode.h"
#include "cpg_node_GuideRectNode.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace algorithm_practice_astar
{
	CostNode::CostNode() :
		mStatusNode( nullptr )
		, mLabel_Cost2Start( nullptr )
		, mLabel_Cost2End( nullptr )
		, mLabel_TotalCost( nullptr )
	{}

	CostNode* CostNode::create( const cocos2d::Size node_size )
	{
		auto ret = new ( std::nothrow ) CostNode();
		if( !ret || !ret->init( node_size ) )
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

	bool CostNode::init( const cocos2d::Size node_size )
	{
		if( !Node::init() )
		{
			return false;
		}

		setContentSize( node_size );

		//
		// Debug
		//
		addChild( cpg_node::PivotNode::create(), std::numeric_limits<int>::max() );
		addChild( cpg_node::GuideRectNode::create( this ), std::numeric_limits<int>::max() );

		//
		// Background
		//
		{
			mStatusNode = DrawNode::create();
			addChild( mStatusNode, std::numeric_limits<int>::min() );
		}

		//
		// Left Top
		//
		{
			mLabel_Cost2Start = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 4 );
			mLabel_Cost2Start->setAnchorPoint( Vec2( 0.f, 1.f ) );
			mLabel_Cost2Start->setPositionY( getContentSize().height );
			addChild( mLabel_Cost2Start );
		}

		//
		// Right Top
		//
		{
			mLabel_Cost2End = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 4, Size::ZERO, TextHAlignment::RIGHT );
			mLabel_Cost2End->setAnchorPoint( Vec2( 1.f, 1.f ) );
			mLabel_Cost2End->setPosition( getContentSize().width, getContentSize().height );
			addChild( mLabel_Cost2End );
		}

		//
		// Center Bottom
		//
		{
			mLabel_TotalCost = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 7, Size::ZERO, TextHAlignment::CENTER );
			mLabel_TotalCost->setAnchorPoint( Vec2( 0.5f, 0.f ) );
			mLabel_TotalCost->setPositionX( getContentSize().width * 0.5f );
			addChild( mLabel_TotalCost );
		}

		return true;
	}

	void CostNode::SetCost( const int cost_2_start, const int cost_2_end )
	{
		mLabel_Cost2Start->setString( std::to_string( cost_2_start ) );
		mLabel_Cost2End->setString( std::to_string( cost_2_end ) );
		mLabel_TotalCost->setString( std::to_string( cost_2_start + cost_2_end ) );
	}
	void CostNode::SetStatus( const bool bOpen )
	{
		(
			bOpen
			? mStatusNode->drawSolidRect( Vec2::ZERO, Vec2( getContentSize().width, getContentSize().height ), Color4F::BLUE )
			: mStatusNode->drawSolidRect( Vec2::ZERO, Vec2( getContentSize().width, getContentSize().height ), Color4F::BLACK )
		);
	}
}
