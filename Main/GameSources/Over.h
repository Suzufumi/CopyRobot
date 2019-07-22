#pragma once
#include "stdafx.h"

namespace basecross
{
	class Over :public Stage 
	{
		//���\�[�X�쐬
		void CreateResourses();
		//�X�v���C�g�쐬
		void CreateWallSprite();
		//�A�j���[�V�����쐬
		void CreateAnimeSprite();
		//�r���[�̍쐬
		void CreateViewLight();
		//�Q�[���I�[�o�[��ʂ̐���
		void CreateOver();
		void ChangeSelect(int num);

		vector<weak_ptr<AnimeSprite>> m_SpVec;
		unique_ptr<InputHandler> m_InputHandler;
		bool m_IsStart=false;
		int SelectNo=2;
		float SelectPosY = -160.0f;
		vector<weak_ptr<AnimeSprite>>m_Cursor;
		shared_ptr<SoundItem> m_Audio;
		weak_ptr<GameObject> m_Player;

	public: Over() : Stage(),
		m_SpVec(2), m_Cursor(1)
	{}
			virtual ~Over();
			virtual void OnCreate()override;
			virtual void OnUpdate()override;
			void OnLStickDown(InputHandler::StickDownDir stickDownDir);
	};

}
