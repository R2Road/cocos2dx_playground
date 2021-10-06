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
	GaugeNode::GaugeNode( const cocos2d::Size max_size ) :
		mMaxSize( max_size )

		, mCurrentRate( 1.f )
		, mAnimationRate( 1.f )

		, mGaugeViewNode( nullptr )
		, mGaugeAnimationViewNode( nullptr )

		, mGaugeStatisticsViewNode( nullptr )
		, mGaugeAnimationStatisticsViewNode( nullptr )
	{}

	GaugeNode* GaugeNode::create( const cocos2d::Size max_size )
	{
		auto ret = new ( std::nothrow ) GaugeNode( max_size );
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
			mGaugeStatisticsViewNode->setPositionY( 40.f );
			addChild( mGaugeStatisticsViewNode );

			mGaugeAnimationStatisticsViewNode = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			mGaugeAnimationStatisticsViewNode->setColor( Color3B( GaugeColor2 ) );
			mGaugeAnimationStatisticsViewNode->setPositionY( 50.f );
			addChild( mGaugeAnimationStatisticsViewNode );
		}

		//
		//
		//
		updateGaugeView();
		updateGaugeAnimationView();

		return true;
	}


	void GaugeNode::UpdateCurrent( float new_rate )
	{
		mCurrentRate = std::min( 1.f, std::max( 0.f, new_rate ) );

		updateGaugeView();
		requestUpdateGaugeAnimation();
	}


	void GaugeNode::updateGaugeView()
	{
		mGaugeViewNode->clear();
		mGaugeViewNode->drawSolidRect(
			Vec2::ZERO
			, Vec2( mMaxSize.width * mCurrentRate, mMaxSize.height )
			, GaugeColor1
		);

		mGaugeStatisticsViewNode->setString( StringUtils::format(
			"%.2f / 1"
			, mCurrentRate
		) );
	}
	void GaugeNode::updateGaugeAnimationView()
	{
		mGaugeAnimationViewNode->clear();
		mGaugeAnimationViewNode->drawSolidRect(
			Vec2::ZERO
			, Vec2( mMaxSize.width * mAnimationRate, mMaxSize.height )
			, GaugeColor2
		);

		mGaugeAnimationStatisticsViewNode->setString( StringUtils::format(
			"%.2f / 1"
			, mAnimationRate
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
		if( mCurrentRate >= mAnimationRate )
		{
			mAnimationRate = mCurrentRate;

			unschedule( schedule_selector( GaugeNode::update4GaugeAnimation ) );
		}
		else if( mCurrentRate < mAnimationRate )
		{
			mAnimationRate = std::max( mCurrentRate, mAnimationRate - 0.02f );
		}

		updateGaugeAnimationView();
	}
}