#include "step_mole_animation_InfoContainer.h"

namespace step_mole
{
	namespace animation
	{
		const cpg::animation::ContainerT& GetInfoContainer()
		{
			static const cpg::animation::ContainerT animation_container = {
				{
					cpg::animation::eIndex::idle
					, 0.5f
					, std::vector<std::string>{ "actor001_idle_01.png", "actor001_idle_02.png", "actor001_idle_03.png" }
				}

				, {
					cpg::animation::eIndex::run
					, 0.2f
					, std::vector<std::string>{ "actor001_run_01.png", "actor001_run_02.png", "actor001_run_03.png", "actor001_run_04.png" }
				}

				, {
					cpg::animation::eIndex::win
					, 0.1f
					, std::vector<std::string>{ "actor001_win_01.png", "actor001_win_02.png" }
				}
			};

			return animation_container;
		}
	}
}