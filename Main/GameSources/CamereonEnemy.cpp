#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void CamereonEnemy::OnCreate()
	{
		// �e�N���X��OnCreate�Ăяo��
		Enemy::OnCreate();

		// �`��R���|�[�l���g�̐ݒ�
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		drawComp->SetMeshResource(L"CAMELEON_MESH");
		drawComp->SetLineTextureResource(L"CAMEL_LINE_TX");
		drawComp->SetBlendState(BlendState::AlphaBlend);
		drawComp->SetOwnShadowActive(false);
		// �ʏ펞�̃A�j���[�V�����̒���
		const int CamelDefaultAnimLength = 80;
		// �����Ă��鎞�̃A�j���[�V����
		const int CamelStayAnimStart = 161;
		const int CamelStayAnimLength = 159;
		// �v���C���[�����������̃A�j���[�V����
		const int CamelFindAnimStart = 321;
		const int CamelFindAnimLength = 27;

		// �A�j���[�V�����̐ݒ�
		drawComp->AddAnimation(L"CamelDefault", 0, CamelDefaultAnimLength, true, 90.0f);
		drawComp->AddAnimation(L"CamelStay", CamelStayAnimStart, CamelStayAnimLength, true);
		drawComp->AddAnimation(L"CamelFind", CamelFindAnimStart, CamelFindAnimLength, true, 60.0f);
		drawComp->ChangeCurrentAnimation(L"CamelDefault");
		// �X�e�[�g�}�V���̍\�z
		m_stateMachine.reset(new StateMachine<CamereonEnemy>(GetThis<CamereonEnemy>()));
		m_stateMachine->ChangeState(CamereonEnemyWander::Instance());
		// �A�j���[�V�����X�e�[�g�}�V���̍\�z
		m_camelAnimStateMachine.reset(new StateMachine<CamereonEnemy>(GetThis<CamereonEnemy>()));
		m_camelAnimStateMachine->ChangeState(CamelEnemyDefaultAnim::Instance());

		// �^�O�t��
		AddTag(L"CamereonEnemy");

	}

	void CamereonEnemy::OnUpdate()
	{
		// ���̜p�j�|�C���g�Ƃ̋����𒲂ׂ�
		CheckWayPointDistance();
		// �����s��(�G���m�������悤�ɐݒ�)
		SeparationEnemy();
		// StateMachine�̃A�b�v�f�[�g
		m_stateMachine->Update();
		// �A�j���[�V�����̍X�V
		UpdateAnimation();

	}

	void CamereonEnemy::OnDraw()
	{
		Enemy::OnDraw();

	}

	void CamereonEnemy::UpdateAnimation()
	{
		float delta = App::GetApp()->GetElapsedTime();

		// �A�j���[�V�����̃A�b�v�f�[�g
		m_camelAnimStateMachine->Update();
		GetDynamicComponent<SmBaseDraw>()->UpdateAnimation(delta);

	}
}