#include "step_rain_of_chaos_collision_CollectionScene.h"

#include <numeric>
#include <new>
#include <random>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_mole_CircleCollisionComponent.h"
#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_defender_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_Bullet = 20140416;
	const int TAG_BulletCountView = 20160528;
}

namespace step_rain_of_chaos
{
	namespace collision
	{
		CollectionScene::CollectionScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mCollisionCollection()
		{}

		Scene* CollectionScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) CollectionScene( back_to_the_previous_scene_callback );
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
			}
			else
			{
				ret->scheduleUpdate();
				ret->autorelease();
			}

			return ret;
		}

		bool CollectionScene::init()
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
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[A] : Add Bullet";
				ss << cpg::linefeed;
				ss << "[S] : Remove Bullet";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 130, 49, 29, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Bullet Count
			//
			{
				auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 14, Size::ZERO, TextHAlignment::RIGHT );
				label->setTag( TAG_BulletCountView );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateBulletCountView();
			}

			return true;
		}

		void CollectionScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( CollectionScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void CollectionScene::update( float dt )
		{
			Scene::update( dt );
		}
		void CollectionScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void CollectionScene::addChild( Node* child, int localZOrder, int tag )
		{
			addCollision( child );
			Scene::addChild( child, localZOrder, tag );
		}
		void CollectionScene::addChild( Node* child, int localZOrder, const std::string &name )
		{
			addCollision( child );
			Scene::addChild( child, localZOrder, name );
		}
		void CollectionScene::removeChild( Node* child, bool cleanup /* = true */ )
		{
			removeCollision( child );
			Scene::removeChild( child, cleanup );
		}
		void CollectionScene::removeAllChildrenWithCleanup( bool cleanup )
		{
			mCollisionCollection.clear();
			Scene::removeAllChildrenWithCleanup( cleanup );
		}

		void CollectionScene::addCollision( cocos2d::Node* child )
		{
			auto target_component = child->getComponent( step_mole::CircleCollisionComponent::GetStaticName() );
			if( !target_component )
			{
				return;
			}

			mCollisionCollection.push_back( static_cast<step_mole::CircleCollisionComponent*>( target_component ) );
		}
		void CollectionScene::removeCollision( cocos2d::Node* child )
		{
			auto target_component = child->getComponent( step_mole::CircleCollisionComponent::GetStaticName() );
			if( !target_component )
			{
				return;
			}

			mCollisionCollection.remove( static_cast<step_mole::CircleCollisionComponent*>( target_component ) );
		}

		void CollectionScene::makeBullet()
		{
			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();

			const Vec2 visibleCenter(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			);
			const float negative_range = ( visibleSize.height - visibleOrigin.y ) * 0.3f;
			const float position_margin = 10.f;

			std::random_device rd;
			std::mt19937 randomEngine( rd() );
			std::uniform_int_distribution<> distX( static_cast<int>( visibleOrigin.x + position_margin ), static_cast<int>( visibleSize.width - position_margin ) );
			std::uniform_int_distribution<> distY( static_cast<int>( visibleOrigin.y + position_margin ), static_cast<int>( visibleSize.height - position_margin ) );

			Vec2 new_bullet_position;
			while( 1 )
			{
				new_bullet_position.set(
					distX( randomEngine )
					, distY( randomEngine )
				);

				if( negative_range >= visibleCenter.distance( new_bullet_position ) )
				{
					continue;
				}

				break;
			}
				

			auto bullet_root_node = Node::create();
			{
				bullet_root_node->setTag( TAG_Bullet );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					bullet_root_node->addChild( pivot, std::numeric_limits<int>::max() );
				}

				// View
				auto view_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				view_node->setScale( _director->getContentScaleFactor() );
				bullet_root_node->addChild( view_node );

				const float radius = ( view_node->getBoundingBox().size.height ) * 0.5f;

				// Collision Component
				bullet_root_node->addComponent( step_mole::CircleCollisionComponent::create( radius, Vec2::ZERO, step_mole::CircleCollisionComponentConfig{ false, false, false } ) );
			}

			bullet_root_node->setPosition( visibleOrigin + new_bullet_position );
			addChild( bullet_root_node );
		}
		void CollectionScene::removeBullet()
		{
			auto node = getChildByTag( TAG_Bullet );
			if( node )
			{
				removeChild( node );
			}
		}

		void CollectionScene::updateBulletCountView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_BulletCountView ) );
			label->setString( StringUtils::format( "Child Count : %3d\nCollision Count%3d", getChildrenCount() - 2, mCollisionCollection.size() ) );
		}

		void CollectionScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			if( EventKeyboard::KeyCode::KEY_A == keycode )
			{
				makeBullet();
				updateBulletCountView();
			}
			if( EventKeyboard::KeyCode::KEY_S == keycode )
			{
				removeBullet();
				updateBulletCountView();
			}
		}
	}
}
