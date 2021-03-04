#include "graph_practice_GraphAndNameNode.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"

#include "cpg_StringTable.h"
#include "cpg_node_PivotNode.h"

#include "graph_practice_GraphViewNode.h"

USING_NS_CC;

namespace graph_practice
{
	GraphAndNameNode::GraphAndNameNode() : mGraphViewNode( nullptr )
	{}

	GraphAndNameNode* GraphAndNameNode::create( const Config config, const char* title, const int part_width, const int part_height, const EvaluatorFunc& evaluator_func )
	{
		auto ret = new ( std::nothrow ) GraphAndNameNode();
		if( !ret || !ret->init( config, title, part_width, part_height, evaluator_func ) )
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

	bool GraphAndNameNode::init( const Config config, const char* title, const int part_width, const int part_height, const EvaluatorFunc& evaluator_func )
	{
		if( !Node::init() )
		{
			return false;
		}

		setContentSize( Size( part_width, part_height ) );

		//
		// Pivot
		//
		if( config.bShowPivot )
		{
			addChild( cpg_node::PivotNode::create(), std::numeric_limits<int>::max() );
		}

		//
		// Background Guide
		//
		if( config.bShowBackgroundGuide)
		{
			auto layer = LayerColor::create( Color4B::MAGENTA, getContentSize().width, getContentSize().height );
			layer->setAnchorPoint( Vec2::ZERO );
			addChild( layer, std::numeric_limits<int>::min() );
		}

		//
		// Title
		//
		auto label = Label::createWithTTF( title, cpg::StringTable::GetFontPath(), 10 );
		label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
		addChild( label, std::numeric_limits<int>::max() );

		//
		// Graph View
		//
		{
			mGraphViewNode = graph_practice::GraphViewNode::create(
				{ config.bShowPivot, config.bShowBackgroundGuide }
				, { part_width, part_height, 1.f, 1.f }
				, evaluator_func );
			addChild( mGraphViewNode );
		}

		//
		// Align
		//
		const int LabelMargin = 4;
		const int ViewNodeWidth = label->getContentSize().width > mGraphViewNode->getContentSize().width ? label->getContentSize().width : mGraphViewNode->getContentSize().width;
		const int ViewNodeHeight = label->getContentSize().height + LabelMargin + mGraphViewNode->getContentSize().height;

		label->setPositionX( ViewNodeWidth * 0.5f );
		mGraphViewNode->setPosition(
			( ViewNodeWidth * 0.5f ) - ( mGraphViewNode->getContentSize().width * 0.5f )
			, label->getContentSize().height + LabelMargin
		);

		return true;
	}

	void GraphAndNameNode::UpdateView( const float g_x )
	{
		mGraphViewNode->UpdateView( g_x );
	}
}
