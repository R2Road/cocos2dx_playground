#pragma once

#include "helper_SceneMover.h"

namespace helper
{
	class BackToThePreviousScene
	{
	protected:
		BackToThePreviousScene( const helper::FuncSceneMover& scene_mover ) : mSceneMover( scene_mover ) {}

	public:
		virtual ~BackToThePreviousScene() {}

	protected:
		void MoveBack()
		{
			mSceneMover();
		}

	private:
		const helper::FuncSceneMover mSceneMover;
	};
}
