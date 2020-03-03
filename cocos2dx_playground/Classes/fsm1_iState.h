#pragma once

namespace fsm1
{
	class iState
	{
	public:
		virtual ~iState() {}

		virtual void Init() {}
		virtual void Clear() {}

		virtual void Enter() {}
		virtual void Update( const float dt ) {}
		virtual void Exit() {}
	};

	template<typename MyStateT, typename OwnerT>
	class CustomeState : public iState
	{
	public:
		using ParentT = CustomeState<MyStateT, OwnerT>;
		using MyOwnerT = OwnerT;

		CustomeState( MyOwnerT& owner ) : mOwner( owner )
		{}

	protected:
		MyOwnerT& mOwner;
	};
}