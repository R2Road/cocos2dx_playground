#pragma once

#include <string>

#include "2d/CCNode.h"

namespace step_defender
{
	namespace tool
	{
		class TileSheetNode : public cocos2d::Node
		{
		public:
			struct Config
			{
				int TileWidth = 32;
				int TileHeight = 32;

				std::string TexturePath;
			};

		private:
			TileSheetNode( const Config& config );

		public:
			static TileSheetNode* create( const Config& config );

		private:
			bool init() override;

		private:
			const Config mConfig;
		};
	}
}
