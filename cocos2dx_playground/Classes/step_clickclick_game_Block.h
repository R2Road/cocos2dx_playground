#pragma once

#include "step_clickclick_game_Constant.h"

namespace step_clickclick
{
	namespace game
	{
		class Block
		{
		public:
			Block( const int index );

			void Reset( const eBlockType type, const int life );

			void DecreaseLife();
			void IncreaseLife();
			void Die();

			int GetIndex() const { return mIndex; }
			eBlockType GetType() const { return mBlockType; }
			bool IsActive() const { return mActive; }
			int GetLife() const { return mLife; }

		private:
			int mIndex;
			eBlockType mBlockType;
			bool mActive;
			int mLife;
		};
	}
}
