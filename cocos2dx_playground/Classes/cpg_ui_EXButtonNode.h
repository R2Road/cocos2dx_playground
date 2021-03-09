#pragma once

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

	private:
		EXButtonNode();

	public:
		static EXButtonNode* EXButtonNode::create( const cocos2d::Size& size );

	private:
		bool init( const cocos2d::Size& size );

	public:
		void onEnter() override;
		void onExit() override;

		void SetView( const eViewIndex view_index, Node* node );
		void SetBackground( Node* node );

	private:
		void onButton( const eButtonEvent button_event );

		void showView( const eViewIndex view_index );

	private:
		cocos2d::EventListenerMouse* mMouseEventListener;
		bool mbOnMouseOver;

		cocos2d::Node* mViewNodes[eViewIndex::SIZE];
		cocos2d::Node* mBackgroundNode;
	};
}
