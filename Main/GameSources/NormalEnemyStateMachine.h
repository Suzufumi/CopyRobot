#pragma once
#include "stdafx.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	///	徘徊ステート
	//--------------------------------------------------------------------------------------
	class NormalEnemyWanderState : public ObjState<NormalEnemy>
	{
		NormalEnemyWanderState()
		{}
	public:
		// ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(NormalEnemyWanderState)

		virtual void Enter(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<NormalEnemy>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	///	ChasePlayerステート
	//--------------------------------------------------------------------------------------
	class NormalEnemyChasePlayer : public ObjState<NormalEnemy>
	{
		NormalEnemyChasePlayer()
		{}
	public:
		DECLARE_SINGLETON_INSTANCE(NormalEnemyChasePlayer)

		virtual void Enter(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<NormalEnemy>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	///	LostPlayerステート
	//--------------------------------------------------------------------------------------
	class NormalEnemyLostPlayer : public ObjState<NormalEnemy>
	{
		NormalEnemyLostPlayer()
		{}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(NormalEnemyLostPlayer)

		virtual void Enter(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<NormalEnemy>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	/// Copiedステート
	//--------------------------------------------------------------------------------------
	class NormalEnemyCopied : public ObjState<NormalEnemy>
	{
		NormalEnemyCopied()
		{}
	public:
		DECLARE_SINGLETON_INSTANCE(NormalEnemyCopied)
		virtual void Enter(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<NormalEnemy>& Obj) override;
	};


}