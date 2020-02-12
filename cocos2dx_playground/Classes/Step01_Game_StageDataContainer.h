#pragma once

#include <string>
#include <vector>

namespace step01
{
	namespace game
	{
		class StageDataContainer
		{
		public:
			using Container = std::vector<std::string>;

			StageDataContainer();

			bool load();

			const std::string& get( const std::size_t stage_index ) const;

		private:
			Container mContainer;
		};
	} // namespace game
}
