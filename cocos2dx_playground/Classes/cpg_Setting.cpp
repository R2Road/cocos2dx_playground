#include "cpg_Setting.h"

#include "platform\CCFileUtils.h"
#include "json/document.h"

namespace cpg
{
	const Setting::Data Setting::Load()
	{
		Data ret;

		// load json
		rapidjson::Document doc;
		{
			const auto json_string( std::move( cocos2d::FileUtils::getInstance()->getStringFromFile( "config/setting.json" ) ) );
			doc.Parse<0>( json_string.c_str() );
		}

		if( doc.HasParseError() )
		{
			cocos2d::log( "json parse error" );
			return ret;
		}

		if( doc.IsNull() )
		{
			cocos2d::log( "json is empty" );
			return ret;
		}

		const auto frame_resolution_itr = doc.FindMember( "frame_resolution" );
		if( doc.MemberEnd() != frame_resolution_itr )
		{
			const auto x_itr = frame_resolution_itr->value.FindMember( "x" );
			const auto y_itr = frame_resolution_itr->value.FindMember( "y" );
			if( frame_resolution_itr->value.MemberEnd() != x_itr
				&& x_itr->value.IsInt()
				&& frame_resolution_itr->value.MemberEnd() != y_itr
				&& y_itr->value.IsInt() )
			{
				ret.mFrameResolution.setSize( x_itr->value.GetInt(), y_itr->value.GetInt() );
			}
		}

		const auto design_resolution_itr = doc.FindMember( "design_resolution" );
		if( doc.MemberEnd() != design_resolution_itr )
		{
			const auto x_itr = design_resolution_itr->value.FindMember( "x" );
			const auto y_itr = design_resolution_itr->value.FindMember( "y" );
			if( design_resolution_itr->value.MemberEnd() != x_itr
				&& x_itr->value.IsInt()
				&& frame_resolution_itr->value.MemberEnd() != y_itr
				&& y_itr->value.IsInt() )
			{
				ret.mDesignResolution.setSize( x_itr->value.GetInt(), y_itr->value.GetInt() );
			}
		}

		const auto display_stats_itr = doc.FindMember( "display_stats" );
		if( doc.MemberEnd() != display_stats_itr )
		{
			ret.mShowDisplayStats = display_stats_itr->value.GetBool();
		}

		return ret;
	}
}