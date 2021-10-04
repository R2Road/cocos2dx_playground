#pragma once

#include "cocos/math/CCGeometry.h"

namespace cpg
{
	class Setting
	{
	public:
		struct Data
		{
			cocos2d::Size mFrameResolution = { 1024, 768 };
			cocos2d::Size mDesignResolution = { 480, 320 };
			bool mShowDisplayStats = true;
		};

	private:
		Setting() = delete;

	public:
		static const Data Load();
	};
}