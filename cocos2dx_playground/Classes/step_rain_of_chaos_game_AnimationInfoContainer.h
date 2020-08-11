#pragma once

#include "cpg_animation_InfoContainer.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		const cpg::animation::InfoContainerT& GetActorAnimationInfoContainer();

		const cpg::animation::InfoContainerT& GetEnemyAnimationInfoContainer();

		const cpg::animation::InfoContainerT& GetBulletAnimationInfoContainer();
	}
}
