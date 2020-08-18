#include "step_rain_of_chaos_game_test_BulletLifeComponentScene.h"

#include <new>
#include <numeric>
#include <random>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_Clamp.h"

#include "step_mole_AnimationComponent.h"
#include "step_rain_of_chaos_game_AnimationInfoContainer.h"
#include "step_mole_CircleCollisionComponent.h"
#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_rain_of_chaos_game_BulletLifeComponent.h"

USING_NS_CC;

namespace
{
	const int TAG_BulletNode = 20140416;
	const int TAG_MoveSpeedView = 100;
}

namespace step_rain_of_chaos
{
	namespace game_test
	{
		BulletLifeComponentScene::BulletLifeComponentScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mStageConfig()
			, mCurrentMoveSpeed( 3 )
		{}

		Scene* BulletLifeComponentScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) BulletLifeComponentScene( back_to_the_previous_scene_callback );
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

		bool BulletLifeComponentScene::init()
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
				ss << std::endl;
				ss << std::endl;
				ss << "[1] : Bullet - Process Start";
				ss << std::endl;
				ss << "[2] : Bullet - Kill";
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow Key - Up] : Move Speed - Increase";
				ss << std::endl;
				ss << "[Arrow Key - Down] : Move Speed - Decrease";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 63, 23, 14, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			mStageConfig.Build(
				visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f
				, 120.f
			);
			
			//
			// Stage Area View
			//
			{
				auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
				sprite->setAnchorPoint( Vec2::ZERO );
				sprite->setContentSize( mStageConfig.GetStageRect().size );
				sprite->setColor( Color3B::GREEN );
				sprite->setPosition( mStageConfig.GetStageRect().origin );
				addChild( sprite );
				{
					auto label = Label::createWithTTF( "Stage Area", "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::RIGHT );
					label->setAnchorPoint( Vec2( 1.f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition( Vec2(
						sprite->getContentSize().width
						, sprite->getContentSize().height
					) );
					sprite->addChild( label );
				}
			}

			//
			// Bullet Life Area View
			//
			{
				auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
				sprite->setAnchorPoint( Vec2::ZERO );
				sprite->setContentSize( mStageConfig.GetBulletLifeRect().size );
				sprite->setColor( Color3B::RED );
				sprite->setPosition( mStageConfig.GetBulletLifeRect().origin );
				addChild( sprite );
				{
					auto label = Label::createWithTTF( "Bullet Life Area", "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::RIGHT );
					label->setAnchorPoint( Vec2( 1.f, 1.f ) );
					label->setColor( Color3B::RED );
					label->setPosition( Vec2(
						sprite->getContentSize().width
						, sprite->getContentSize().height
					) );
					sprite->addChild( label );
				}
			}

			//
			// Bullet Generate Area View
			//
			{
				auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
				sprite->setAnchorPoint( Vec2::ZERO );
				sprite->setContentSize( mStageConfig.GetBulletGenerateAreaMax().size );
				sprite->setColor( Color3B::WHITE );
				sprite->setPosition( mStageConfig.GetBulletGenerateAreaMax().origin );
				addChild( sprite );
				{
					auto label = Label::createWithTTF( "Bullet Generate Area", "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::RIGHT );
					label->setAnchorPoint( Vec2( 1.f, 1.f ) );
					label->setColor( Color3B::WHITE );
					label->setPosition( Vec2(
						sprite->getContentSize().width
						, sprite->getContentSize().height
					) );
					sprite->addChild( label );
				}
			}

			//
			// Current Move Speed
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
				label->setTag( TAG_MoveSpeedView );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateMoveSpeedView();
			}

			//
			// Object Node
			//
			{
				auto object_node = Node::create();
				object_node->setTag( TAG_BulletNode );
				object_node->setPosition( mStageConfig.GetCenter() );
				addChild( object_node, 1 );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( _director->getContentScaleFactor() );
					object_node->addChild( pivot, std::numeric_limits<int>::max() );
				}

				// View
				auto view_node = Sprite::createWithSpriteFrameName( "step_rain_of_chaos_bullet_01_damaged_3.png" );
				view_node->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				view_node->setScale( _director->getContentScaleFactor() );
				object_node->addChild( view_node );

				// Animation Component
				auto animation_component = step_mole::AnimationComponent::create( step_rain_of_chaos::game::GetBulletAnimationInfoContainer() );
				view_node->addComponent( animation_component );

				// Circle Collision Component
				auto circle_collision_component = step_mole::CircleCollisionComponent::create( 4.f, Vec2::ZERO, step_mole::CircleCollisionComponentConfig{ true, true, true } );
				object_node->addComponent( circle_collision_component );

				// Bullet Life Component
				object_node->addComponent( step_rain_of_chaos::game::BulletLifeComponent::create( mStageConfig.GetBulletLifeRect(), animation_component, circle_collision_component, nullptr ) );
			}

			return true;
		}

		void BulletLifeComponentScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BulletLifeComponentScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BulletLifeComponentScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void BulletLifeComponentScene::updateMoveSpeedView()
		{
			auto life_time_node = static_cast<Label*>( getChildByTag( TAG_MoveSpeedView ) );
			life_time_node->setString( StringUtils::format( "Move Speed : %d", mCurrentMoveSpeed ) );
		}

		void BulletLifeComponentScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_1:
			{
				const Vec2 pivot_vector( mStageConfig.GetBulletGenerateAreaMax().size.width * 0.5f, mStageConfig.GetBulletGenerateAreaMax().size.height * 0.5f );
				Vec2 direction_vector = pivot_vector;

				static std::mt19937 randomEngine( std::random_device{}() );
				std::uniform_real_distribution<> dist( 0, 360 );
				direction_vector.rotate( Vec2::ZERO, CC_DEGREES_TO_RADIANS( dist( randomEngine ) ) );				
				direction_vector.x = cpg::clamp( direction_vector.x, -pivot_vector.x, pivot_vector.x );
				direction_vector.y = cpg::clamp( direction_vector.y, -pivot_vector.y, pivot_vector.y );

				const auto start_position = mStageConfig.GetCenter() + direction_vector;

				direction_vector.normalize();
				direction_vector.scale( mCurrentMoveSpeed );

				auto object_node = getChildByTag( TAG_BulletNode );
				auto bullet_life_component = static_cast<step_rain_of_chaos::game::BulletLifeComponent*>( object_node->getComponent( step_rain_of_chaos::game::BulletLifeComponent::GetStaticName() ) );
				bullet_life_component->ProcessStart( start_position, -direction_vector );
			}
			return;

			case EventKeyboard::KeyCode::KEY_2:
			{
				auto object_node = getChildByTag( TAG_BulletNode );
				auto bullet_life_component = static_cast<step_rain_of_chaos::game::BulletLifeComponent*>( object_node->getComponent( step_rain_of_chaos::game::BulletLifeComponent::GetStaticName() ) );
				bullet_life_component->ProcessBoom();
			}
			return;

			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mCurrentMoveSpeed += 1;
				updateMoveSpeedView();
				return;

			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mCurrentMoveSpeed = std::max( 1, mCurrentMoveSpeed - 1 );
				updateMoveSpeedView();
				return;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
