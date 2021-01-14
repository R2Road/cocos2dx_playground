#pragma once

namespace algorithm_practice_floodfill
{
	enum eToolIndex
	{
		Wall,
		Road,
		Entry,
	};

	enum class eGridType
	{
		Road,
		Wall,
	};

	struct GridValue
	{
		eGridType Type = eGridType::Road;
	};
}
