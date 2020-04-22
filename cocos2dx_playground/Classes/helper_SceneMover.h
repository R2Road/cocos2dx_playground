#pragma once

#include <functional>

#include "base/CCDirector.h"

namespace helper
{
	using FuncSceneMover = std::function<void()>;

	template<typename SceneToGoBackT>
	FuncSceneMover CreateSceneMover()
	{
		FuncSceneMover ret = []()
		{
			cocos2d::Director::getInstance()->replaceScene( SceneToGoBackT::create() );
		};
		return ret;
	}
}
