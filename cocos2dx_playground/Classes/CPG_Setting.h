#pragma once

#include "cocos/math/CCGeometry.h"

namespace CPG
{
	class Setting
	{
	private:
		explicit Setting( const cocos2d::Size _frame_resolution, const cocos2d::Size _design_resolution );

	public:
		static const Setting load();

		const cocos2d::Size frame_resolution;
		const cocos2d::Size design_resolution;
	};
}