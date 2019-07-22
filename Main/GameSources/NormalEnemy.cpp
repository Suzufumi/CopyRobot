#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void NormalEnemy::OnCreate()
	{
		// �e�N���X��OnCreate���Ăяo��
		Enemy::OnCreate();
		// �ʏ�A�j���[�V�����̒���
		const int DefalutAnimLength = 119;
		const int StartFindAnimFlame = 120;
		// �v���C���[���������A�j���[�V�����̒���
		const int FindAnimLength = 39;
		const int StartChaseAnimFlame = 160;
		// �ǂ�������A�j���[�V�����̒���
		const int ChaseAnimLength = 79;
		const int StartReleaseFlame = 240;
		// �v���C���[�������������̃A�j���[�V�����̒���
		const int ReleaseAnimLength = 59;


		// �`��R���|�[�l���g�̐ݒ�
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		// ���b�V���̌`��̐ݒ�
		drawComp->SetMeshResource(L"ENEMY_MESH");
		drawComp->SetLineTextureResource(L"ENEMY_LINE_TX");
		drawComp->SetBlendState(BlendState::AlphaBlend);
		drawComp->SetOwnShadowActive(false);

		// �A�j���[�V�����̓o�^
		drawComp->AddAnimation(L"Default", 0, DefalutAnimLength, true);
		drawComp->AddAnimation(L"Find", StartFindAnimFlame, FindAnimLength, false);
		drawComp->AddAnimation(L"Chase", StartChaseAnimFlame, ChaseAnimLength, true);
		drawComp->AddAnimation(L"Release", StartReleaseFlame, ReleaseAnimLength, false);
		drawComp->ChangeCurrentAnimation(L"Default");
		// �X�e�[�g�}�V���̍\�z
		m_stateMachine.reset(new StateMachine<NormalEnemy>(GetThis<NormalEnemy>()));
		m_stateMachine->ChangeState(NormalEnemyWanderState::Instance());

		// �A�j���[�V�����X�e�[�g�}�V���̍\�z
		m_animStateMachine.reset(new StateMachine<NormalEnemy>(GetThis<NormalEnemy>()));
		m_animStateMachine->ChangeState(NormalEnemyDefaultAnim::Instance());

		// �^�O�t��
		AddTag(L"NormalEnemy");
	}

	void NormalEnemy::OnUpdate()
	{
		// �p�j�|�C���g�̃`�F�b�N
		CheckWayPointDistance();
		// �����s��(�G���m�������悤�ɐݒ�)
		SeparationEnemy();
		// StateMachine�̃A�b�v�f�[�g
		m_stateMachine->Update();
		// �A�j���[�V�����̍X�V
		UpdateAnimation();
	}

	void NormalEnemy::OnDraw()
	{
		Enemy::OnDraw();
	}

	// �A�j���[�V�����̍X�V
	void NormalEnemy::UpdateAnimation()
	{
		float delta = App::GetApp()->GetElapsedTime();

		// �A�j���[�V�����̃A�b�v�f�[�g
		m_animStateMachine->Update();

		GetDynamicComponent<SmBaseDraw>()->UpdateAnimation(delta);
		auto drawComp = GetComponent<PNTBoneModelDraw>();
		// �A�j���[�V�������I��������
		if (drawComp->IsTargetAnimeEnd())
		{
			// DefaultAnimation�ֈڍs����
			m_animStateMachine->ChangeState(NormalEnemyDefaultAnim::Instance());
		}

	}
}