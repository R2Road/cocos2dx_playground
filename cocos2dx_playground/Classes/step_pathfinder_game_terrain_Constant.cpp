#include "step_pathfinder_game_terrain_Constant.h"

#include <array>

#include "2d/CCLabel.h"

namespace step_pathfinder
{
	namespace game
	{
		namespace terrain
		{
			const std::array<TileData, static_cast<std::size_t>( eTileType::SIZE )> TileTable = { {
				{ eTileType::damage, true, false, "step_pathfinder_tile_damage.png", "Damage Tile" }
				,{ eTileType::road, true, false, "step_pathfinder_tile_road.png", "Road Tile" }
				,{ eTileType::entrance, true, true, "step_pathfinder_tile_entrance.png", "Entrance" }
				,{ eTileType::exit, true, true, "step_pathfinder_tile_exit.png", "Exit" }
				,{ eTileType::magic_circle_on, true, true, "step_pathfinder_tile_magic_circle_01.png", "Switch" }
				,{ eTileType::magic_circle_off, false, true, "step_pathfinder_tile_magic_circle_02.png", "" }
			} };

			const TileData& TileType2TileData( const eTileType tile_type )
			{
				if( eTileType::FIRST > tile_type || eTileType::SIZE <= tile_type )
				{
					static const TileData dummy{ eTileType::damage, false, false, "step_pathfinder_tile_damage.png", "W T F" };
					return dummy;
				}

				return TileTable[static_cast<std::size_t>( tile_type )];
			}
			bool TileType2UniqueFlag( const eTileType tile_type )
			{
				if( eTileType::FIRST > tile_type || eTileType::SIZE <= tile_type )
				{
					return false;
				}

				return TileTable[static_cast<std::size_t>( tile_type )].bUnique;
			}

			cocos2d::Size GetMaxMenuSize()
			{
				auto label = cocos2d::Label::createWithTTF( "", "fonts/arial.ttf", 9 );

				const cocos2d::Size button_margin( 10.f, 4.f );
				cocos2d::Size max_size;
				for( const auto& t : TileTable )
				{
					label->setString( t.Name );

					max_size.width = max_size.width < label->getContentSize().width ? label->getContentSize().width : max_size.width;
					max_size.height = max_size.height < label->getContentSize().height ? label->getContentSize().height : max_size.height;
				}

				max_size = button_margin + max_size + button_margin;

				return max_size;
			}
		}
	}
}
