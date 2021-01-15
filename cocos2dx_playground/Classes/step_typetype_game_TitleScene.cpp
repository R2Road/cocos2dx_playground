#include "step_typetype_game_TitleScene.h"

#include <new>
#include <numeric>
#include <sstream>
#include <vector>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_typetype_game_LetterViewNode.h"
#include "step_typetype_game_PlayScene.h"
#include "step_typetype_game_ResultScene.h"
#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace
{
	const std::vector<char*> Title_Word_Sprite_Frames = {
		"T", "Y", "P", "E", "T", "Y", "P", "E",
	};
}

namespace step_typetype
{
	namespace game
	{
		TitleScene::TitleScene() :
			mKeyboardListener( nullptr )

			, mStep4Letters( 0 )
			, mElapsedTime4Letters( 0.f )
			, mCurrentLetterTag( 0 )

			, mInputIndicatorLabel( nullptr )
			, mElapsedTime4InputIndicator( 0.f )
		{}

		Scene* TitleScene::create()
		{
			auto ret = new ( std::nothrow ) TitleScene();
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

		bool TitleScene::init()
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
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << "[F1] : " << ResultScene::getTitle();

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 7 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Title
			//
			{
				const Vec2 pivotPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.56f )
				);

				const float WordBlockWidth = 44.f;
				const float WordsWidth = ( WordBlockWidth * 3 );
				const float WordsMargin = 30.f;

				int i = 0;
				for( const auto& w : Title_Word_Sprite_Frames )
				{
					auto letter_view_node = Label::createWithTTF( w, "fonts/NanumSquareR.ttf", 32 );
					if( 4 > i )
					{
						letter_view_node->setTag( i );
						letter_view_node->setAnchorPoint( Vec2( 0.5f, 1.f ) );
						letter_view_node->setColor( Color3B( 100u, 100u, 100u ) );
						letter_view_node->setPosition(
							pivotPosition
							- Vec2( WordsMargin, 0.f )
							- Vec2( WordsWidth, 0.f )
							+ Vec2( i * WordBlockWidth, 0.f )
						);
					}
					else
					{
						letter_view_node->setTag( i - 4 );
						letter_view_node->setAnchorPoint( Vec2( 0.5f, 0.f ) );
						letter_view_node->setColor( Color3B::WHITE );
						letter_view_node->setPosition(
							pivotPosition
							+ Vec2( WordsMargin, 0.f )
							+ Vec2( ( i - 4 ) * WordBlockWidth, 0.f )
						);
					}
					addChild( letter_view_node );

					++i;
				}
			}


			//
			// input indicator
			//
			{
				mInputIndicatorLabel = Label::createWithTTF( "PRESS SPACE BAR", "fonts/NanumSquareR.ttf", 14 );
				mInputIndicatorLabel->setPosition(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.23f
				);
				addChild( mInputIndicatorLabel, 1 );
			}

			//
			// Setup
			//
			schedule( schedule_selector( TitleScene::update4Letters ) );
			schedule( schedule_selector( TitleScene::update4InputIndicator ) );

			return true;
		}

		void TitleScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TitleScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void TitleScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void TitleScene::update4Letters( float dt )
		{
			switch( mStep4Letters )
			{
			case 0: // wait
				mElapsedTime4Letters += dt;
				if( 3.f <= mElapsedTime4Letters )
				{
					++mStep4Letters;
					mElapsedTime4Letters = 0.f;
				}
				break;

			case 1: // Move Letter
				mElapsedTime4Letters += dt;
				if( 0.13f <= mElapsedTime4Letters )
				{
					for( auto c : getChildren() )
					{
						if( mCurrentLetterTag != c->getTag() )
						{
							continue;
						}

						if( Color3B::WHITE == c->getColor() )
						{
							c->setAnchorPoint( Vec2( 0.5f, 1.f ) );
							c->setColor( Color3B( 100u, 100u, 100u ) );
						}
						else
						{
							c->setAnchorPoint( Vec2( 0.5f, 0.f ) );
							c->setColor( Color3B::WHITE );
						}
					}

					++mCurrentLetterTag;
					mElapsedTime4Letters = 0.f;

					if( 4 == mCurrentLetterTag )
					{
						mStep4Letters = 0;
						mCurrentLetterTag = 0;
					}
				}
				break;
			}
		}

		void TitleScene::update4InputIndicator( float dt )
		{
			mElapsedTime4InputIndicator += dt;

			if( mInputIndicatorLabel->isVisible() && mElapsedTime4InputIndicator > 0.8f )
			{
				mElapsedTime4InputIndicator = 0.f;
				mInputIndicatorLabel->setVisible( false );
			}
			else if( !mInputIndicatorLabel->isVisible() && mElapsedTime4InputIndicator > 0.4f )
			{
				mElapsedTime4InputIndicator = 0.f;
				mInputIndicatorLabel->setVisible( true );
			}
		}

		void TitleScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_typetype::RootScene::create() );
				return;
			}

			if( EventKeyboard::KeyCode::KEY_F1 == keycode )
			{
				_director->replaceScene( ResultScene::create( 123.456f ) );
				return;
			}

			if( EventKeyboard::KeyCode::KEY_SPACE == keycode )
			{
				_director->replaceScene( step_typetype::game::PlayScene::create() );
				return;
			}
		}
	}
}
