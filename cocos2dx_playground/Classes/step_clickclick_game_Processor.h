#pragma once

namespace step_clickclick
{
	namespace game
	{
		class EffectManagerNode;

		class Stage;
		class StageViewNode;

		class Processor
		{
		private:
			Processor() = delete;

		public:
			static void Do( Stage* stage, StageViewNode* stage_view_node, EffectManagerNode* effect_manager_node, const int block_linear_index, int* out_score );
		};
	}
}
