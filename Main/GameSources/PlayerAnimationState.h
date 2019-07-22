#pragma once
#include "stdafx.h"

namespace basecross
{
	class Player;

	class PlayerDefaultAnim : public ObjState<Player>
	{
		PlayerDefaultAnim() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerDefaultAnim)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	class PlayerCopyOpenAnim : public ObjState<Player>
	{
		PlayerCopyOpenAnim() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerCopyOpenAnim)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	class PlayerCopyCloseAnim : public ObjState<Player>
	{
		PlayerCopyCloseAnim() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerCopyCloseAnim)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};


	class PlayerGoalAnim : public ObjState<Player>
	{
		PlayerGoalAnim() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerGoalAnim)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
}