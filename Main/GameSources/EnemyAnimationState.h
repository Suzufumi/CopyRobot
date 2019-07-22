#pragma once
#include "stdafx.h"

namespace basecross
{
	class NormalEnemy;
	//----------------------------------------------------------------------
	/// NormalEnemyアニメーション
	//----------------------------------------------------------------------

	//----------------------------------------------------------------------
	/// Defaultアニメーション
	//----------------------------------------------------------------------
	class NormalEnemyDefaultAnim : public ObjState<NormalEnemy>
	{
		NormalEnemyDefaultAnim()
		{}
	public:
		DECLARE_SINGLETON_INSTANCE(NormalEnemyDefaultAnim)
		virtual void Enter(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<NormalEnemy>& Obj) override;
	};

	//----------------------------------------------------------------------
	/// Chaseアニメーション
	//----------------------------------------------------------------------
	class NormalEnemyChaseAnim : public ObjState<NormalEnemy>
	{
		NormalEnemyChaseAnim()
		{}
	public:
		DECLARE_SINGLETON_INSTANCE(NormalEnemyChaseAnim)
		virtual void Enter(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<NormalEnemy>& Obj) override;
	};

	//----------------------------------------------------------------------
	/// Releaseアニメーション
	//----------------------------------------------------------------------
	class NormalEnemyReleaseAnim : public ObjState<NormalEnemy>
	{
		NormalEnemyReleaseAnim()
		{}
	public:
		DECLARE_SINGLETON_INSTANCE(NormalEnemyReleaseAnim)
		virtual void Enter(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<NormalEnemy>& Obj) override;
	};


	//----------------------------------------------------------------------
	/// CamereonEnemyアニメーション
	//----------------------------------------------------------------------

	class CamereonEnemy;
	//----------------------------------------------------------------------
	/// Defaultアニメーション(CamelEnemy)
	//----------------------------------------------------------------------
	class CamelEnemyDefaultAnim : public ObjState<CamereonEnemy>
	{
		CamelEnemyDefaultAnim()
		{}
	public:
		DECLARE_SINGLETON_INSTANCE(CamelEnemyDefaultAnim)
		virtual void Enter(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<CamereonEnemy>& Obj) override;
	};

	//----------------------------------------------------------------------
	/// Stayアニメーション(CamelEnemy)
	//----------------------------------------------------------------------
	class CamelEnemyStayAnim : public ObjState<CamereonEnemy>
	{
		CamelEnemyStayAnim()
		{}
	public:
		DECLARE_SINGLETON_INSTANCE(CamelEnemyStayAnim)
		virtual void Enter(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<CamereonEnemy>& Obj) override;
	};

	//----------------------------------------------------------------------
	/// Findアニメーション(CamelEnemy)
	//----------------------------------------------------------------------
	class CamelEnemyFindAnim : public ObjState<CamereonEnemy>
	{
		CamelEnemyFindAnim()
		{}
	public:
		DECLARE_SINGLETON_INSTANCE(CamelEnemyFindAnim)
		virtual void Enter(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<CamereonEnemy>& Obj) override;
	};
}