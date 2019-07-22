#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	unsigned int secondCount = 0;
	// �����鎞��
	unsigned int FindTime = 0;
	//------------------------------------------------------------------------------
	/// �p�j�X�e�[�g
	//------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CamereonEnemyWander)

	void CamereonEnemyWander::Enter(const shared_ptr<CamereonEnemy>& Obj)
	{
		// �Œ�Փ˃I�u�W�F�N�g�ɂ��邩�ǂ���
		Obj->GetComponent<Collision>()->SetFixed(false);
		// �X�e�[�g��Normal�ɐݒ�
		Obj->SetEnemyState(Enemy::EnemyState::Normal);
	}

	void CamereonEnemyWander::Execute(const shared_ptr<CamereonEnemy>& Obj)
	{
		// �p�j�|�C���g�̎擾
		auto wayPoint = Obj->GetWayPoint();
		// �p�j�s��
		Obj->GetCellSearchBehavior()->WanderingBehavior(wayPoint);
		
		// ���S�t���O��false�Ȃ�
		if (!Obj->GetIsAlived())
		{
			// StateMachine�̐؂�ւ�
			Obj->GetStateMachine()->ChangeState(CamereonEnemyCopied::Instance());
		}
		
		// �v���C���[���߂��ɂ��邩�ǂ���
		if (Obj->CheckFindPlayer())
		{	
			// �����鎞�Ԃ��v���X
			FindTime++;
		}

		if (FindTime >= Obj->GetFindPlayerTime())
		{
			// StateMachine�̐؂�ւ�
			Obj->GetStateMachine()->ChangeState(CamereonEnemyChasePlayer::Instance());
		}
	}

	void CamereonEnemyWander::Exit(const shared_ptr<CamereonEnemy>& Obj)
	{		
		// �ϐ��̏�����
		FindTime = 0;
		auto rigidComp = Obj->GetComponent<Rigidbody>();
		// velocity�̃��Z�b�g
		rigidComp->SetVelocityZero();
	}

	//------------------------------------------------------------------------------
	/// �R�s�[���ꂽ�X�e�[�g
	//------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CamereonEnemyCopied)


	void CamereonEnemyCopied::Enter(const shared_ptr<CamereonEnemy>& Obj)
	{
		const float FADEOUT_TIME = 3.0f;
		const float RESPAWN_TIME = 7.0f;
		// �ȑO�̃X�e�[�g��Chase��������
		if(Obj->GetStateMachine()->GetPreviousState() == CamereonEnemyChasePlayer::Instance())
		{
			// �X�v���C�g�𓧖��ɂ���
			auto redSpritePtr = Obj->GetRedLineSprite();
			redSpritePtr.lock()->FadeOut(FADEOUT_TIME);
		}
		// �X�e�[�g��Copied�ɐݒ�
		Obj->SetEnemyState(Enemy::EnemyState::Copied);
		// ���X�|�[���̏�����7�b��ɌĂ�
		Obj->CreatePostEvent(RESPAWN_TIME, L"Respawn");
		// �A�j���[�V�����̐؂�ւ�
		Obj->GetCamelAnimStateMachine()->ChangeState(CamelEnemyStayAnim::Instance());
	}

	void CamereonEnemyCopied::Execute(const shared_ptr<CamereonEnemy>& Obj)
	{
		secondCount++;
		// �����Ă��邩�w��̕b���ɂȂ�����
		if (Obj->GetIsAlived() && secondCount >= Obj->GetReturnWanderTime())
		{
			// StateMachine�̐؂�ւ�
			Obj->GetStateMachine()->ChangeState(CamereonEnemyWander::Instance());
		}
	}

	void CamereonEnemyCopied::Exit(const shared_ptr<CamereonEnemy>& Obj)
	{
		// �ϐ���������
		secondCount = 0;
		// �A�j���[�V�����̐؂�ւ�
		Obj->GetCamelAnimStateMachine()->ChangeState(CamelEnemyDefaultAnim::Instance());
	}

	//------------------------------------------------------------------------------
	/// ChasePlayer�X�e�[�g
	//------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CamereonEnemyChasePlayer)


	void CamereonEnemyChasePlayer::Enter(const shared_ptr<CamereonEnemy>& Obj)
	{
		const float SE_VOLUME = 0.6f;
		const Col4 SPRITECOLOR = Col4(1.0f, 1.0f, 1.0f, 0.8f);
		// �X�e�[�g��Chase�ɐݒ�
		Obj->SetEnemyState(Enemy::EnemyState::Chase);
		auto xaPtr = App::GetApp()->GetXAudio2Manager();
		// ���ʉ��̍Đ�
		xaPtr->Start(L"WARNING_SE", 0, SE_VOLUME);
		// ��ʂ�Ԃ�����
		auto redSpritePtr = Obj->GetRedLineSprite();
		redSpritePtr.lock()->SetDiffuse(SPRITECOLOR);
		// �A�j���[�V�����̐؂�ւ�
		Obj->GetCamelAnimStateMachine()->ChangeState(CamelEnemyFindAnim::Instance());
	}

	void CamereonEnemyChasePlayer::Execute(const shared_ptr<CamereonEnemy>& Obj)
	{
		secondCount++;
		// �v���C���[��Position���擾
		auto playerPos = Obj->GetPlayerPos();
		// �ǐՍs���̐ݒ�
		Obj->GetCellSearchBehavior()->ChasePlayer(playerPos);
		// ���g�̎��E���擾
		float deg = Obj->GetEnemyEyeSight(playerPos);
		// �v���C���[��StateMachine���擾
		auto playerCurrentState = Obj->GetPlayerCurrentStateMachine();
		// �v���C���[�̎��E����O�ꂽ�APlayer���ϐg���Ă�����
		if (Rad2Deg(deg) > Obj->GetEyesightLimit() && playerCurrentState == PlayerCamereonAssimililation::Instance() ||
			Rad2Deg(deg) > Obj->GetEyesightLimit() && secondCount >= Obj->GetChaseTime())
		{
			// StateMachine�̐؂�ւ�
			Obj->GetStateMachine()->ChangeState(CamereonEnemyLostPlayer::Instance());
		}
		// ���S�t���O��false�Ȃ�
		if (!Obj->GetIsAlived())
		{
			Obj->GetStateMachine()->ChangeState(CamereonEnemyCopied::Instance());
		}

	}

	void CamereonEnemyChasePlayer::Exit(const shared_ptr<CamereonEnemy>& Obj)
	{
		secondCount = 0;
	}

	//------------------------------------------------------------------------------
	/// LostPlayer�X�e�[�g
	//------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CamereonEnemyLostPlayer)


	void CamereonEnemyLostPlayer::Enter(const shared_ptr<CamereonEnemy>& Obj)
	{
		// �X�e�[�g��Lost�ɐݒ�
		Obj->SetEnemyState(Enemy::EnemyState::Lost);
		// �Ԃ��X�v���C�g�̓�����
		auto redSpritePtr = Obj->GetRedLineSprite();
		redSpritePtr.lock()->FadeOut(1.0f);
		// Rigidbody�R���|�[�l���g�̎擾
		auto rigidComp = Obj->GetComponent<Rigidbody>();
		// velocity��0�ɂ���
		rigidComp->SetVelocityZero();
	}

	void CamereonEnemyLostPlayer::Execute(const shared_ptr<CamereonEnemy>& Obj)
	{
		secondCount++;
		// �T�b�ȓ����A�v���C���[����������
		if (secondCount >= Obj->GetLostPlayerTime())
		{

			Obj->GetStateMachine()->ChangeState(CamereonEnemyWander::Instance());
		}

		if (!Obj->GetIsAlived())
		{
			Obj->GetStateMachine()->ChangeState(CamereonEnemyCopied::Instance());
		}

	}

	void CamereonEnemyLostPlayer::Exit(const shared_ptr<CamereonEnemy>& Obj)
	{
		secondCount = 0;
		Obj->GetCamelAnimStateMachine()->ChangeState(CamelEnemyDefaultAnim::Instance());
	}


}