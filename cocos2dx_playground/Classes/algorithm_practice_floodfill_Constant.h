#pragma once

namespace algorithm_practice_floodfill
{
	enum eToolIndex
	{
		Wall,
		Road,
		Entry,
	};

	struct GridValue
	{
		enum class eType
		{
			Road,
			Wall,
		};

		eType Type = eType::Road;
	};
}
