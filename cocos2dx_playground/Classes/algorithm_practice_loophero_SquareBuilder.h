#pragma once

#include <list>

#include "cpg_Point.h"

namespace algorithm_practice_loophero
{
	class SquareBuilder
	{
	public:
		using ContainerT = std::list<cpg::Point>;

	private:
		SquareBuilder() = delete;

	public:
		static cpg::Point Build( ContainerT* out_container );
	};
}
