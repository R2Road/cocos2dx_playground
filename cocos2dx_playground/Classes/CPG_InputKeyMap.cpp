#include "CPG_InputKeyMap.h"

#include <utility>
#include <fstream>

#include "platform\CCFileUtils.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		namespace
		{
			const char* string_key_code = "key_code";
			const char* string_idx = "idx";
			const bool loadKeyMapJson( const char* _key_map_path, KeyMap::KeyMapContainer& _container )
			{
				// load json
				const std::string regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile( _key_map_path );
				rapidjson::Document doc;
				doc.Parse<0>( regionStr.c_str() );

				if( doc.HasParseError() )
				{
					cocos2d::log( "json parse error" );
					return false;
				}

				if( doc.IsNull() )
				{
					cocos2d::log( "json is empty" );
					return false;
				}

				if( !doc.IsArray() )
				{
					cocos2d::log( "invalid data struct" );
					return false;
				}

				_container.reserve( doc.Size() );

				rapidjson::Value::MemberIterator key_code_itr;
				rapidjson::Value::MemberIterator idx_itr;
				for( auto cur = doc.Begin(); cur != doc.End(); ++cur )
				{
					key_code_itr = cur->FindMember( string_key_code );
					idx_itr = cur->FindMember( string_idx );
					if( key_code_itr == cur->MemberEnd() || idx_itr == cur->MemberEnd() )
						continue;

					_container.emplace_back( KeyMap::KeyMapPiece{
						static_cast<EventKeyboard::KeyCode>( key_code_itr->value.GetInt() )
						, idx_itr->value.GetInt()
					} );
				}

				return true;
			}

			const bool saveKeyMapJson( const char* _key_map_path, const KeyMap::KeyMapContainer& _container )
			{
				std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
				path.append( _key_map_path );

				rapidjson::Document document;
				document.SetArray();

				for( const auto k : _container )
				{
					rapidjson::Value val;
					val.SetObject();

					val.AddMember( rapidjson::Value::StringRefType( string_idx ), static_cast<int>( k.idx ), document.GetAllocator() );
					val.AddMember( rapidjson::Value::StringRefType( string_key_code ), static_cast<int>( k.keycode ),  document.GetAllocator() );

					document.PushBack( val, document.GetAllocator() );
				}

				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer( buffer );
				document.Accept( writer );

				std::ofstream fs( path.c_str(), std::ios::out );
				fs << buffer.GetString() << std::endl;
				fs.close();

				return true;
			}
		}

		KeyMap::KeyMap( KeyMapContainer&& _container ) : container( std::move( _container ) ) {}

		KeyMapSp KeyMap::create_with_json( const char* _key_map_path )
		{
			KeyMapContainer container;
			if( !loadKeyMapJson( _key_map_path, container ) )
				return get_dummy();

			KeyMapSp ret( new ( std::nothrow ) KeyMap( std::move( container ) ) );
			return ret;
		}

		KeyMapSp KeyMap::create( const char* _key_map_file_name )
		{
			std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
			path.append( _key_map_file_name );

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

		void KeyMap::save( const char* _key_map_path ) const
		{
			saveKeyMapJson( _key_map_path, container );
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