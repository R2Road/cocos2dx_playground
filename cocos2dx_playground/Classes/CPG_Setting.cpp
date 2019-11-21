#include "CPG_Setting.h"

#include "platform\CCFileUtils.h"
#include "json/document.h"

namespace CPG
{
	void Setting::load( cocos2d::Size& frame_resolution, cocos2d::Size& design_resolution )
	{
		// default
		frame_resolution.setSize( 1024, 768 );
		design_resolution.setSize( 480, 320 );

		// load json
		const std::string& regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile( "config/setting.json" );
		rapidjson::Document doc;
		doc.Parse<0>( regionStr.c_str() );

		if( doc.HasParseError() )
		{
			cocos2d::log( "json parse error" );
			return;
		}

		if( doc.IsNull() )
		{
			cocos2d::log( "json is empty" );
			return;
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
	}
}