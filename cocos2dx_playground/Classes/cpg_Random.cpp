#include "cpg_Random.h"

#include <random>
#include <algorithm>

namespace cpg
{
	std::mt19937& getRandomEngine()
	{
		static std::random_device rd;
		static std::mt19937 randomEngine( rd() );

		return randomEngine;
	}

	int Random::GetInt( const int min, const int max )
	{
		std::uniform_int_distribution<> dist( std::min( min, max ), std::max( min, max ) );
		return dist( getRandomEngine() );
	}
	int Random::GetInt_0To1()
	{
		return GetInt( 0, 1 );
	}

	float Random::GetFloat( const float min, const float max )
	{
		std::uniform_real_distribution<> dist( std::min( min, max ), std::max( min, max ) );
		return dist( getRandomEngine() );
	}

	bool Random::GetBool()
	{
		return 0 == GetInt_0To1();
	}
}

