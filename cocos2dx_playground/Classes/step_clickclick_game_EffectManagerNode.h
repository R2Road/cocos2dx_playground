#pragma once

#include "2d/CCNode.h"

#include "step_clickclick_game_Constant.h"

namespace step_clickclick
{
	namespace game
	{
		class EffectManagerNode : public cocos2d::Node
		{
		public:
			struct Config
			{
				bool bShowBackground = false;
			};

		private:
			EffectManagerNode();

		public:
			static EffectManagerNode* create( const Config config, const int width, const int height );

		private:
			bool init( const Config config, const int width, const int height );

		public:
			void PlayEffect( const int index, const eEffectIndex effect_index );
		};
	}
}
