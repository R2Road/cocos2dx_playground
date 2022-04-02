#pragma once

#include <functional>

#include "ui/UIWidget.h"

NS_CC_BEGIN
class EventListenerMouse;
NS_CC_END

namespace cpg_ui
{
	class EXButtonNode : public cocos2d::ui::Widget
	{
	public:
		enum class eButtonEvent
		{
			MouseOver,
			MouseLeave,
			Push,
			Move,
			Release
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
		EXButtonNode();

	public:
		static EXButtonNode* EXButtonNode::create( const cocos2d::Size& size );

	private:
		bool init( const cocos2d::Size& size );

	public:
		void onEnter() override;
		void onExit() override;

		void SetView_Button( const eViewIndex view_index, Node* node );
		void SetView_Content( Node* node );
		void SetView_Background( Node* node );
		void SetCallback( const OnButtonCallback& callback ) { mOnButtonCallback = callback; }

	private:
		void onButton( const eButtonEvent button_event );

		void showView( const eViewIndex view_index );

	private:
		cocos2d::EventListenerMouse* mMouseEventListener;
		bool mbOnMouseOver;

		cocos2d::Node* mViewNodes[eViewIndex::SIZE];
		cocos2d::Node* mContentViewNode;
		cocos2d::Node* mBackgroundNode;

		OnButtonCallback mOnButtonCallback;
	};
}
