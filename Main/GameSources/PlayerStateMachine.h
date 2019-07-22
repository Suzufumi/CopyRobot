/*!
@file PlayerStateMachine.h
@brief �v���C���[�̃X�e�[�g�}�V��
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	///	�������Ȃ�
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
	///	�Q�[���I�[�o�[�X�e�[�g
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
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerNormalState : public ObjState<Player>
	{
		PlayerNormalState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerNormalState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//--------------------------------------------------------------------------------------
	///	�R�s�[�X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerCopyingState : public ObjState<Player>
	{
		PlayerCopyingState() {}

		Player::CopyType m_PrevType;
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerCopyingState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�R�A�ɃG�l�~�[����荞�ރX�e�[�g
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
	///	�J�����I��Default�X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerCamereonDefaultState : public ObjState<Player>
	{
		PlayerCamereonDefaultState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
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
	///	�S�[�����
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
	///	�X�^�[�g���
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