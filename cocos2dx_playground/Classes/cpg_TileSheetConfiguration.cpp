#include "cpg_TileSheetConfiguration.h"

#include "platform\CCFileUtils.h"
#include "json/document.h"

namespace cpg
{
	bool TileSheetConfiguration::Load( const char* config_file_path )
	{
		//
		// Load Json
		//
		rapidjson::Document doc;
		{
			const std::string json_string( cocos2d::FileUtils::getInstance()->getStringFromFile( config_file_path ) );
			doc.Parse<0>( json_string.c_str() );
		}

		if( doc.HasParseError() )
		{
			CCLOG( "json parse error" );
			return false;
		}

		if( doc.IsNull() )
		{
			CCLOG( "json is empty" );
			return false;
		}

		const auto tile_w_itr = doc.FindMember( "tile_w" );
		const auto tile_h_itr = doc.FindMember( "tile_h" );
		const auto tile_margin_w_itr = doc.FindMember( "tile_margin_w" );
		const auto tile_margin_h_itr = doc.FindMember( "tile_margin_h" );
		const auto texture_itr = doc.FindMember( "texture" );
		if(
			doc.MemberEnd() == tile_w_itr
			|| doc.MemberEnd() == tile_h_itr
			|| doc.MemberEnd() == tile_margin_w_itr
			|| doc.MemberEnd() == tile_margin_h_itr
			|| doc.MemberEnd() == texture_itr
			)
		{
			CCLOG( "check property of sheet" );
			return false;
		}

		CCASSERT( 0 < tile_w_itr->value.GetInt(), "Invalid Tile Width" );
		CCASSERT( 0 < tile_h_itr->value.GetInt(), "Invalid Tile Height" );
		CCASSERT( 0 <= tile_margin_w_itr->value.GetInt(), "Invalid Tile Margin Width" );
		CCASSERT( 0 <= tile_margin_h_itr->value.GetInt(), "Invalid Tile Margin Height" );
		CCASSERT( 0 < texture_itr->value.GetStringLength(), "Invalid Texture Path" );
			
			
		TileWidth = tile_w_itr->value.GetInt();
		TileHeight = tile_h_itr->value.GetInt();
		TileMargin_Width = tile_margin_w_itr->value.GetInt();
		TileMargin_Height = tile_margin_h_itr->value.GetInt();

		BlockWidth = TileMargin_Width + TileWidth + TileMargin_Width;
		BlockHeight = TileMargin_Height + TileHeight + TileMargin_Height;

		TexturePath = texture_itr->value.GetString();

		return true;
	}
}