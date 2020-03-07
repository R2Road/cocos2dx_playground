#pragma once

#include <vector>

#include "2d/CCNode.h"
#include "ui/UIWidget.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_clickclick
{
	namespace game
	{
		class Stage : public cocos2d::Node
		{
		private:
			enum class ePannelType
			{
				Single,
				Together,
			};

			class Pannel
			{
			public:
				Pannel( const int index, const int count, cocos2d::Node* const pannel_node, cocos2d::Label* const label_node );

				void Init( const int count );
				void SetVisible( const bool visible );
				void Action();

				ePannelType GetType() const { return mPannelType; }
				int GetCount() const { return mCount; }

			private:
				int mIndex;
				ePannelType mPannelType;
				int mCount;
				cocos2d::Node* const mPannelNode;
				cocos2d::Label* const mLabelNode;
			};

			Stage();

		public:
			static Stage* create();

			bool init() override;

			void Setup( const int width, const int height );

		private:
			void onPannel( Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

		private:
			const int mStageWidth;
			const int mStageHeight;
			const int mCenterX;
			const int mCenterY;
			std::vector<Pannel> Pannels;
		};
	}
}
