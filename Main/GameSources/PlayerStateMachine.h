/*!
@file PlayerStateMachine.h
@brief プレイヤーのステートマシン
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	///	何もしない
	//--------------------------------------------------------------------------------------
	class PlayerNoneState : public ObjState<Player>
	{
		PlayerNoneState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerNoneState)
		virtual void Enter(const shared_ptr<Player>& Obj)override {};
		virtual void Execute(const shared_ptr<Player>& Obj)override {};
		virtual void Exit(const shared_ptr<Player>& Obj)override {};
	};

	//--------------------------------------------------------------------------------------
	///	ゲームオーバーステート
	//--------------------------------------------------------------------------------------
	class PlayerDeadState : public ObjState<Player>
	{
		PlayerDeadState() {}
	public:
		weak_ptr<SpriteStudioAnim> m_OverAnim;
		DECLARE_SINGLETON_INSTANCE(PlayerDeadState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	class PlayerNormalState : public ObjState<Player>
	{
		PlayerNormalState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerNormalState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//--------------------------------------------------------------------------------------
	///	コピーステート
	//--------------------------------------------------------------------------------------
	class PlayerCopyingState : public ObjState<Player>
	{
		PlayerCopyingState() {}

		Player::CopyType m_PrevType;
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerCopyingState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	コアにエネミーを取り込むステート
	//--------------------------------------------------------------------------------------
	class PlayerTakeInState : public ObjState<Player>
	{
		PlayerTakeInState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerTakeInState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	カメレオンDefaultステート
	//--------------------------------------------------------------------------------------
	class PlayerCamereonDefaultState : public ObjState<Player>
	{
		PlayerCamereonDefaultState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerCamereonDefaultState)

		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	class PlayerCamereonTransform : public ObjState<Player>
	{
		PlayerCamereonTransform() {}

		shared_ptr<SoundItem> m_Se;
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerCamereonTransform)

		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	class PlayerCamereonAssimililation : public ObjState<Player>
	{
		PlayerCamereonAssimililation() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerCamereonAssimililation)

		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	class PlayerCamereonRelease : public ObjState<Player>
	{
		PlayerCamereonRelease() {}

		shared_ptr<SoundItem> m_Se;
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerCamereonRelease)

		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	ゴール状態
	//--------------------------------------------------------------------------------------
	class PlayerGoalState : public ObjState<Player>
	{
		PlayerGoalState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerGoalState);

		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	スタート状態
	//--------------------------------------------------------------------------------------
	class PlayerStartState : public ObjState<Player>
	{
		bool m_IsStartOpenSe;
		PlayerStartState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerStartState);

		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
}