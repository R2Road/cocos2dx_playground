#include "CPG_InputKeyMap.h"

#include <utility>

#include "platform\CCFileUtils.h"
#include "json/document.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		KeyMap::KeyMap( KeyMapContainer&& _container ) : container( std::move( _container ) ) {}

		KeyMapSp KeyMap::create( const char* _key_map_path )
		{
			// load json
			const std::string regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile( _key_map_path );
			rapidjson::Document doc;
			doc.Parse<0>( regionStr.c_str() );

			if( doc.HasParseError() )
			{
				cocos2d::log( "json parse error" );
				return get_dummy();
			}

			if( doc.IsNull() )
			{
				cocos2d::log( "json is empty" );
				return get_dummy();
			}

			KeyMapContainer container;
			container.reserve( doc.Size() );

			for( auto cur = doc.Begin(); cur != doc.End(); ++cur )
				container.emplace_back( KeyMapPiece{ static_cast<EventKeyboard::KeyCode>(
					(*cur)["key_code"].GetInt() )
					, ( *cur )["idx"].GetInt()
				} );

			KeyMapSp ret( new ( std::nothrow ) KeyMap( std::move( container ) ) );
			return ret;
		}

		const KeyMapSp& KeyMap::get_dummy()
		{
			static const KeyMapSp ret( new ( std::nothrow ) KeyMap( std::move( KeyMapContainer( { KeyMapPiece{ cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE, 0 } } ) ) ) );
			return ret;
		}

		const int KeyMap::getKeyIndex( const cocos2d::EventKeyboard::KeyCode _key_code ) const
		{
			for( const auto& k : container )
				if( k.keycode == _key_code )
					return k.idx;

			return 0;
		}
	}
}