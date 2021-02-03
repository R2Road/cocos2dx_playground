#include "algorithm_practice_floodfill_ProcessorNode.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "base/CCDirector.h"

#include "cpg_StringTable.h"

USING_NS_CC;

namespace algorithm_practice_floodfill
{
	ProcessorNode::ProcessorNode( const Config config ) :
		mConfig( config )
	{}

	ProcessorNode* ProcessorNode::create( const Config config )
	{
		auto ret = new ( std::nothrow ) ProcessorNode( config );
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

	bool ProcessorNode::init()
	{
		if( !Node::init() )
		{
			return false;
		}

		const auto visibleOrigin = _director->getVisibleOrigin();
		const auto visibleSize = _director->getVisibleSize();
		const Vec2 visibleCenter(
			visibleOrigin.x + ( visibleSize.width * 0.5f )
			, visibleOrigin.y + ( visibleSize.height * 0.5f )
		);

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << std::endl;
			ss << std::endl;
			ss << std::endl;
			ss << "[R] : " << "Reset";
			ss << std::endl;
			ss << "[Space] : " << "Step";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 7, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition(
				visibleOrigin
				+ Vec2( 0.f, visibleSize.height )
				- Vec2( 0.f, 20.f )
			);
			addChild( label, std::numeric_limits<int>::max() );
		}

		return true;
	}
}
