#include "step_rain_of_chaos_game_StageNode.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "ui/UIScale9Sprite.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		StageNode::StageNode( const StageConfig stage_config ) : mStageConfig( stage_config )
		{}

		StageNode* StageNode::create( const StageConfig stage_config, const DebugConfig stage_node_config )
		{
			auto ret = new ( std::nothrow ) StageNode( stage_config );
			if( !ret || !ret->init( stage_node_config ) )
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

		bool StageNode::init( const DebugConfig stage_node_config )
		{
			if( !Node::init() )
			{
				return false;
			}

			setContentSize( _director->getVisibleSize() );
			
			//
			// Pivot
			//
			if( stage_node_config.bShowPivot )
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 4.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Stage Area Guide
			//
			if( stage_node_config.bShowAreaGuide )
			{
				// Stage Area View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( mStageConfig.GetStageArea().size );
					sprite->setColor( Color3B::GREEN );
					sprite->setPosition( mStageConfig.GetStageArea().origin );
					addChild( sprite, std::numeric_limits<int>::min() );
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

				// Bullet Life Area View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( mStageConfig.GetBulletLifeArea().size );
					sprite->setColor( Color3B::RED );
					sprite->setPosition( mStageConfig.GetBulletLifeArea().origin );
					addChild( sprite, std::numeric_limits<int>::min() );
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


				// Bullet Generate Area View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( mStageConfig.GetBulletGenerateArea().size );
					sprite->setColor( Color3B::WHITE );
					sprite->setPosition( mStageConfig.GetBulletGenerateArea().origin );
					addChild( sprite, std::numeric_limits<int>::min() );
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
			}

			return true;
		}
	} // namespace game
} // namespace step_mole
