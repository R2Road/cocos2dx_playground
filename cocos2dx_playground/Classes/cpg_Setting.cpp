#include "cpg_Setting.h"

#include "platform\CCFileUtils.h"
#include "json/document.h"

namespace cpg
{
	Setting::Setting( const cocos2d::Size frame_resolution, const cocos2d::Size design_resolution, const bool show_display_stats ) :
		mFrameResolution( frame_resolution )
		, mDesignResolution( design_resolution )
		, mShowDisplayStats( show_display_stats )
	{}

	const Setting Setting::load()
	{
		// default
		cocos2d::Size frame_resolution( 1024, 768 );
		cocos2d::Size design_resolution( 480, 320 );
		bool show_display_stats = true;

		// load json
		const std::string& regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile( "config/setting.json" );
		rapidjson::Document doc;
		doc.Parse<0>( regionStr.c_str() );

		if( doc.HasParseError() )
		{
			cocos2d::log( "json parse error" );
			return Setting( frame_resolution, design_resolution, show_display_stats );
		}

		if( doc.IsNull() )
		{
			cocos2d::log( "json is empty" );
			return Setting( frame_resolution, design_resolution, show_display_stats );
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
				frame_resolution.setSize( x_itr->value.GetInt(), y_itr->value.GetInt() );
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
				design_resolution.setSize( x_itr->value.GetInt(), y_itr->value.GetInt() );
		}

		const auto display_stats_itr = doc.FindMember( "display_stats" );
		if( doc.MemberEnd() != display_stats_itr )
			show_display_stats = display_stats_itr->value.GetBool();

		return Setting( frame_resolution, design_resolution, show_display_stats );
	}
}