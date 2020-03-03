#pragma once

namespace fsm1
{
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

		CustomeState( MyOwnerT& owner ) : mOwner( owner )
		{}

	protected:
		MyOwnerT& mOwner;
	};
}