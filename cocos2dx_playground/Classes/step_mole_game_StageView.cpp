#include "step_mole_game_StageView.h"

#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "ui/UIButton.h"

#include "step_mole_AnimationComponent.h"
#include "step_mole_animation_InfoContainer.h"
#include "step_mole_CircleCollisionComponent.h"
#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_mole_ObjectComponent.h"

USING_NS_CC;

namespace step_mole
{
	namespace game
	{
		StageNode::StageNode( const StageConfig stage_config ) :
			mStageConfig( stage_config )
			, mObjectComponentList( stage_config.BlockCount_Vercital * stage_config.BlockCount_Horizontal, nullptr )
			, mCollisionComponentList( stage_config.BlockCount_Vercital * stage_config.BlockCount_Horizontal, nullptr )
		{}

		StageNode* StageNode::create(
			const StageConfig stage_config
			, const TargetProcessExitCallback& target_rest_callback
			, const StageNodeConfig stage_view_config
			, const CircleCollisionComponentConfig& circle_collision_component_config
		)
		{
			auto ret = new ( std::nothrow ) StageNode( stage_config );
			if( !ret || !ret->init( target_rest_callback, stage_view_config, circle_collision_component_config ) )
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

		bool StageNode::init( const TargetProcessExitCallback& target_rest_callback, const StageNodeConfig stage_view_config, const CircleCollisionComponentConfig& circle_collision_component_config )
		{
			if( !Node::init() )
			{
				return false;
			}

			const Size StageSize(
				mStageConfig.BlockSize.width * mStageConfig.BlockCount_Horizontal
				, mStageConfig.BlockSize.width * mStageConfig.BlockCount_Vercital
			);
			setContentSize( StageSize );

			// Pivot
			if( stage_view_config.bShowPivot )
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 4.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			if( stage_view_config.bShowBackgroundGuide )
			{
				auto background_guide = LayerColor::create( Color4B( 255, 0, 255, 150 ), getContentSize().width, getContentSize().height );
				addChild( background_guide, std::numeric_limits<int>::min() );
			}

			auto content_root_node = Node::create();
			addChild( content_root_node );

			//
			// Terrain View
			//
			{
				auto tile_sprite_frame_0 = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_tile_0.png" );
				auto tile_sprite_frame_1 = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_tile_1.png" );
				CCASSERT( tile_sprite_frame_0, "Sprite Frame Not Found" );

				const auto block_scale = mStageConfig.BlockSize.height / tile_sprite_frame_0->getRect().size.height;

				bool first_tile_indicator = true;
				bool current_tile_indicator = true;
				for( int by = 0; mStageConfig.BlockCount_Vercital > by; ++by )
				{
					current_tile_indicator = first_tile_indicator;

					for( int bx = 0; mStageConfig.BlockCount_Horizontal > bx; ++bx )
					{
						auto block_sprite = Sprite::createWithSpriteFrame( current_tile_indicator ? tile_sprite_frame_0 : tile_sprite_frame_1 );
						block_sprite->setAnchorPoint( Vec2::ZERO );
						block_sprite->setScale( block_scale );
						block_sprite->setPosition(
							bx * mStageConfig.BlockSize.width
							, by * mStageConfig.BlockSize.height
						);
						content_root_node->addChild( block_sprite );

						current_tile_indicator = !current_tile_indicator;
					}

					first_tile_indicator = !first_tile_indicator;
				}
			}

			//
			// Objects
			//
			{
				std::random_device rd;
				std::mt19937 randomEngine( rd() );
				std::uniform_int_distribution<> dist( 0, 1 );

				const Vec2 offset( mStageConfig.BlockSize.width * 0.5f, mStageConfig.BlockSize.height * 0.5f );
				std::size_t object_linear_index = 0;

				for( int by = 0; mStageConfig.BlockCount_Vercital > by; ++by )
				{
					for( int bx = 0; mStageConfig.BlockCount_Horizontal > bx; ++bx )
					{
						object_linear_index = bx + ( mStageConfig.BlockCount_Horizontal * by );

						auto object_node = MakeObject(
							object_linear_index
							, Vec2(
								offset
								+ Vec2( bx * mStageConfig.BlockSize.width, by * mStageConfig.BlockSize.height )
							)
							, dist( randomEngine )
							, target_rest_callback
							, circle_collision_component_config
						);
						content_root_node->addChild( object_node, 1 );

						mObjectComponentList[object_linear_index] = static_cast<ObjectComponent*>( object_node->getComponent( ObjectComponent::GetStaticName() ) );
						mCollisionComponentList[object_linear_index] = static_cast<CircleCollisionComponent*>( object_node->getComponent( CircleCollisionComponent::GetStaticName() ) );
					}
				}
			}

			//
			// Click Area
			//
			{
				auto click_area = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				click_area->setScale9Enabled( true );
				click_area->setContentSize( StageSize );
				click_area->addTouchEventListener( CC_CALLBACK_2( StageNode::onStageClick, this ) );
				click_area->setPosition( Vec2(
					StageSize.width * 0.5f
					, StageSize.height * 0.5f
				) );
				addChild( click_area );
			}

			return true;
		}

		Node* StageNode::MakeObject(
			const int object_tag
			, const cocos2d::Vec2 object_position
			, const int defalut_view_type
			, const TargetProcessExitCallback& target_rest_callback
			, const CircleCollisionComponentConfig& circle_collision_component_config
		)
		{
			auto object_node = Node::create();
			object_node->setTag( object_tag );
			object_node->setPosition( object_position );
			{
				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 2.f );
					object_node->addChild( pivot, std::numeric_limits<int>::max() );
				}

				// View
				auto view_node = Sprite::createWithSpriteFrameName( 0 == defalut_view_type ? "step_mole_target_wait_0.png" : "step_mole_target_wait_1.png" );
				view_node->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				view_node->setScale( 2.f );
				view_node->setPositionY( -18.f );
				object_node->addChild( view_node );
				
				// Animation Component
				auto animation_component = step_mole::AnimationComponent::create( step_mole::animation::GetInfoContainer() );
				view_node->addComponent( animation_component );

				// Collision Component
				auto circle_collision_component = step_mole::CircleCollisionComponent::create( 16.f, Vec2( 0.f, -4.f ), circle_collision_component_config );
				object_node->addComponent( circle_collision_component );

				// Object Component
				object_node->addComponent( step_mole::ObjectComponent::create( animation_component, circle_collision_component, target_rest_callback ) );

			}

			return object_node;
		}
		void StageNode::onStageClick( Ref* /*sender*/, ui::Widget::TouchEventType /*touch_event_type*/ )
		{
			CCLOG( "On Stage Click" );
		}

		void StageNode::RequestAction( const std::size_t object_index, const float life_time )
		{
			CCASSERT( object_index < mObjectComponentList.size(), "Invalid Object Index" );

			mObjectComponentList[object_index]->ProcessStart( life_time );
		}
	} // namespace game
} // namespace step_mole
