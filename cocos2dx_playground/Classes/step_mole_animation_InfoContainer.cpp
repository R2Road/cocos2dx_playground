#include "step_mole_animation_InfoContainer.h"

namespace step_mole
{
	namespace animation
	{
		const cpg_animation::InfoContainerT& GetObjectInfoContainer()
		{
			static const cpg_animation::InfoContainerT animation_info_container = {
				{
					cpg_animation::eIndex::wait
					, 0.6f
					, std::vector<std::string>{ "step_mole_target_wait_0.png", "step_mole_target_wait_1.png" }
				}

				, {
					cpg_animation::eIndex::wakeup
					, 0.06f
					, std::vector<std::string>{ "step_mole_target_wkup_0.png", "step_mole_target_wkup_1.png", "step_mole_target_wkup_2.png", "step_mole_target_wkup_3.png", "step_mole_target_wkup_4.png" }
				}

				, {
					cpg_animation::eIndex::idle
					, 0.06f
					, std::vector<std::string>{ "step_mole_target_idl_0.png", "step_mole_target_idl_1.png", "step_mole_target_idl_2.png", "step_mole_target_idl_3.png", "step_mole_target_idl_4.png", "step_mole_target_idl_5.png", "step_mole_target_idl_6.png", "step_mole_target_idl_7.png" }
				}

				, {
					cpg_animation::eIndex::sleep
					, 0.1f
					, std::vector<std::string>{ "step_mole_target_slp_0.png", "step_mole_target_slp_1.png", "step_mole_target_slp_2.png", "step_mole_target_slp_3.png", "step_mole_target_slp_4.png", "step_mole_target_slp_5.png" }
				}

				, {
					cpg_animation::eIndex::damaged_1
					, 0.1f
					, std::vector<std::string>{ "step_mole_target_dmgd1_0.png", "step_mole_target_dmgd1_1.png", "step_mole_target_dmgd1_2.png" }
				}

				, {
					cpg_animation::eIndex::damaged_2
					, 0.06f
					, std::vector<std::string>{ "step_mole_target_dmgd2_0.png", "step_mole_target_dmgd2_1.png", "step_mole_target_dmgd2_2.png", "step_mole_target_dmgd2_3.png", "step_mole_target_dmgd2_4.png", "step_mole_target_dmgd2_5.png" }
				}
			};

			return animation_info_container;
		}

		const cpg_animation::InfoContainerT& GetEffectInfoContainer()
		{
			static const cpg_animation::InfoContainerT animation_info_container = {
				{
					cpg_animation::eIndex::attack_1
					, 0.04f
					, std::vector<std::string>{ "step_mole_effect_attack_01_0.png", "step_mole_effect_attack_01_1.png", "step_mole_effect_attack_01_2.png", "step_mole_effect_attack_01_3.png", "step_mole_effect_attack_01_4.png", "step_mole_effect_attack_01_5.png" }
				}
			};

			return animation_info_container;
		}
	}
}