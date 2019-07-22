/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage
	{
	protected:
		wstring m_StageXmlPath;

		//�G�t�F�N�g�̃C���^�[�t�F�C�X
		shared_ptr<EfkInterface> m_EfkInterface;

		//�r���[�ƃ��C�g�̍쐬
		virtual void CreateViewLight();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
		//�G�t�F�N�g�̃C���^�[�t�F�C�X�̎擾
		shared_ptr<EfkInterface> GetEfkInterface() const {
			return m_EfkInterface;
		}
		// �v���C���[�����
		void CreatePlayer();
		// �G�����
		void CreateEnemy();
		//�������
		void CreatePlane();
		//�ǂ����
		void CreateWall();
		//������
		void CreateRock();
		//�S�[�������
		void CreateGoal();
		void CreateTutorialBox();

		virtual void OnUpdate()override;
		//�`��
		virtual void OnDraw() override;

		virtual void RenderStage()override;

		virtual void DrawStage() override;

		virtual void OnEvent(const shared_ptr<Event>& event)override;
		// �Z���}�b�v�̍쐬
		void CreateStageCellMap();
		// �{�b�N�X�̃R�X�g���Z���}�b�v�ɔ��f
		template<typename T>
		void SetCellMapCost(wstring TagName);

		void StopAllEnemies();
		void ActiveAllEnemies();
		bool IsEnemyFindPlayer();
	};


}
//end basecross

