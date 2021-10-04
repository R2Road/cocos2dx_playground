#pragma once

namespace cpg
{
	class Random
	{
	public:
		static int GetInt( const int min, const int max );
		static int GetInt_0To1();
		static float GetFloat( const float min, const float max );
		static bool GetBool();
	};
}

