#pragma once
#include "stdafx.h"

namespace basecross
{
	//------------------------------------------------------------------------------
	/// 徘徊ステート
	//------------------------------------------------------------------------------
	class CamereonEnemyWander : public ObjState<CamereonEnemy>
	{
		CamereonEnemyWander()
		{}
	public:
		// ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(CamereonEnemyWander)

		virtual void Enter(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<CamereonEnemy>& Obj) override;
	};

	//------------------------------------------------------------------------------
	/// コピーされているステート
	//------------------------------------------------------------------------------
	class CamereonEnemyCopied : public ObjState<CamereonEnemy>
	{
		CamereonEnemyCopied()
		{}
	public:
		// ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(CamereonEnemyCopied)

		virtual void Enter(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<CamereonEnemy>& Obj) override;
	};

	//------------------------------------------------------------------------------
	/// ChasePlayerステート
	//------------------------------------------------------------------------------
	class CamereonEnemyChasePlayer : public ObjState<CamereonEnemy>
	{
		CamereonEnemyChasePlayer()
		{}
	public:
		// ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(CamereonEnemyChasePlayer)

		virtual void Enter(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<CamereonEnemy>& Obj) override;
	};

	//------------------------------------------------------------------------------
	///	LostPlayerステート
	//------------------------------------------------------------------------------
	class CamereonEnemyLostPlayer : public ObjState<CamereonEnemy>
	{
		CamereonEnemyLostPlayer()
		{}
	public:
		// ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(CamereonEnemyLostPlayer)

		virtual void Enter(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<CamereonEnemy>& Obj) override;
	};

}