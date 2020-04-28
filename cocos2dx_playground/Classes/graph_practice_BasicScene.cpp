#include "graph_practice_BasicScene.h"

#include <new>
#include <numeric>
#include <sstream>
#include <string>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCTweenFunction.h"
#include "ui/UIScale9Sprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

USING_NS_CC;

namespace
{
	const char* FontPath = "fonts/arial.ttf";
	const int FontSize = 9;
	const int TAG_CurrentValueIndicator = 20140416;
}

namespace graph_practice
{
	BasicScene::BasicScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mElapsedTime( 0.f )
	{}

	Scene* BasicScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) BasicScene( back_to_the_previous_scene_callback );
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

	bool BasicScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		scheduleUpdate();

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
			auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// Practice
		//
		{
			auto root_node = Node::create();
			root_node->setContentSize( Size( 100.f, 100.f ) );
			root_node->setPosition(
				visibleOrigin.x + ( visibleSize.width * 0.2 ) - ( root_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height * 0.5f - ( root_node->getContentSize().height * 0.5f )
			);
			addChild( root_node );

			//
			// Graph View
			//
			{
				auto view_node = Node::create();
				root_node->addChild( view_node );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					view_node->addChild( pivot, std::numeric_limits<int>::max() );
				}

				//
				// Background
				//
				{
					auto background_layer = LayerColor::create( Color4B( 100, 100, 100, 150 ), root_node->getContentSize().width, root_node->getContentSize().height );
					view_node->addChild( background_layer, -2 );
				}

				//
				// x, y
				//
				{
					auto x_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
					x_view->setScale9Enabled( true );
					x_view->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
					x_view->setContentSize( Size( root_node->getContentSize().width, 2.f ) );
					x_view->setColor( Color3B::BLUE );
					view_node->addChild( x_view );

					auto y_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
					y_view->setScale9Enabled( true );
					y_view->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT );
					y_view->setContentSize( Size( 2.f, root_node->getContentSize().height ) );
					y_view->setColor( Color3B::BLUE );
					view_node->addChild( y_view );
				}
			}

			//
			// Current Value Indicator
			//
			{
				mGraphNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mGraphNode->setTag( TAG_CurrentValueIndicator );
				mGraphNode->setScale( 4.f );
				root_node->addChild( mGraphNode );
			}
		}

		return true;
	}

	void BasicScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void BasicScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Node::onExit();
	}
	void BasicScene::update( float dt )
	{
		mElapsedTime += dt;
		if( 1.f < mElapsedTime )
		{
			mElapsedTime = 0.f;
		}

		mGraphNode->setPosition(
			mGraphNode->getParent()->getContentSize().width * mElapsedTime
			, mGraphNode->getParent()->getContentSize().height * mElapsedTime
		);

		Node::update( dt );
	}


	void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
