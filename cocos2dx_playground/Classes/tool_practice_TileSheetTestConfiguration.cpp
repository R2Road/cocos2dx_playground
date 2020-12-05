#include "tool_practice_TileSheetTestConfiguration.h"

#include "platform\CCFileUtils.h"
#include "json/document.h"

namespace tool_practice
{
	TileSheetTestConfiguration::TileSheetTestConfiguration() :
		mLayerCount( 1 )
		, mWidth( 10 )
		, mHeight( 10 )
		, mTileSheetConfiguration( 16, 16, 1, 1, "textures/step_defender/step_defender_tilesheet_01.png" )
	{}

	bool TileSheetTestConfiguration::Load()
	{
		//
		// Load Json
		//
		rapidjson::Document doc;
		{
			const std::string json_string( cocos2d::FileUtils::getInstance()->getStringFromFile( "datas/tool_practice/tile_sheet_test.json" ) );
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

		rapidjson::Value::MemberIterator root_itr;

		//
		// Layer
		//
		{
			root_itr = doc.FindMember( "layer" );
			if( doc.MemberEnd() == root_itr )
			{
				CCLOG( "layer is empty" );
				return false;
			}

			mLayerCount = root_itr->value.GetInt();

			CCASSERT( 0 < mLayerCount, "Invalid Layer Count" );
		}

		//
		// Size
		//
		{
			root_itr = doc.FindMember( "size" );
			if( doc.MemberEnd() == root_itr )
			{
				CCLOG( "size is empty" );
				return false;
			}

			const auto w_itr = root_itr->value.FindMember( "w" );
			const auto h_itr = root_itr->value.FindMember( "h" );
			if(
				root_itr->value.MemberEnd() == w_itr
				|| root_itr->value.MemberEnd() == h_itr
			)
			{
				CCLOG( "check property of size" );
				return false;
			}

			mWidth = w_itr->value.GetInt();
			mHeight = h_itr->value.GetInt();

			CCASSERT( 0 < mWidth, "Invalid Width" );
			CCASSERT( 0 < mHeight, "Invalid Height" );
		}

		//
		// Sheet
		//
		{
			root_itr = doc.FindMember( "sheet" );
			if( doc.MemberEnd() == root_itr )
			{
				CCLOG( "sheet is empty" );
				return false;
			}

			const auto tile_w_itr = root_itr->value.FindMember( "tile_w" );
			const auto tile_h_itr = root_itr->value.FindMember( "tile_h" );
			const auto tile_margin_w_itr = root_itr->value.FindMember( "tile_margin_w" );
			const auto tile_margin_h_itr = root_itr->value.FindMember( "tile_margin_h" );
			const auto texture_itr = root_itr->value.FindMember( "texture" );
			if(
				root_itr->value.MemberEnd() == tile_w_itr
				|| root_itr->value.MemberEnd() == tile_h_itr
				|| root_itr->value.MemberEnd() == tile_margin_w_itr
				|| root_itr->value.MemberEnd() == tile_margin_h_itr
				|| root_itr->value.MemberEnd() == texture_itr
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

			mTileSheetConfiguration = step_defender::game::TileSheetConfiguration{
				tile_w_itr->value.GetInt()
				, tile_h_itr->value.GetInt()
				, tile_margin_w_itr->value.GetInt()
				, tile_margin_h_itr->value.GetInt()
				, texture_itr->value.GetString()
			};
		}

		return true;
	}
}
