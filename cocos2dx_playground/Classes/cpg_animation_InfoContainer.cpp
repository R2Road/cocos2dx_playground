#include "cpg_animation_InfoContainer.h"

#include <new>

namespace cpg
{
	namespace animation
	{
		InfoContainer::InfoContainer() : mContainer()
		{}

		InfoContainerUp InfoContainer::create()
		{
			InfoContainerUp ret( new ( std::nothrow ) InfoContainer() );


			ret->mContainer.emplace_back(
				cpg::animation::eIndex::idle
				, 0.5f
				, std::vector<std::string>{ "actor001_idle_01.png", "actor001_idle_02.png", "actor001_idle_03.png" }
			);

			ret->mContainer.emplace_back(
				cpg::animation::eIndex::run
				, 0.2f
				, std::vector<std::string>{ "actor001_run_01.png", "actor001_run_02.png", "actor001_run_03.png", "actor001_run_04.png" }
			);

			ret->mContainer.emplace_back(
				cpg::animation::eIndex::win
				, 0.1f
				, std::vector<std::string>{ "actor001_win_01.png", "actor001_win_02.png" }
			);


			

			return ret;
		}
	}
}