#include "step_rain_of_chaos_game_AnimationInfoContainer.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		const cpg::animation::InfoContainerT& GetActorAnimationInfoContainer()
		{
			static const cpg::animation::InfoContainerT animation_info_container = {
				{
					cpg::animation::eIndex::run
					, 0.2f
					, std::vector<std::string>{ "actor001_run_01.png", "actor001_run_02.png", "actor001_run_03.png", "actor001_run_04.png" }
				}
			};

			return animation_info_container;
		}

		const cpg::animation::InfoContainerT& GetBulletAnimationInfoContainer()
		{
			static const cpg::animation::InfoContainerT animation_info_container = {
				{
					cpg::animation::eIndex::wakeup
					, 0.06f
					, std::vector<std::string>{ "step_rain_of_chaos_bullet_01_wakeup_0.png", "step_rain_of_chaos_bullet_01_wakeup_1.png", "step_rain_of_chaos_bullet_01_wakeup_2.png", "step_rain_of_chaos_bullet_01_wakeup_3.png" }
				}

				, {
					cpg::animation::eIndex::idle
					, 0.06f
					, std::vector<std::string>{ "step_rain_of_chaos_bullet_01_idle_0.png", "step_rain_of_chaos_bullet_01_idle_1.png" }
				}

				, {
					cpg::animation::eIndex::sleep
					, 0.1f
					, std::vector<std::string>{ "step_rain_of_chaos_bullet_01_damaged_0.png", "step_rain_of_chaos_bullet_01_damaged_1.png", "step_rain_of_chaos_bullet_01_damaged_2.png", "step_rain_of_chaos_bullet_01_damaged_0.png", "step_rain_of_chaos_bullet_01_damaged_1.png", "step_rain_of_chaos_bullet_01_damaged_2.png" }
				}

				, {
					cpg::animation::eIndex::damaged_1
					, 0.1f
					, std::vector<std::string>{ "step_rain_of_chaos_bullet_01_damaged_0.png", "step_rain_of_chaos_bullet_01_damaged_1.png", "step_rain_of_chaos_bullet_01_damaged_2.png" }
				}

				, {
					cpg::animation::eIndex::damaged_2
					, 0.06f
					, std::vector<std::string>{ "step_rain_of_chaos_bullet_01_damaged_0.png", "step_rain_of_chaos_bullet_01_damaged_1.png", "step_rain_of_chaos_bullet_01_damaged_2.png" }
				}
			};

			return animation_info_container;
		}
	}
}