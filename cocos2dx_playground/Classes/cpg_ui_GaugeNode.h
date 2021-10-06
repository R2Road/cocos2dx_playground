#pragma once

#include "2d/CCNode.h"

NS_CC_BEGIN
	class DrawNode;
	class Label;
NS_CC_END

namespace cpg_ui
{
	class GaugeNode : public cocos2d::Node
	{
	private:
		GaugeNode( const cocos2d::Size max_size );

	public:
		static GaugeNode* create( const cocos2d::Size max_size );

	private:
		bool init();

	public:
		void UpdateCurrent( const float new_rate );

	private:
		void updateGaugeView();
		void updateGaugeAnimationView();

		void requestUpdateGaugeAnimation();
		void update4GaugeAnimation( float delta_time );

	private:
		const cocos2d::Size mMaxSize;

		float mCurrentRate;
		float mAnimationRate;

		cocos2d::DrawNode* mGaugeViewNode;
		cocos2d::DrawNode* mGaugeAnimationViewNode;

		cocos2d::Label* mGaugeStatisticsViewNode;
		cocos2d::Label* mGaugeAnimationStatisticsViewNode;
	};
}
