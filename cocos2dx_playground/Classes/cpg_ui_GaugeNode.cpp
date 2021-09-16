#include "cpg_ui_GaugeNode.h"

#include <new>
#include <numeric>

#include "2d/CCDrawNode.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/ccUTF8.h"

#include "cpg_StringTable.h"
#include "cpg_node_PivotNode.h"

USING_NS_CC;

namespace
{
	const Color4F GaugeColor1( Color4F::GREEN );
	const Color4F GaugeColor2( Color4F::ORANGE );
}

namespace cpg_ui
{
	GaugeNode::GaugeNode( const cocos2d::Size max_size, const int max_amount ) :
		mMaxSize( max_size )

		, mGaugeMax( max_amount )
		, mGaugeMin( 0 )
		, mGaugeCurrent( max_amount )
		, mGaugeAnimationCurrent( max_amount )

		, mGaugeViewNode( nullptr )
		, mGaugeAnimationViewNode( nullptr )

		, mGaugeStatisticsViewNode( nullptr )
		, mGaugeAnimationStatisticsViewNode( nullptr )
	{}

	GaugeNode* GaugeNode::create( const cocos2d::Size max_size, const int max_amount )
	{
		auto ret = new ( std::nothrow ) GaugeNode( max_size, max_amount );
		if( !ret || !ret->init() )
		{
			CC_SAFE_DELETE( ret );
		}
		else
		{
			ret->autorelease();
		}

		return ret;
	}

	bool GaugeNode::init()
	{
		if( !Node::init() )
		{
			return false;
		}

		//
		// Pivot
		//
		{
			addChild( cpg_node::PivotNode::create(), std::numeric_limits<int>::max() );
		}

		//
		// Test Gauge
		//
		{
			mGaugeViewNode = DrawNode::create();
			addChild( mGaugeViewNode, 1 );

			mGaugeAnimationViewNode = DrawNode::create();
			mGaugeAnimationViewNode->setPositionY( 5.f );
			addChild( mGaugeAnimationViewNode, 0 );
		}

		//
		// Test Gauge Stat View
		//
		{
			mGaugeStatisticsViewNode = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			mGaugeStatisticsViewNode->setColor( Color3B( GaugeColor1 ) );
			mGaugeStatisticsViewNode->setPositionY( 10.f );
			addChild( mGaugeStatisticsViewNode );

			mGaugeAnimationStatisticsViewNode = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			mGaugeAnimationStatisticsViewNode->setColor( Color3B( GaugeColor2 ) );
			mGaugeAnimationStatisticsViewNode->setPositionY( 20.f );
			addChild( mGaugeAnimationStatisticsViewNode );
		}

		//
		//
		//
		updateGaugeView();
		updateGaugeAnimationView();

		return true;
	}


	void GaugeNode::UpdateCurrent( const int new_current )
	{
		mGaugeCurrent = std::min( mGaugeMax, std::max( 0, new_current ) );

		updateGaugeView();
		requestUpdateGaugeAnimation();
	}


	void GaugeNode::updateGaugeView()
	{
		const float gauge_rate = static_cast<float>( mGaugeCurrent ) / static_cast<float>( mGaugeMax );

		mGaugeViewNode->clear();
		mGaugeViewNode->drawSolidRect(
			Vec2::ZERO
			, Vec2( mMaxSize.width * gauge_rate, mMaxSize.height )
			, GaugeColor1
		);

		mGaugeStatisticsViewNode->setString( StringUtils::format(
			"%d / %d"
			, mGaugeCurrent
			, mGaugeMax
		) );
	}
	void GaugeNode::updateGaugeAnimationView()
	{
		const float gauge_rate = static_cast<float>( mGaugeAnimationCurrent ) / static_cast<float>( mGaugeMax );

		mGaugeAnimationViewNode->clear();
		mGaugeAnimationViewNode->drawSolidRect(
			Vec2::ZERO
			, Vec2( mMaxSize.width * gauge_rate, mMaxSize.height )
			, GaugeColor2
		);

		mGaugeAnimationStatisticsViewNode->setString( StringUtils::format(
			"%d / %d"
			, mGaugeAnimationCurrent
			, mGaugeMax
		) );
	}


	void GaugeNode::requestUpdateGaugeAnimation()
	{
		if( !isScheduled( schedule_selector( GaugeNode::update4GaugeAnimation ) ) )
		{
			schedule( schedule_selector( GaugeNode::update4GaugeAnimation ) );
		}
	}
	void GaugeNode::update4GaugeAnimation( float /*delta_time*/ )
	{
		if( mGaugeCurrent == mGaugeAnimationCurrent )
		{
			unschedule( schedule_selector( GaugeNode::update4GaugeAnimation ) );
			return;
		}

		if( mGaugeCurrent > mGaugeAnimationCurrent )
		{
			mGaugeAnimationCurrent = mGaugeCurrent;
		}
		else //if( mGaugeCurrent < mGaugeAnimationCurrent )
		{
			mGaugeAnimationCurrent = std::max( mGaugeCurrent, mGaugeAnimationCurrent - 1 );
		}

		updateGaugeAnimationView();
	}
}