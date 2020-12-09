#include "input_practice_Setting.h"

#include "platform\CCFileUtils.h"
#include "json/document.h"

namespace input_practice
{
	Setting Setting::instance;

	Setting::Setting() :
		mFileName_for_KeyAllow( "r_k_a.json" )
		, mFileName_for_KeyMap( "r_k_m.json" )
	{}

	void Setting::load()
	{
		// load json
		const std::string& regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile( "config/research_setting.json" );
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

		const auto key_allow_file_itr = doc.FindMember( "key_allow_file" );
		if( doc.MemberEnd() != key_allow_file_itr )
			instance.mFileName_for_KeyAllow = key_allow_file_itr->value.GetString();

		const auto key_map_file_itr = doc.FindMember( "key_map_file" );
		if( doc.MemberEnd() != key_map_file_itr )
			instance.mFileName_for_KeyMap = key_map_file_itr->value.GetString();
	}
}