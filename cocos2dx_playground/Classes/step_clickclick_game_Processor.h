#pragma once

namespace step_clickclick
{
	namespace game
	{
		class Stage;
		class StageView;

		class Processor
		{
		private:
			Processor() = delete;

		public:
			static void Do( Stage* stage, StageView* stage_view, const int block_linear_index, int* out_score );
		};
	}
}
