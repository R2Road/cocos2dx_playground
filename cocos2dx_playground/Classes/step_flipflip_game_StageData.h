#pragma once

#include <vector>

#include "cpg_GridIndexConverter.h"
#include "step_flipflip_game_Constant.h"

namespace step_flipflip
{
	namespace game
	{
		struct StageConfig;

		class StageData
		{
		public:
			struct CardInfo
			{
				eCardType Type = eCardType::A;
				eCardStatus Status = eCardStatus::Close;
			};

			using ContainerT = std::vector<CardInfo>;

			StageData();

			eCardType GetType( const int x, const int y ) const;

			eCardStatus GetStatus( const int x, const int y ) const;
			void SetStatus( const eCardStatus status, const int x, const int y );
			int GetClosedCardsCount() const { return mClosedCardsCount; }

			bool Reset( const int width, const int height, const int shuffle_limit );

		private:
			cpg::GridIndexConverter mIndexConverter;
			ContainerT mContainer;

			int mClosedCardsCount;
		};
	}
}
