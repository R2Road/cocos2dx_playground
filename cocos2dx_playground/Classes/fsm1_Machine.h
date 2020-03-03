#pragma once

#include <memory>
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

		template<typename CustomeStateT, typename OwnerT>
		CustomeStateT& AddState( OwnerT& owner, const bool is_start )
		{
			static_assert(
				std::is_base_of<fsm1::iState, CustomeStateT>::value
				, "fsm1 : Machine : Add - Invalid Type"
			);

			auto new_state = new CustomeStateT( owner );
			new_state->Init();
			if( is_start )
			{
				mStartStateIndex = mStateList.size();
			}

			mStateList.emplace_back( new_state );

			return *new_state;
		}

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