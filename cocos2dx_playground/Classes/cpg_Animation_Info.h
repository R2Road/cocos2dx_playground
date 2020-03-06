#pragma once

#include <vector>

#include "cpg_Animation_Index.h"

namespace cpg
{
	namespace animation
	{
		struct Info
		{
			cpg::animation::eIndex Index = cpg::animation::eIndex::none;
			float delay = 0.f;
			std::vector<std::string> SpriteFrameNames;
		};
	}
}
