#pragma once

namespace cpg
{
	namespace animation
	{
		enum class eIndex
		{
			none = 0,

			wait,
			wakeup,
			run,
			idle,
			win,

			sleep,
			damaged_1,
			damaged_2,

			SIZE,
			FIRST = none,
		};
	}
}
