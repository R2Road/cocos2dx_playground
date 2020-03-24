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

			bool Load();

			const std::string& Get( const std::size_t stage_index ) const;
			const Container::size_type Size() const { return mContainer.size(); }

		private:
			Container mContainer;
		};
	} // namespace game
}
