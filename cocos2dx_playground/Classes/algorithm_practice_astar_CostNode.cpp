#include "algorithm_practice_astar_CostNode.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"

#include "cpg_node_PivotNode.h"
#include "cpg_node_GuideRectNode.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace algorithm_practice_astar
{
	CostNode::CostNode()
	{}

	Node* CostNode::create( const cocos2d::Size node_size )
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
		// Left Top
		//
		{
			auto label = Label::createWithTTF( "999", cpg::StringTable::GetFontPath(), 4, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPositionY( getContentSize().height );
			addChild( label );
		}

		//
		// Right Top
		//
		{
			auto label = Label::createWithTTF( "999", cpg::StringTable::GetFontPath(), 4, Size::ZERO, TextHAlignment::RIGHT );
			label->setAnchorPoint( Vec2( 1.f, 1.f ) );
			label->setPosition( getContentSize().width, getContentSize().height );
			addChild( label );
		}

		//
		// Center Bottom
		//
		{
			auto label = Label::createWithTTF( "999", cpg::StringTable::GetFontPath(), 7, Size::ZERO, TextHAlignment::CENTER );
			label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
			label->setPositionX( getContentSize().width * 0.5f );
			addChild( label );
		}

		return true;
	}
}
