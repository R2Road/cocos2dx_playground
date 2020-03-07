#pragma once

#include <vector>

#include "2d/CCNode.h"

namespace step_clickclick
{
	namespace game
	{
		class Stage : public cocos2d::Node
		{
		private:
			struct Pannel
			{
				Pannel( const int count, cocos2d::Node* const pannel_node ) : Count( count ), PannelNode( pannel_node ) {}

				int Count;
				cocos2d::Node* const PannelNode;
			};

			Stage();;

		public:
			static Stage* create();

			bool init() override;

		private:
			std::vector<Pannel> Pannels;
		};
	}
}
