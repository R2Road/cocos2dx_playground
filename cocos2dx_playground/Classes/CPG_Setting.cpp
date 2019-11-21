#include "CPG_Setting.h"

namespace CPG
{
	void Setting::load( cocos2d::Size& frame_resolution, cocos2d::Size& design_resolution )
	{
		frame_resolution.setSize( 1024, 768 );
		design_resolution.setSize( 480, 320 );
	}
}