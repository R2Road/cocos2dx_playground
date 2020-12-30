#pragma once

namespace step_clickclick
{
	namespace game
	{
		class EffectManagerNode;

		class Stage;
		class StageView;

		class Processor
		{
		private:
			Processor() = delete;

		public:
			static void Do( Stage* stage, StageView* stage_view, EffectManagerNode* effect_manager_node, const int block_linear_index, int* out_score );
		};
	}
}
