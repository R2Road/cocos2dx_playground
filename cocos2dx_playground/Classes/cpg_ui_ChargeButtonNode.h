#pragma once

#include <functional>

#include "ui/UIWidget.h"

NS_CC_BEGIN
class EventListenerMouse;
NS_CC_END

namespace cpg_ui
{
	class ChargeButtonPushedNode : public cocos2d::Node
	{
	protected:
		ChargeButtonPushedNode() {}

	public:
		virtual void SetChargeRate( const float charge_rate ) = 0;
	};

	class ChargeButtonNode : public cocos2d::ui::Widget
	{
	public:
		enum class eButtonEvent
		{
			MouseOver,
			MouseLeave,
			Push,
			Move,
			Release,
			Charged,
		};
		enum eViewIndex
		{
			Normal,
			MouseOver,
			Push,

			SIZE,
		};

		using OnButtonCallback = std::function<void( const eButtonEvent )>;

	private:
		ChargeButtonNode( const float time_limit );

	public:
		static ChargeButtonNode* ChargeButtonNode::create( const cocos2d::Size& size, const float time_limit );

	private:
		bool init( const cocos2d::Size& size );

	public:
		void onEnter() override;
		void onExit() override;

		void SetPushedView( ChargeButtonPushedNode* pushed_node );
		void SetView( const eViewIndex view_index, Node* node );
		void SetBackground( Node* node );
		void SetCallback( const OnButtonCallback& callback ) { mOnButtonCallback = callback; }

	private:
		void onButton( const eButtonEvent button_event );

		void showView( const eViewIndex view_index );
		
		void update4Charge( const float dt );
		void setPushedViewVisible( const bool visible );
		void updatePushedView( const float charge_rate );

	private:
		cocos2d::EventListenerMouse* mMouseEventListener;
		bool mbOnMouseOver;

		cocos2d::Node* mViewNodes[eViewIndex::SIZE];
		ChargeButtonPushedNode* mPushedNode;
		cocos2d::Node* mBackgroundNode;

		OnButtonCallback mOnButtonCallback;

		const float mTimelimit;
		float mCharge;
	};
}
