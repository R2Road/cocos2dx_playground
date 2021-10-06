#include "cpg_input_KeyMap.h"

#include <utility>

#include "json/document.h"
#include "platform\CCFileUtils.h"

USING_NS_CC;

namespace
{
	const char* string_key_code = "key_code";
	const bool loadKeyMapJson( const char* key_map_path, cpg_input::KeyMap::KeyMapContainer& container )
	{
		// load json
		rapidjson::Document doc;
		{
			const std::string json_string( FileUtils::getInstance()->getStringFromFile( key_map_path ) );
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

		if( !doc.IsArray() )
		{
			CCLOG( "invalid data struct" );
			return false;
		}

		container.reserve( doc.Size() );

		rapidjson::Value::MemberIterator key_code_itr;
		int key_idx = 0;
		for( auto cur = doc.Begin(); cur != doc.End(); ++cur )
		{
			key_code_itr = cur->FindMember( string_key_code );
			if( key_code_itr == cur->MemberEnd() )
			{
				continue;
			}

			container.emplace_back( static_cast<EventKeyboard::KeyCode>( key_code_itr->value.GetInt() ), key_idx );

			++key_idx;
		}

		return true;
	}
}

namespace cpg_input
{
	KeyMap::KeyMap( KeyMapContainer&& container ) : mContainer( std::move( container ) ) {}

	KeyMapSp KeyMap::create_with_json( const char* key_map_path )
	{
		KeyMapContainer container;
		if( !loadKeyMapJson( key_map_path, container ) )
		{
			return get_dummy();
		}

		KeyMapSp ret( new ( std::nothrow ) KeyMap( std::move( container ) ) );
		return ret;
	}

	KeyMapSp KeyMap::create( const char* key_map_file_name )
	{
		std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
		path.append( key_map_file_name );

		return create_with_json( path.c_str() );
	}

	const KeyMapSp& KeyMap::get_default()
	{
		static const auto default = create_with_json( "datas/keyconfig/keymap_default.json" );
		return default;
	}

	const KeyMapSp& KeyMap::get_dummy()
	{
		static const KeyMapSp ret( new ( std::nothrow ) KeyMap( std::move( KeyMapContainer( { KeyMapPiece{ cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE, 0 } } ) ) ) );
		return ret;
	}

	int KeyMap::getKeyIndex( const cocos2d::EventKeyboard::KeyCode key_code ) const
	{
		for( const auto& k : mContainer )
		{
			if( k.keycode == key_code )
			{
				return k.idx;
			}
		}

		return 0;
	}
}