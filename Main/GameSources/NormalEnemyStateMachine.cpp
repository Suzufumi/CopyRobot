#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	///	�p�j�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NormalEnemyWanderState)
	
	void NormalEnemyWanderState::Enter(const shared_ptr<NormalEnemy>& Obj)
	{
		Obj->GetComponent<Collision>()->SetFixed(false);
		Obj->SetEnemyState(Enemy::EnemyState::Normal);
	}

	void NormalEnemyWanderState::Execute(const shared_ptr<NormalEnemy>& Obj)
	{
		// �v���C���[�����߂����Ԃ��擾
		unsigned int findTime = Obj->GetFindTime();
		// ���݂�WayPoint(�ړI�n)���擾
		auto targetPos = Obj->GetWayPoint();
		// �p�j�s��
		Obj->GetCellSearchBehavior()->WanderingBehavior(targetPos);
		// �v���C���[����������
		if (Obj->CheckFindPlayer())
		{
			// ���߂����Ԃ̐ݒ�
			Obj->SetFindTime(++findTime);
		}
		// �v���C���[�����߂����Ԃ����ȏ�ɂȂ�����
		if (findTime >= Obj->GetFindPlayerTime())
		{
			// �X�e�[�g�̐؂�ւ�
			Obj->GetStateMachine()->ChangeState(NormalEnemyChasePlayer::Instance());
		}
		
		if (!Obj->GetIsAlived())
		{
			Obj->GetStateMachine()->ChangeState(NormalEnemyCopied::Instance());
		}

	}

	void NormalEnemyWanderState::Exit(const shared_ptr<NormalEnemy>& Obj)
	{
		auto rigidComp = Obj->GetComponent<Rigidbody>();
		rigidComp->SetVelocityZero();
		Obj->SetFindTime(0);
	}


	//--------------------------------------------------------------------------------------
	///	ChasePlayer�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NormalEnemyChasePlayer)

	void NormalEnemyChasePlayer::Enter(const shared_ptr<NormalEnemy>& Obj)
	{
		const Col4 SPRITECOLOR = Col4(1.0f, 1.0f, 1.0f, 0.8f);
		const float SE_VOLUME = 0.6f;
		// �A�j���[�V�����̐؂�ւ�
		Obj->GetAnimStateMachine()->ChangeState(NormalEnemyChaseAnim::Instance());
		// SE�̍Đ�
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Start(L"WARNING_SE", 0, SE_VOLUME);
		// �X�v���C�g�̕\��
		auto redSpritePtr = Obj->GetRedLineSprite();
		redSpritePtr.lock()->SetDiffuse(SPRITECOLOR);

		Obj->SetEnemyState(Enemy::EnemyState::Chase);
	}

	void NormalEnemyChasePlayer::Execute(const shared_ptr<NormalEnemy>& Obj)
	{
		// �s���̕b���J�E���g
		unsigned int secondCount = Obj->GetBehaviorConut();
		Obj->SetBehaviorCount(++secondCount);
		// Player��Position���擾
		auto playerPos = Obj->GetPlayerPos();
		// �ǐՍs��
		Obj->GetCellSearchBehavior()->ChasePlayer(playerPos);
		// Enemy�̎��E���擾
		auto deg = Obj->GetEnemyEyeSight(playerPos);
		// �v���C���[�̌��݂�StateMachine���擾
		auto playerCurrentState = Obj->GetPlayerCurrentStateMachine();
		
		if (Rad2Deg(deg) > Obj->GetEyesightLimit() && playerCurrentState == PlayerCamereonAssimililation::Instance() ||
			Rad2Deg(deg) > Obj->GetEyesightLimit() && secondCount >= Obj->GetChaseTime())
		{
			Obj->GetStateMachine()->ChangeState(NormalEnemyLostPlayer::Instance());
		}

		if (!Obj->GetIsAlived())
		{
			Obj->GetStateMachine()->ChangeState(NormalEnemyCopied::Instance());
		}
	}

	void NormalEnemyChasePlayer::Exit(const shared_ptr<NormalEnemy>& Obj)
	{
		// �b���̃��Z�b�g
		Obj->SetBehaviorCount(0);
	}


	//--------------------------------------------------------------------------------------
	///	LostPlayer�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NormalEnemyLostPlayer)

	void NormalEnemyLostPlayer::Enter(const shared_ptr<NormalEnemy>& Obj)
	{
		// �A�j���[�V�����̐؂�ւ�
		Obj->GetAnimStateMachine()->ChangeState(NormalEnemyReleaseAnim::Instance());
		// �X�v���C�g�̃|�C���^���擾
		auto redSpritePtr = Obj->GetRedLineSprite();
		// �X�v���C�g�̃t�F�[�h�A�E�g
		redSpritePtr.lock()->FadeOut(1.0f);
		auto rigidComp = Obj->GetComponent<Rigidbody>();
		rigidComp->SetVelocityZero();

		Obj->SetEnemyState(Enemy::EnemyState::Lost);

	}

	void NormalEnemyLostPlayer::Execute(const shared_ptr<NormalEnemy>& Obj)
	{
		// �s���̕b���J�E���g
		unsigned int secondCount = Obj->GetBehaviorConut();
		Obj->SetBehaviorCount(++secondCount);
		// �b�������ȏ�ɂȂ�����
		if (secondCount >= Obj->GetLostPlayerTime())
		{
			// �p�j�s���֖߂�
			Obj->GetStateMachine()->ChangeState(NormalEnemyWanderState::Instance());
		}

		if (!Obj->GetIsAlived())
		{
			Obj->GetStateMachine()->ChangeState(NormalEnemyCopied::Instance());
		}

	}

	void NormalEnemyLostPlayer::Exit(const shared_ptr<NormalEnemy>& Obj)
	{
		Obj->SetBehaviorCount(0);
	}

	//--------------------------------------------------------------------------------------
	///	Copied�X�e�[�g
	//--------------------------------------------------------------------------------------

	IMPLEMENT_SINGLETON_INSTANCE(NormalEnemyCopied)

	void NormalEnemyCopied::Enter(const shared_ptr<NormalEnemy>& Obj)
	{
		const float FADEOUT_TIME = 3.0f;
		const float RESPAWN_TIME = 7.0f;
		if (Obj->GetStateMachine()->GetPreviousState() == NormalEnemyChasePlayer::Instance())
		{
			Obj->GetAnimStateMachine()->ChangeState(NormalEnemyDefaultAnim::Instance());
			auto redSpritePtr = Obj->GetRedLineSprite();
			// �X�v���C�g�̃t�F�[�h�A�E�g
			redSpritePtr.lock()->FadeOut(FADEOUT_TIME);
		}
		// ���X�|�[����7�b��Ɏ��s
		Obj->CreatePostEvent(RESPAWN_TIME, L"Respawn");
		Obj->SetEnemyState(Enemy::EnemyState::Copied);
	}

	void NormalEnemyCopied::Execute(const shared_ptr<NormalEnemy>& Obj)
	{
		// �s���̕b���J�E���g
		unsigned int secondCount = Obj->GetBehaviorConut();
		Obj->SetBehaviorCount(++secondCount);
		// ���ȏ�̕b���ɂȂ�����
		if (Obj->GetIsAlived() && secondCount >= Obj->GetReturnWanderTime())
		{
			Obj->GetStateMachine()->ChangeState(NormalEnemyWanderState::Instance());
		}
	}
	void NormalEnemyCopied::Exit(const shared_ptr<NormalEnemy>& Obj)
	{
		Obj->SetBehaviorCount(0);
	}

}