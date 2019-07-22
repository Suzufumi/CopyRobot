#pragma once
#include "stdafx.h"

namespace basecross
{
	class GameClear :public Stage
	{
		//���\�[�X�쐬
		void CreateResourses();
		//�X�v���C�g�쐬
		void CreateWallSprite();
		//�A�j���[�V�����쐬
		void CreateAnimeSprite();
		//�r���[�̍쐬
		void CreateViewLight();
		//�N���A��ʂ̐���
		void CreateClear();
		void ChangeSelect(int num);

		vector<weak_ptr<AnimeSprite>> m_SpVec;
		unique_ptr<InputHandler> m_InputHandler;
		vector<weak_ptr<AnimeSprite>>m_Cursor;
		shared_ptr<SoundItem> m_Audio;
		float SelectPosY = 0.0f;
		bool m_IsStart=false;
		int SelectNo=1;

		weak_ptr<GameObject> m_Player;
		weak_ptr<GameObject> m_WallSprite;

	public: GameClear() : Stage(),
		m_SpVec(3) ,m_Cursor(1)
	{}
			virtual ~GameClear();
			virtual void OnCreate()override;
			virtual void OnUpdate()override;
			virtual void DrawStage()override;
		    void OnLStickDown(InputHandler::StickDownDir stickDownDir);
	};

}
