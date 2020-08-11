#include "step_rain_of_chaos_game_AnimationInfoContainer.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		const cpg::animation::InfoContainerT& GetActorAnimationInfoContainer()
		{
			static const cpg::animation::InfoContainerT animation_info_container = {
				{
					cpg::animation::eIndex::idle
					, 0.1f
					, std::vector<std::string>{ "step_rain_of_chaos_actor_01_idle_0.png", "step_rain_of_chaos_actor_01_idle_1.png", "step_rain_of_chaos_actor_01_idle_2.png", "step_rain_of_chaos_actor_01_idle_3.png", "step_rain_of_chaos_actor_01_idle_4.png", "step_rain_of_chaos_actor_01_idle_5.png", "step_rain_of_chaos_actor_01_idle_6.png", "step_rain_of_chaos_actor_01_idle_7.png" }
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