#pragma once

#include <vector>

namespace fsm1
{
	class Machine;

	class iState
	{
	public:
		iState() {}
		virtual ~iState() {}

		// none copy
		iState( const iState& ) = delete;
		iState( iState&& ) = delete;
		iState& operator=( const iState& ) = delete;
		iState& operator=( iState&& ) = delete;

		virtual void Init() {}
		virtual void Clear() {}

		virtual void Enter() {}
		virtual void Update( const float dt ) {}
		virtual void Exit() {}
	};

	template<typename SubStateT, typename OwnerT>
	class CustomeState : public iState
	{
	public:
		using SuperStateT = CustomeState<SubStateT, OwnerT>;
		using MyOwnerT = OwnerT;

		CustomeState( MyOwnerT& owner, Machine& machine, const std::size_t index ) : mOwner( owner ), mMachine( machine ), mIndex( index )
		{}

		std::size_t GetIndex() const { return mIndex; }

		void AddTransition( const std::size_t target_state_index )
		{
			mTransitions.emplace_back( target_state_index );
		}

	protected:
		void TransitionRequest( const std::size_t transition_index )
		{
			assert( mTransitions.size() > transition_index );
			mMachine.TransitionRequest( mTransitions[transition_index] );
		}

	protected:
		MyOwnerT& mOwner;
		Machine& mMachine;
		const int mIndex;
		std::vector<std::size_t> mTransitions;
	};
}