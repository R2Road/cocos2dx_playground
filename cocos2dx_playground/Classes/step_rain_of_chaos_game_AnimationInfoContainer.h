#pragma once

#include "cpg_animation_InfoContainer.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		const cpg_animation::InfoContainerT& GetActorAnimationInfoContainer();

		const cpg_animation::InfoContainerT& GetEnemyAnimationInfoContainer();

		const cpg_animation::InfoContainerT& GetBulletAnimationInfoContainer();
	}
}
