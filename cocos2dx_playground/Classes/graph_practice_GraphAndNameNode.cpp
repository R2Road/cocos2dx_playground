#include "graph_practice_GraphAndNameNode.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCLabel.h"
#include "2d/CCSpriteBatchNode.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_StringTable.h"

#include "graph_practice_GraphViewNode.h"

USING_NS_CC;

namespace graph_practice
{
	GraphAndNameNode::GraphAndNameNode() : mGraphViewNode( nullptr )
	{}

	GraphAndNameNode* GraphAndNameNode::create( const char* title, const int part_width, const int part_height, const EvaluatorFunc& evaluator_func )
	{
		auto ret = new ( std::nothrow ) GraphAndNameNode();
		if( !ret || !ret->init( title, part_width, part_height, evaluator_func ) )
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

	bool GraphAndNameNode::init( const char* title, const int part_width, const int part_height, const EvaluatorFunc& evaluator_func )
	{
		if( !Node::init() )
		{
			return false;
		}

		setContentSize( Size( part_width, part_height ) );

		// Pivot
		{
			auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
			pivot->setScale( 2.f );
			addChild( pivot, std::numeric_limits<int>::max() );
		}

		//
		// Background Guide
		//
		{
			auto layer = LayerColor::create( Color4B::MAGENTA, getContentSize().width, getContentSize().height );
			layer->setAnchorPoint( Vec2::ZERO );
			addChild( layer, std::numeric_limits<int>::min() );
		}

		const int HeaderSize = 10;

		int ViewNodeSize = 0;
		int ViewNodeY = 0;
		const int ViewNodeMargin = 4;

		//
		// Title
		//
		{
			auto label = Label::createWithTTF( title, cpg::StringTable::GetFontPath(), 10 );
			label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
			label->setPositionX( getContentSize().width * 0.5f );
			addChild( label, std::numeric_limits<int>::max() );

			ViewNodeY = static_cast<int>( label->getContentSize().height ) + ViewNodeMargin;

			ViewNodeSize = getContentSize().height - ViewNodeY - ( HeaderSize + ViewNodeMargin );
			ViewNodeSize = getContentSize().width < ViewNodeSize ? getContentSize().width : ViewNodeSize;
		}

		//
		// Graph View
		//
		{
			mGraphViewNode = graph_practice::GraphViewNode::create( "Sine", ViewNodeSize, ViewNodeSize, evaluator_func );
			mGraphViewNode->setPosition(
				( getContentSize().width * 0.5f ) - ( mGraphViewNode->getContentSize().width * 0.5f )
				, ViewNodeY
			);
			addChild( mGraphViewNode );
		}

		return true;
	}

	void GraphAndNameNode::UpdateView( const float g_x )
	{
		mGraphViewNode->UpdateView( g_x );
	}
}
