#pragma once

namespace cpg_animation
{
	enum class eIndex
	{
		none = 0,

		wait,
		wakeup,
		run,
		idle,
		attack_1,
		win,

		sleep,
		damaged_1,
		damaged_2,

		SIZE,
		FIRST = none,
	};
}
