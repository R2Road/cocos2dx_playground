#include "Step01_Json_LoadNSaveScene.h"

#include <new>
#include <numeric>
#include <sstream>
#include <random>
#include <fstream>
#include <utility>

#include "base/ccUTF8.h"

#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "Step01_RootScene.h"

USING_NS_CC;

namespace step01
{
	namespace json
	{
		namespace
		{
			const char* FilePath_Step01_Json_LoadNSave = "step01_json_load_and_save.json";
		}

		LoadNSaveScene::LoadNSaveScene() : mKeyboardListener( nullptr ), mJsonString(), mDatas() {}

		Scene* LoadNSaveScene::create()
		{
			auto ret = new ( std::nothrow ) LoadNSaveScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool LoadNSaveScene::init()
		{
			if( !Scene::init() )
				return false;

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();


			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << std::endl;
				ss << "<Json File Path> : " << cocos2d::FileUtils::getInstance()->getWritablePath() << FilePath_Step01_Json_LoadNSave;

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 58, 0, 61, 255 ) );
				addChild( background_layer, -1 );
			}

			//
			// Json Save And Load
			//
			{
				std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
				path.append( FilePath_Step01_Json_LoadNSave );

				makeDummyJsonFile( path.c_str() );
				loadJsonFile( path.c_str() );
			}

			//
			// Json View
			//
			{
				auto label = Label::createWithTTF(
					StringUtils::format( "Json String : %s", mJsonString.c_str() )
					, "fonts/arial.ttf"
					, 9
				);
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.6f )
				) );
				addChild( label, 0 );
			}

			//
			// Data View
			//
			{
				std::stringstream ss;
				ss << "Json Datas :";
				for( auto i : mDatas )
				{
					ss << " " << i;
				}

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9 );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.4f )
				) );
				addChild( label, 0 );
			}

			return true;
		}

		void LoadNSaveScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( LoadNSaveScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void LoadNSaveScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void LoadNSaveScene::makeDummyJsonFile( const char* json_path )
		{
			if( cocos2d::FileUtils::getInstance()->isFileExist( json_path ) )
			{
				return;
			}

			std::random_device rd;
			std::mt19937 randomEngine( rd() );
			std::uniform_int_distribution<> dist( 0, 9 );

			mDatas.reserve( 10 );
			for( int i = 0; i < 10; ++i )
			{
				mDatas.emplace_back( dist( randomEngine ) );
			}

			saveJsonFile( json_path );
		}
		void LoadNSaveScene::saveJsonFile( const char* json_path )
		{
			rapidjson::Document document;
			document.SetArray();

			for( const auto d : mDatas )
			{
				document.PushBack( d, document.GetAllocator() );
			}

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer( buffer );
			document.Accept( writer );

			std::ofstream fs( json_path, std::ios::out );
			fs << buffer.GetString() << std::endl;
			fs.close();
		}
		bool LoadNSaveScene::loadJsonFile( const char* json_path )
		{
			mJsonString = std::move( cocos2d::FileUtils::getInstance()->getStringFromFile( json_path ) );

			rapidjson::Document doc;
			doc.Parse<0>( mJsonString.c_str() );

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

			mDatas.clear();
			mDatas.reserve( doc.Size() );
			for( rapidjson::SizeType i = 0u; i < doc.Size(); ++i )
			{
				const auto& value = doc[i];

				mDatas.emplace_back( value.GetInt() );
			}

			return true;
		}

		void LoadNSaveScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( RootScene::create() );
				return;
			}
		}
	}
}
