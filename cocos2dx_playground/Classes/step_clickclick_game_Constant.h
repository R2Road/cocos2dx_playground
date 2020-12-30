#pragma once

namespace step_clickclick
{
	namespace game
	{
		enum class eBlockType
		{
			Single,
			Same,
			Different,

			SIZE,
			FIRST = Single,
			END = Different,
		};

		const char* ConvertBlockType2SpriteFrameName( const eBlockType block_type );


		enum eEffectIndex
		{
			Increase
			, Decrease
			, Die
		};
	}
}
