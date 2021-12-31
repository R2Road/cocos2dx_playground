#include "tool_practice_TileSheetTestConfiguration.h"

#include "platform\CCFileUtils.h"
#include "json/document.h"

namespace tool_practice
{
	TileSheetTestConfiguration::TileSheetTestConfiguration() :
		mLayerCount( 1 )
		, mWidth( 1 )
		, mHeight( 1 )
		, mTileSheetConfiguration()
	{}

	bool TileSheetTestConfiguration::Load( const char* config_file_path )
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

			mTileSheetConfiguration.Load( root_itr->value.GetString() );
		}

		return true;
	}
}
