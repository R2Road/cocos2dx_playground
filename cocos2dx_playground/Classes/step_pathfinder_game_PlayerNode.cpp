#include "step_pathfinder_game_PlayerNode.h"

#include <new>

#include "2d/CCActionInterval.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"

USING_NS_CC;

namespace step_pathfinder
{
	namespace game
	{
		PlayerNode::PlayerNode()
		{}

		PlayerNode* PlayerNode::create()
		{
			auto ret = new ( std::nothrow ) PlayerNode();
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

		bool PlayerNode::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			auto view_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
			view_node->setAnchorPoint( Vec2( 0.5f, 0.2f ) );
			view_node->setScale( _director->getContentScaleFactor() );
			addChild( view_node );

			//
			// Animation Setup
			//
			{
				auto animation_object = Animation::create();
				animation_object->setDelayPerUnit( 0.2f );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
				animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

				auto animate_action = Animate::create( animation_object );

				auto repeat_action = RepeatForever::create( animate_action );

				view_node->runAction( repeat_action );
			}

			return true;
		}
	}
}
