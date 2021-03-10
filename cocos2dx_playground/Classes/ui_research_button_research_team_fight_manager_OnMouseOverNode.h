#pragma once

#include "ui/UILayout.h"

namespace ui_research
{
	namespace button_research
	{
		namespace team_fight_manager
		{
			class OnMouseOverNode : public cocos2d::ui::Layout
			{
			private:
				OnMouseOverNode();

			public:
				static OnMouseOverNode* create();

			private:
				bool init() override;

				void update4Rotation( float dt );

			public:
				void setVisible( bool visible ) override;

			private:
				cocos2d::Node* mRotateNode;
				float mElapsedTime;
			};
		}
	}
}
