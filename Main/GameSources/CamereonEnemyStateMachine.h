#pragma once
#include "stdafx.h"

namespace basecross
{
	//------------------------------------------------------------------------------
	/// �p�j�X�e�[�g
	//------------------------------------------------------------------------------
	class CamereonEnemyWander : public ObjState<CamereonEnemy>
	{
		CamereonEnemyWander()
		{}
	public:
		// �X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(CamereonEnemyWander)

		virtual void Enter(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<CamereonEnemy>& Obj) override;
	};

	//------------------------------------------------------------------------------
	/// �R�s�[����Ă���X�e�[�g
	//------------------------------------------------------------------------------
	class CamereonEnemyCopied : public ObjState<CamereonEnemy>
	{
		CamereonEnemyCopied()
		{}
	public:
		// �X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(CamereonEnemyCopied)

		virtual void Enter(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<CamereonEnemy>& Obj) override;
	};

	//------------------------------------------------------------------------------
	/// ChasePlayer�X�e�[�g
	//------------------------------------------------------------------------------
	class CamereonEnemyChasePlayer : public ObjState<CamereonEnemy>
	{
		CamereonEnemyChasePlayer()
		{}
	public:
		// �X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(CamereonEnemyChasePlayer)

		virtual void Enter(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<CamereonEnemy>& Obj) override;
	};

	//------------------------------------------------------------------------------
	///	LostPlayer�X�e�[�g
	//------------------------------------------------------------------------------
	class CamereonEnemyLostPlayer : public ObjState<CamereonEnemy>
	{
		CamereonEnemyLostPlayer()
		{}
	public:
		// �X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(CamereonEnemyLostPlayer)

		virtual void Enter(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<CamereonEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<CamereonEnemy>& Obj) override;
	};

}