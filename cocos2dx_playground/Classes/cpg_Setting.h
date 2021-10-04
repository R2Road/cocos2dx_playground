#pragma once

#include "cocos/math/CCGeometry.h"

namespace cpg
{
	class Setting
	{
	private:
		explicit Setting( const cocos2d::Size frame_resolution, const cocos2d::Size design_resolution, const bool show_display_stats );

	public:
		static const Setting Load();

		const cocos2d::Size mFrameResolution;
		const cocos2d::Size mDesignResolution;
		const bool mShowDisplayStats;
	};
}