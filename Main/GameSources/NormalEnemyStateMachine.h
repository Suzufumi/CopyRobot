#pragma once
#include "stdafx.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	///	�p�j�X�e�[�g
	//--------------------------------------------------------------------------------------
	class NormalEnemyWanderState : public ObjState<NormalEnemy>
	{
		NormalEnemyWanderState()
		{}
	public:
		// �X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(NormalEnemyWanderState)

		virtual void Enter(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<NormalEnemy>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	///	ChasePlayer�X�e�[�g
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
	///	LostPlayer�X�e�[�g
	//--------------------------------------------------------------------------------------
	class NormalEnemyLostPlayer : public ObjState<NormalEnemy>
	{
		NormalEnemyLostPlayer()
		{}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(NormalEnemyLostPlayer)

		virtual void Enter(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<NormalEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<NormalEnemy>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	/// Copied�X�e�[�g
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