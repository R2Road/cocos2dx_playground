#include "graph_practice_GraphViewNode.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCDrawNode.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_StringTable.h"
#include "cpg_node_PivotNode.h"

USING_NS_CC;

namespace graph_practice
{
	GraphViewNode::GraphViewNode( const Config config, const EvaluatorFunc& evaluator_func ) :
		mConfig( { std::max( 20, config.PartWidth ), std::max( 20, config.PartHeight ), config.AllowedTimeX, config.AllowedTimeY } )
		, mEvaluatorFunc( evaluator_func )

		, mIndicatorNode( nullptr )
		, mIndicatorXNode( nullptr )
		, mIndicatorYNode( nullptr )
		, mIndicatorBridgeXNode( nullptr )
		, mIndicatorBridgeYNode( nullptr )
		, mIndicatorY2Node( nullptr )
	{}

	GraphViewNode* GraphViewNode::create( const DebugConfig debug_config, const Config config, const EvaluatorFunc& evaluator_func )
	{
		auto ret = new ( std::nothrow ) GraphViewNode( config, evaluator_func );
		if( !ret || !ret->init( debug_config ) )
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

	bool GraphViewNode::init( const DebugConfig debug_config )
	{
		if( !Node::init() )
		{
			return false;
		}

		const int HeaderHeight = 10;
		const Size GraphSize = Size( mConfig.PartWidth * mConfig.AllowedTimeX, mConfig.PartHeight * mConfig.AllowedTimeY );
		setContentSize( GraphSize + Size( 0.f, HeaderHeight ) );

		// Pivot
		if( debug_config.bShowPivot )
		{
			addChild( cpg_node::PivotNode::create(), std::numeric_limits<int>::max() );
		}

		//
		// Background Guide
		//
		if( debug_config.bShowBackgroundGuide )
		{
			auto layer = LayerColor::create( Color4B::BLUE, getContentSize().width, getContentSize().height );
			layer->setAnchorPoint( Vec2::ZERO );
			addChild( layer, std::numeric_limits<int>::min() );
		}

		//
		// Graph View
		//
		{
			auto view_node = Node::create();
			view_node->setContentSize( Size( GraphSize.width, GraphSize.height ) );
			addChild( view_node );

			//
			// Background
			//
			{
				auto background = LayerColor::create( Color4B( 44u, 44u, 44u, 255u ), view_node->getContentSize().width, view_node->getContentSize().height );
				view_node->addChild( background, std::numeric_limits<int>::min() );
			}

			//
			// Grid
			//
			{
				const Color4F GridColor1( 0.23f, 0.23f, 0.23f, 1.f );
				const Color4F GridColor2( 0.3f, 0.3f, 0.3f, 1.f );

				auto draw_node = DrawNode::create();
				view_node->addChild( draw_node, std::numeric_limits<int>::min() + 1 );

				// Vertical : 1 per 1
				{
					const float Spacing = mConfig.PartWidth * 0.1f;
					const int guide_count = GraphSize.width / Spacing;
					for( int i = 1; guide_count >= i; ++i )
					{
						draw_node->drawLine( Vec2( Spacing * i, 0.f ), Vec2( Spacing * i, GraphSize.height ), GridColor1 );
					}
				}

				// Horizontal : 1 per 1
				{
					const float Spacing = mConfig.PartHeight * 0.1f;
					const int guide_count = GraphSize.height / Spacing;
					for( int i = 1; guide_count >= i; ++i )
					{
						draw_node->drawLine( Vec2( 0.f, Spacing * i ), Vec2( GraphSize.width, Spacing * i ), GridColor1 );
					}
				}

				// Vertical : 1 per 10
				{
					const float Spacing = mConfig.PartWidth * 0.1f;
					const int guide_count = GraphSize.width / Spacing;
					for( int i = 0; guide_count >= i; i += 10 )
					{
						draw_node->drawLine( Vec2( Spacing * i, 0.f ), Vec2( Spacing * i, GraphSize.height ), GridColor2 );
					}
				}

				// Horizontal : 1 per 10
				{
					const float Spacing = mConfig.PartHeight * 0.1f;
					const int guide_count = GraphSize.height / Spacing;
					for( int i = 0; guide_count >= i; i += 10 )
					{
						draw_node->drawLine( Vec2( 0.f, Spacing * i ), Vec2( GraphSize.width, Spacing * i ), GridColor2 );
					}
				}
			}

			//
			// Axis View
			//
			{
				const Color4F AxisColor( 0.8f, 0.8f, 0.8f, 1.f );

				auto draw_node = DrawNode::create( 2.f );
				view_node->addChild( draw_node, std::numeric_limits<int>::min() + 2 );

				// x
				draw_node->drawLine( Vec2( 0.f, 0.f ), Vec2( GraphSize.width, 0.f ), AxisColor );

				// y
				draw_node->drawLine( Vec2( 0.f, 0.f ), Vec2( 0.f, GraphSize.height ), AxisColor );

				// diagonal
				const float diagonal_view_size = std::min( GraphSize.width, GraphSize.height );
				draw_node->drawLine( Vec2( 0.f, 0.f ), Vec2( diagonal_view_size, diagonal_view_size ), AxisColor );
			}

			//
			// Dot
			//
			{
				auto draw_node = DrawNode::create();
				view_node->addChild( draw_node, std::numeric_limits<int>::min() + 3 );

				const float dot_radius = 0.4f;
				const int dot_count = mConfig.AllowedTimeX * 100;
				float g_x = 0.f;
				float g_y = 0.f;
				for( int i = 0; dot_count > i; ++i )
				{
					g_x = 0.01f * i;
					g_y = mEvaluatorFunc( g_x );

					draw_node->drawDot(
						Vec2( mConfig.PartWidth * g_x, mConfig.PartHeight * g_y )
						, dot_radius
						, Color4F::GREEN
					);
				}
			}

			//
			// Current Value Indicator
			//
			{
				mIndicatorNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorNode->setScale( 6.f );
				mIndicatorNode->setColor( Color3B( 73, 179, 255 ) );
				view_node->addChild( mIndicatorNode, 2 );

				mIndicatorXNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorXNode->setScale( 6.f );
				mIndicatorXNode->setColor( Color3B::RED );
				view_node->addChild( mIndicatorXNode, 1 );

				mIndicatorYNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorYNode->setScale( 6.f );
				mIndicatorYNode->setColor( Color3B::RED );
				view_node->addChild( mIndicatorYNode, 1 );

				mIndicatorBridgeXNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorBridgeXNode->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				mIndicatorBridgeXNode->setOpacity( 80u );
				view_node->addChild( mIndicatorBridgeXNode );

				mIndicatorBridgeYNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorBridgeYNode->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
				mIndicatorBridgeYNode->setOpacity( 80u );
				view_node->addChild( mIndicatorBridgeYNode );

				mIndicatorY2Node = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorY2Node->setScale( 6.f );
				mIndicatorY2Node->setColor( Color3B( 73, 179, 255 ) );
				mIndicatorY2Node->setPositionY( view_node->getContentSize().height + ( HeaderHeight * 0.5f ) );
				view_node->addChild( mIndicatorY2Node );
			}
		}

		return true;
	}

	void GraphViewNode::UpdateView( const float g_x )
	{
		const float fixed_g_x = std::min( g_x, mConfig.AllowedTimeX );

		const auto g_y = mEvaluatorFunc( fixed_g_x );

		mIndicatorNode->setPosition(
			mConfig.PartWidth * fixed_g_x
			, mConfig.PartHeight * g_y
		);

		mIndicatorXNode->setPositionX( mConfig.PartWidth * fixed_g_x );
		mIndicatorYNode->setPositionY( mConfig.PartHeight * g_y );

		mIndicatorBridgeXNode->setContentSize( Size( 1.f, std::abs( mIndicatorNode->getPositionY() ) ) );
		mIndicatorBridgeXNode->setScaleY( 0 <= mIndicatorNode->getPositionY() ? 1.f : -1.f );
		mIndicatorBridgeXNode->setPositionX( mConfig.PartWidth * fixed_g_x );

		mIndicatorBridgeYNode->setContentSize( Size( mIndicatorNode->getPositionX(), 1.f ) );
		mIndicatorBridgeYNode->setScaleY( 0 <= mIndicatorNode->getPositionX() ? 1.f : -1.f );
		mIndicatorBridgeYNode->setPositionY( mConfig.PartHeight * g_y );

		mIndicatorY2Node->setPositionX( mConfig.PartHeight * g_y );
	}
}
