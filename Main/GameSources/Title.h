#pragma once
#include "stdafx.h"

namespace basecross 
{
	class Title :public Stage
	{
		shared_ptr<SoundItem> m_BGM;
		weak_ptr<GameObject> m_Player;
		float m_PlayerAnimCount = 0.0f;
		bool m_IsStart = false;

		//���\�[�X�쐬
		void CreateResourses();
		//�X�v���C�g�쐬
		void CreateWallSprite();
		//�r���[�̍쐬
		void CreateViewLight();
		//�^�C�g���̐���
		void CreateTitle();
		//�^�C�g���A�j���[�V�����̍쐬
		void CreateTitleAnime();

		shared_ptr<SoundItem> m_Audio;

	public: Title() : Stage() {}
			virtual ~Title();
			virtual void OnCreate()override;
			virtual void OnUpdate()override;
			virtual void OnDestroy()override;
	};

}
