#pragma once

#include <vector>

#include "2d/CCNode.h"

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
			class Pannel
			{
			public:
				Pannel( const int index, const int count, cocos2d::Node* const pannel_node, cocos2d::Label* const label_node );

				void SetVisible( const bool visible );

			private:
				int mIndex;
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
			const int mStageWidth;
			const int mStageHeight;
			const int mCenterX;
			const int mCenterY;
			std::vector<Pannel> Pannels;
		};
	}
}
