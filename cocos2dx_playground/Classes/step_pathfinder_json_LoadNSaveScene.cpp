#include "step_pathfinder_json_LoadNSaveScene.h"

#include <new>
#include <numeric>
#include <random>
#include <utility>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/ccUTF8.h"
#include "platform/CCFileUtils.h"
#include "ui/UIButton.h"

#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "helper_Win32DirectoryOpen.h"

USING_NS_CC;

namespace
{
	const char* FilePath_Step_Pathfinder_Json_LoadNSave = "step_pathfinder_json_load_and_save.json";
}

namespace step_pathfinder
{
	namespace json
	{
		LoadNSaveScene::LoadNSaveScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* LoadNSaveScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) LoadNSaveScene( back_to_the_previous_scene_callback );
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
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

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();


			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( 0.f, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 58, 0, 61, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Target Path
			//
			{
				const float MAX_LINE_WIDTH = visibleSize.width * 0.8f;

				// Title
				{
					auto label = Label::createWithTTF( "<Target Path>", cpg::StringTable::GetFontPath(), 14 );
					label->setAnchorPoint( Vec2( 1.f, 0.f ) );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.8f )
					);
					addChild( label );

					// Open Folder
					if( helper::isEnableWin32DirectoryOpen() )
					{
						auto button = ui::Button::create( "guide_01_2.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
						button->setScale9Enabled( true );
						button->addTouchEventListener( []( Ref* /*sender*/, ui::Widget::TouchEventType touch_event_type ) {
							if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
							{
								return;
							}

							helper::Win32DirectoryOpen( FileUtils::getInstance()->getWritablePath().c_str() );
						} );
						addChild( button );
						{
							auto title_label = Label::createWithTTF( "Open Folder", cpg::StringTable::GetFontPath(), 12 );
							button->setTitleLabel( title_label );

							button->setContentSize( title_label->getContentSize() + Size( 10.f, 4.f ) + Size( 10.f, 4.f ) );
						}

						button->setPosition(
							label->getPosition()
							+ Vec2(
								button->getContentSize().width * 0.5f
								, button->getContentSize().height * 0.5f
							)
						);
					}
				}

				// Path
				{
					auto label = Label::createWithTTF( FileUtils::getInstance()->getWritablePath().c_str(), cpg::StringTable::GetFontPath(), 10 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setMaxLineWidth( MAX_LINE_WIDTH );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.8f )
					);
					addChild( label );
				}
			}

			//
			// File Name
			//
			{
				// Title
				{
					auto label = Label::createWithTTF( "<File Name>", cpg::StringTable::GetFontPath(), 14 );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.6f )
					);
					addChild( label );
				}

				// Name
				{
					auto label = Label::createWithTTF( FilePath_Step_Pathfinder_Json_LoadNSave, cpg::StringTable::GetFontPath(), 14 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.6f )
					);
					addChild( label );
				}
			}

			//
			// Json Save And Load
			//
			std::string json_string;
			ContainerT json_datas;
			{
				std::string path( std::move( FileUtils::getInstance()->getWritablePath() ) );
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
					auto label = Label::createWithTTF( "<Json File Content String>", cpg::StringTable::GetFontPath(), 14 );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.4f )
					);
					addChild( label );
				}

				// Content
				{
					auto label = Label::createWithTTF( json_string.c_str(), cpg::StringTable::GetFontPath(), 14 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.4f )
					);
					addChild( label );
				}
			}

			//
			// Parsed Json Data View
			//
			{
				// Title
				{
					auto label = Label::createWithTTF( "<Parsed Json Data>", cpg::StringTable::GetFontPath(), 14 );
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.2f )
					);
					addChild( label );
				}

				// Path
				{
					std::stringstream ss;
					for( auto i : json_datas )
					{
						ss << "   " << i;
					}

					auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 14 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.2f )
					);
					addChild( label );
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

			Scene::onExit();
		}

		void LoadNSaveScene::makeDummyJsonFile( const char* json_path ) const
		{
			if( FileUtils::getInstance()->isFileExist( json_path ) )
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

			FileUtils::getInstance()->writeStringToFile( buffer.GetString(), json_path );
		}
		bool LoadNSaveScene::loadJsonFile( const char* json_path, std::string& out_json_string, ContainerT& out_json_datas )
		{
			out_json_string = std::move( FileUtils::getInstance()->getStringFromFile( json_path ) );

			rapidjson::Document doc;
			doc.Parse<0>( out_json_string.c_str() );

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
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
