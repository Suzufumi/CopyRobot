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

		//リソース作成
		void CreateResourses();
		//スプライト作成
		void CreateWallSprite();
		//ビューの作成
		void CreateViewLight();
		//タイトルの制作
		void CreateTitle();
		//タイトルアニメーションの作成
		void CreateTitleAnime();

		shared_ptr<SoundItem> m_Audio;

	public: Title() : Stage() {}
			virtual ~Title();
			virtual void OnCreate()override;
			virtual void OnUpdate()override;
			virtual void OnDestroy()override;
	};

}
