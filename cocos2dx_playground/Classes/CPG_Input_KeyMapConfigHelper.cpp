#include "CPG_Input_KeyMapConfigHelper.h"

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
			const char* string_name = "name";
			const char* string_empty = "o_o";
		}

		KeyMapConfigHelper::KeyMapConfigHelper() : container() {}

		const bool KeyMapConfigHelper::load( const char* _key_map_file_name )
		{
			if( !load_Resource() )
			{
				cocos2d::log( "Failed : KeyMapConfigHelper - load_Resource()" );
				return false;
			}

			std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
			path.append( _key_map_file_name );

			if( load_Json( path.c_str() ) )
				return true;

			// load default
			if( load_Json( "datas/keyconfig/keymap_default.json" ) )
			{
				save_Json( path.c_str() );
				return true;
			}

			return false;
		}
		void KeyMapConfigHelper::save( const char* _key_map_file_name )
		{
			std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
			path.append( _key_map_file_name );

			save_Json( path.c_str() );
		}

		void KeyMapConfigHelper::set( const int _key_index, const cocos2d::EventKeyboard::KeyCode _new_keycode )
		{
			if( 0 > _key_index || _key_index >= container.size() )
				return;

			container[static_cast<std::size_t>( _key_index )].keycode = _new_keycode;
		}

		const bool KeyMapConfigHelper::load_Resource()
		{
			// load json
			const std::string jsonStr( std::move( cocos2d::FileUtils::getInstance()->getStringFromFile( "datas/keyconfig/keymap_resource_for_config.json" ) ) );
			rapidjson::Document doc;
			doc.Parse<0>( jsonStr.c_str() );

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

			container.reserve( doc.Size() );

			rapidjson::Value::MemberIterator name_itr;
			int key_idx = 0;
			for( auto cur = doc.Begin(); cur != doc.End(); ++cur, ++key_idx )
			{
				name_itr = cur->FindMember( string_name );
				if( name_itr == cur->MemberEnd() )
				{
					container.emplace_back( "-", key_idx, EventKeyboard::KeyCode::KEY_NONE );
					continue;
				}

				container.emplace_back( name_itr->value.GetString(), key_idx, EventKeyboard::KeyCode::KEY_NONE );
			}

			return true;
		}
		const bool KeyMapConfigHelper::load_Json( const char* _key_map_path )
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

			rapidjson::Value::MemberIterator key_code_itr;
			auto keymap_cur = container.begin();
			const auto keymap_end = container.end();
			for( auto cur = doc.Begin();
				cur != doc.End() && keymap_cur != keymap_end;
				++cur, ++keymap_cur )
			{
				key_code_itr = cur->FindMember( string_key_code );
				if( key_code_itr == cur->MemberEnd() )
					continue;

				keymap_cur->keycode = static_cast<EventKeyboard::KeyCode>( key_code_itr->value.GetInt() );
			}

			return true;
		}

		void KeyMapConfigHelper::save_Json( const char* _key_map_path ) const
		{
			rapidjson::Document document;
			document.SetArray();

			for( const auto h : container )
			{
				rapidjson::Value val;
				val.SetObject();

				val.AddMember( rapidjson::Value::StringRefType( string_key_code ), static_cast<int>( h.keycode ), document.GetAllocator() );

				document.PushBack( val, document.GetAllocator() );
			}

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer( buffer );
			document.Accept( writer );

			std::ofstream fs( _key_map_path, std::ios::out );
			fs << buffer.GetString() << std::endl;
			fs.close();
		}
	}
}