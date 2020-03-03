#include "fsm1_Machine.h"

#include <numeric>
#include <assert.h>

namespace fsm1
{
	Machine::Machine() :
		INVALID_STATE_INDEX( std::numeric_limits<std::size_t>::max() )
		, mStateList()
		, mStartStateIndex( 0 )
		, mCurrentState( nullptr )
	{}
	Machine::~Machine()
	{
		for( auto s : mStateList )
		{
			delete s;
		}
	}

	void Machine::Enter()
	{
		if( !mStateList.empty() && INVALID_STATE_INDEX != mStartStateIndex )
		{
			mCurrentState = mStateList[mStartStateIndex];
		}

		assert( mCurrentState );
		if( mCurrentState )
		{
			mCurrentState->Enter();
		}
	}
	void Machine::Update( const float dt )
	{
		if( mCurrentState )
		{
			mCurrentState->Update( dt );
		}
	}
	void Machine::Exit()
	{
		if( mCurrentState )
		{
			mCurrentState->Exit();
		}
	}
}