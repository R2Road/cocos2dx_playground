#pragma once

#include <vector>
#include <type_traits>

#include "fsm1_iState.h"

namespace fsm1
{
	class Machine
	{
	public:
		Machine();
		~Machine();

		template<typename StateT, typename OwnerT>
		StateT& AddState( OwnerT& owner, const bool is_start )
		{
			static_assert(
				std::is_base_of<fsm1::iState, StateT>::value
				, "fsm1 : Machine : Add - Invalid Type"
			);

			auto new_state = new StateT( owner, *this, mStateList.size() );
			new_state->Init();
			if( is_start )
			{
				mStartStateIndex = new_state->GetIndex();
			}

			mStateList.emplace_back( new_state );

			return *new_state;
		}

		void TransitionRequest( const std::size_t state_index );

		void Enter();
		void Update( const float dt );
		void Exit();

	private:
		const std::size_t INVALID_STATE_INDEX;

		std::vector<iState*> mStateList;
		std::size_t mStartStateIndex;
		iState* mCurrentState;
	};
}