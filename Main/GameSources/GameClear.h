#pragma once
#include "stdafx.h"

namespace basecross
{
	class GameClear :public Stage
	{
		//リソース作成
		void CreateResourses();
		//スプライト作成
		void CreateWallSprite();
		//アニメーション作成
		void CreateAnimeSprite();
		//ビューの作成
		void CreateViewLight();
		//クリア画面の制作
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
