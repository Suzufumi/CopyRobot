#pragma once
#include "stdafx.h"

namespace basecross
{
	class Over :public Stage 
	{
		//リソース作成
		void CreateResourses();
		//スプライト作成
		void CreateWallSprite();
		//アニメーション作成
		void CreateAnimeSprite();
		//ビューの作成
		void CreateViewLight();
		//ゲームオーバー画面の制作
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
