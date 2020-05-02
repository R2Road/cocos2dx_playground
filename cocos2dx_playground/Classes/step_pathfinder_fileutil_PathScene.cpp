#include "step_pathfinder_fileutil_PathScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "platform/CCFileUtils.h"

USING_NS_CC;

namespace
{
	const char* FontPath = "fonts/arial.ttf";
}

namespace step_pathfinder
{
	PathScene::PathScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* PathScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) PathScene( back_to_the_previous_scene_callback );
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

	bool PathScene::init()
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

			auto label = Label::createWithTTF( ss.str(), FontPath, 9, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 41, 0, 61, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		const float MAX_LINE_WIDTH = visibleSize.width * 0.8f;

		//
		// Writable Path
		//
		{
			// Title
			{
				auto label = Label::createWithTTF( "<Writable Path : for Save Various Data>", FontPath, 9 );
				label->setAnchorPoint( Vec2( 0.f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.1f
					, visibleOrigin.y + visibleSize.height * 0.7f
				) );
				addChild( label );
			}
			
			// Path
			{
				auto label = Label::createWithTTF( cocos2d::FileUtils::getInstance()->getWritablePath().c_str(), FontPath, 9 );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setMaxLineWidth( MAX_LINE_WIDTH );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.1f
					, visibleOrigin.y + visibleSize.height * 0.7f
				) );
				addChild( label );
			}
		}

		//
		// Default Resource Root Path
		//
		{
			// Title
			{
				auto label = Label::createWithTTF( "<Default Resource Root Path : Copied Resources>", FontPath, 9 );
				label->setAnchorPoint( Vec2( 0.f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.1f
					, visibleOrigin.y + visibleSize.height * 0.5f
				) );
				addChild( label );
			}

			// Path
			{
				auto label = Label::createWithTTF( FileUtils::getInstance()->getDefaultResourceRootPath().c_str(), FontPath, 9 );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setMaxLineWidth( MAX_LINE_WIDTH );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.1f
					, visibleOrigin.y + visibleSize.height * 0.5f
				) );
				addChild( label );
			}
		}

		//
		// Your Resource Path
		//
		{
			// Title
			{
				auto label = Label::createWithTTF( "<Your Resource Path : for Tool>", FontPath, 9 );
				label->setAnchorPoint( Vec2( 0.f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.1f
					, visibleOrigin.y + visibleSize.height * 0.3f
				) );
				addChild( label );
			}

			// Path
			{
				auto source_path = FileUtils::getInstance()->getDefaultResourceRootPath();
				if( !source_path.empty() )
				{
					const char indicator = '/';
					int current_offset = source_path.size();
					int count = 0;
					do
					{
						current_offset = source_path.rfind( indicator, current_offset - 1 );
						if( std::string::npos == current_offset || 0 == current_offset )
						{
							break;
						}

						if( 0 == current_offset )
						{
							break;
						}

						++count;
					} while( 4 > count );

					if( 4 == count )
					{
						source_path.erase( current_offset + 1 );
						source_path.append( "Resources/" );
					}
				}

				auto label = Label::createWithTTF( source_path.c_str(), FontPath, 9 );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setMaxLineWidth( MAX_LINE_WIDTH );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.1f
					, visibleOrigin.y + visibleSize.height * 0.3f
				) );
				addChild( label );
			}
		}

		return true;
	}

	void PathScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PathScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void PathScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Node::onExit();
	}
	
	void PathScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
