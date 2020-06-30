#pragma once

#include "2d/CCNode.h"

namespace step_rain_of_chaos
{
	class BackgroundNode : public cocos2d::Node
	{
	private:
		BackgroundNode();

	public:
		static BackgroundNode* create( const std::size_t vertical_amount, const std::size_t horizontal_amount, const char* texture_path);

		bool init( const std::size_t vertical_amount, const std::size_t horizontal_amount, const char* texture_path );
	};
}
