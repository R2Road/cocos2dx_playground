#pragma once

#include <string>
#include <vector>

namespace step_pathfinder
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
			const Container::size_type size() const { return mContainer.size(); }

		private:
			Container mContainer;
		};
	} // namespace game
}
