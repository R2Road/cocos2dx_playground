#pragma once

#include <memory>
#include <vector>

#include "cpg_Animation_Info.h"

namespace cpg
{
	namespace animation
	{
		using InfoContainerUp = std::unique_ptr<class InfoContainer>;

		class InfoContainer
		{
		private:
			using ContainerT = std::vector<cpg::animation::Info>;

			InfoContainer();

		public:
			static InfoContainerUp create();

			const ContainerT& Get() const { return mContainer; }

		private:
			ContainerT mContainer;
		};
	}
}
