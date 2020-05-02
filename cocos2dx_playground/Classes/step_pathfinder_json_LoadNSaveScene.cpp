#include "step_pathfinder_json_LoadNSaveScene.h"

#include <new>
#include <numeric>
#include <sstream>
#include <random>
#include <fstream>
#include <utility>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "platform/CCFileUtils.h"
#include "base/ccUTF8.h"

#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "step_pathfinder_RootScene.h"

USING_NS_CC;

namespace
{
	const char* FilePath_Step_Pathfinder_Json_LoadNSave = "step_pathfinder_json_load_and_save.json";
}

namespace step_pathfinder
{
	namespace json
	{
		LoadNSaveScene::LoadNSaveScene() : mKeyboardListener( nullptr ) {}

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
			{
				return false;
			}

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

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
			// Json Data Path View
			//
			{
				// Title
				{
					auto label = Label::createWithTTF( "<Json File Path>", "fonts/arial.ttf", 14 );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setPosition( Vec2(
						visibleOrigin.x + ( visibleSize.width * 0.5f )
						, visibleOrigin.y + ( visibleSize.height * 0.7f )
					) );
					addChild( label );
				}

				// Path
				{
					auto label = Label::createWithTTF(
						StringUtils::format( "%s%s", FileUtils::getInstance()->getWritablePath().c_str(), FilePath_Step_Pathfinder_Json_LoadNSave )
						, "fonts/arial.ttf"
						, 9
					);
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition( Vec2(
						visibleOrigin.x + ( visibleSize.width * 0.5f )
						, visibleOrigin.y + ( visibleSize.height * 0.7f )
					) );
					addChild( label );
				}
			}

			//
			// Json Save And Load
			//
			std::string json_string;
			ContainerT json_datas;
			{
				std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
				path.append( FilePath_Step_Pathfinder_Json_LoadNSave );

				makeDummyJsonFile( path.c_str() );
				loadJsonFile( path.c_str(), json_string, json_datas );
			}

			//
			// Json File Content String View
			//
			{
				// Title
				{
					auto label = Label::createWithTTF( "<Json File Content String>", "fonts/arial.ttf", 14 );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setPosition( Vec2(
						visibleOrigin.x + ( visibleSize.width * 0.5f )
						, visibleOrigin.y + ( visibleSize.height * 0.5f )
					) );
					addChild( label );
				}

				// Content
				{
					auto label = Label::createWithTTF( json_string.c_str(), "fonts/arial.ttf", 14 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition( Vec2(
						visibleOrigin.x + ( visibleSize.width * 0.5f )
						, visibleOrigin.y + ( visibleSize.height * 0.5f )
					) );
					addChild( label, 0 );
				}
			}

			//
			// Parsed Json Data View
			//
			{
				// Title
				{
					auto label = Label::createWithTTF( "<Parsed Json Data>", "fonts/arial.ttf", 14 );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setPosition( Vec2(
						visibleOrigin.x + ( visibleSize.width * 0.5f )
						, visibleOrigin.y + ( visibleSize.height * 0.3f )
					) );
					addChild( label );
				}

				// Path
				{
					std::stringstream ss;
					for( auto i : json_datas )
					{
						ss << "   " << i;
					}

					auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 14 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition( Vec2(
						visibleOrigin.x + ( visibleSize.width * 0.5f )
						, visibleOrigin.y + ( visibleSize.height * 0.3f )
					) );
					addChild( label, 0 );
				}
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

		void LoadNSaveScene::makeDummyJsonFile( const char* json_path ) const
		{
			if( cocos2d::FileUtils::getInstance()->isFileExist( json_path ) )
			{
				return;
			}

			std::random_device rd;
			std::mt19937 randomEngine( rd() );
			std::uniform_int_distribution<> dist( 0, 9 );

			ContainerT datas;
			datas.reserve( 10 );
			for( int i = 0; i < 10; ++i )
			{
				datas.emplace_back( dist( randomEngine ) );
			}

			saveJsonFile( json_path, datas );
		}
		void LoadNSaveScene::saveJsonFile( const char* json_path, const ContainerT& json_datas ) const
		{
			rapidjson::Document document;
			document.SetArray();

			for( const auto d : json_datas )
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
		bool LoadNSaveScene::loadJsonFile( const char* json_path, std::string& out_json_string, ContainerT& out_json_datas )
		{
			out_json_string = std::move( cocos2d::FileUtils::getInstance()->getStringFromFile( json_path ) );

			rapidjson::Document doc;
			doc.Parse<0>( out_json_string.c_str() );

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

			out_json_datas.clear();
			out_json_datas.reserve( doc.Size() );
			for( rapidjson::SizeType i = 0u; i < doc.Size(); ++i )
			{
				const auto& value = doc[i];

				out_json_datas.emplace_back( value.GetInt() );
			}

			return true;
		}

		void LoadNSaveScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( step_pathfinder::RootScene::create() );
				return;
			}
		}
	}
}
