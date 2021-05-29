#include "step_pathfinder_fileutil_GenerateNDeleteScene.h"

#include <new>
#include <numeric>
#include <string>
#include <utility>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"
#include "platform/CCFileUtils.h"
#include "ui/UIButton.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "helper_Win32DirectoryOpen.h"

USING_NS_CC;

namespace
{
	const int TAG_FileStatus = 20140416;

	const char* FilePath_for_Generate_N_Delete = "step_pathfinder_fileutil_GenerateNDeleteScene.txt";
}

namespace step_pathfinder
{
	namespace fileutil
	{
		GenerateNDeleteScene::GenerateNDeleteScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback, std::string&& file_full_path ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mFileFullPath( std::move( file_full_path ) )
		{}

		Scene* GenerateNDeleteScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto file_full_path = std::move( FileUtils::getInstance()->getWritablePath() );
			file_full_path.append( FilePath_for_Generate_N_Delete );

			auto ret = new ( std::nothrow ) GenerateNDeleteScene( back_to_the_previous_scene_callback, std::move( file_full_path ) );
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

		bool GenerateNDeleteScene::init()
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

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 41, 0, 61, 255 ) );
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
					label->setAnchorPoint( Vec2( 0.f, 0.f ) );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.1f, visibleSize.height * 0.8f )
					);
					addChild( label );

					// Open Folder
					if( helper::isEnableWin32DirectoryOpen() )
					{
						auto button = ui::Button::create( "guide_01_2.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
						button->setScale9Enabled( true );
						button->addTouchEventListener( []( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type ) {
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
								label->getContentSize().width + 4.f + ( button->getContentSize().width * 0.5f )
								, button->getContentSize().height * 0.5f
							)
						);
					}
				}

				// Path
				{
					auto label = Label::createWithTTF( cocos2d::FileUtils::getInstance()->getWritablePath().c_str(), cpg::StringTable::GetFontPath(), 10 );
					label->setAnchorPoint( Vec2( 0.f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setMaxLineWidth( MAX_LINE_WIDTH );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.1f, visibleSize.height * 0.8f )
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
					label->setAnchorPoint( Vec2( 0.f, 0.f ) );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.1f, visibleSize.height * 0.6f )
					);
					addChild( label );
				}

				// Name
				{
					auto label = Label::createWithTTF( FilePath_for_Generate_N_Delete, cpg::StringTable::GetFontPath(), 14 );
					label->setAnchorPoint( Vec2( 0.f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.1f, visibleSize.height * 0.6f )
					);
					addChild( label );
				}
			}

			//
			// File Status
			//
			{
				// Title
				{
					auto label = Label::createWithTTF( "<File Status>", cpg::StringTable::GetFontPath(), 14 );
					label->setAnchorPoint( Vec2( 0.f, 0.f ) );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.1f, visibleSize.height * 0.4f )
					);
					addChild( label );
				}

				// Status
				{
					auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 14 );
					label->setTag( TAG_FileStatus );
					label->setAnchorPoint( Vec2( 0.f, 1.f ) );
					label->setColor( Color3B::RED );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.1f, visibleSize.height * 0.4f )
					);
					addChild( label );
				}

				updateFileStatus();
			}

			const Size ButtonMargin( 20.f, 12.f );

			//
			// Generate
			//
			{
				auto button = ui::Button::create( "guide_01_2.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->addTouchEventListener( CC_CALLBACK_2( GenerateNDeleteScene::onGenerateButton, this ) );
				button->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.3f, visibleSize.height * 0.2f )
				);
				addChild( button );
				{
					auto label = Label::createWithTTF( "Generate", cpg::StringTable::GetFontPath(), 12 );
					button->setTitleLabel( label );

					button->setContentSize( label->getContentSize() + ButtonMargin + ButtonMargin );
				}
			}

			//
			// Delete
			//
			{
				auto button = ui::Button::create( "guide_01_2.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->addTouchEventListener( CC_CALLBACK_2( GenerateNDeleteScene::onDeleteButton, this ) );
				button->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.7f, visibleSize.height * 0.2f )
				);
				addChild( button );
				{
					auto label = Label::createWithTTF( "Delete", cpg::StringTable::GetFontPath(), 12 );
					button->setTitleLabel( label );

					button->setContentSize( label->getContentSize() + ButtonMargin + ButtonMargin );
				}
			}

			return true;
		}

		void GenerateNDeleteScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GenerateNDeleteScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void GenerateNDeleteScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void GenerateNDeleteScene::updateFileStatus()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_FileStatus ) );

			if( FileUtils::getInstance()->isFileExist( mFileFullPath ) )
			{
				label->setString( "Exist" );
			}
			else
			{
				label->setString( "Nothing" );
			}
		}

		void GenerateNDeleteScene::onGenerateButton( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			if( FileUtils::getInstance()->isFileExist( mFileFullPath ) )
			{
				return;
			}

			FileUtils::getInstance()->writeStringToFile( mFileFullPath, mFileFullPath );

			updateFileStatus();
		}
		void GenerateNDeleteScene::onDeleteButton( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			if( !FileUtils::getInstance()->isFileExist( mFileFullPath ) )
			{
				return;
			}

			FileUtils::getInstance()->removeFile( mFileFullPath );

			updateFileStatus();
		}

		void GenerateNDeleteScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
