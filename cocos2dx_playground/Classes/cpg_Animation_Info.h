#pragma once

#include <utility>
#include <vector>

#include "cpg_Animation_Index.h"

namespace cpg
{
	namespace animation
	{
		struct Info
		{
			Info( cpg::animation::eIndex index, float delay, std::vector<std::string>&& sprite_frame_names )
				: Index( index ), delay( delay ), SpriteFrameNames( std::move( sprite_frame_names ) )
			{}

			cpg::animation::eIndex Index;
			float delay;
			std::vector<std::string> SpriteFrameNames;
		};
	}
}
