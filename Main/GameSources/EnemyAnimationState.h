#pragma once
#include "stdafx.h"

namespace basecross
{
	class NormalEnemy;
	//----------------------------------------------------------------------
	/// NormalEnemy�A�j���[�V����
	//----------------------------------------------------------------------

	//----------------------------------------------------------------------
	/// Default�A�j���[�V����
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
	/// Chase�A�j���[�V����
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
	/// Release�A�j���[�V����
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
	/// CamereonEnemy�A�j���[�V����
	//----------------------------------------------------------------------

	class CamereonEnemy;
	//----------------------------------------------------------------------
	/// Default�A�j���[�V����(CamelEnemy)
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
	/// Stay�A�j���[�V����(CamelEnemy)
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
	/// Find�A�j���[�V����(CamelEnemy)
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