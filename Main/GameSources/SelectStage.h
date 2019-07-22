#pragma once
#include "stdafx.h"

namespace basecross
{
	class AnimeSprite;

	class SelectStage : public Stage
	{

		void CreateViewLight();
		//スプライト作成
		void CreateScrollSprite();
		//void CreateAnimeSprite();
		void CreateArrowSprite();
		void CreateStageSelectSquare();
		bool m_CntrolLock;		
		bool m_IsStart = false;
		vector<weak_ptr<AnimeSprite>> m_SpVec;
		void ChangeSelect(int num);
		void CreateSelectCharacter();
		void CreateSelectRing();
		//文字列の表示
		void DrawStrings();
		shared_ptr<GameObject> m_DrawOrder;
		weak_ptr<GameObject> m_Ring;
		weak_ptr<GameObject> m_L_Arrow;
		weak_ptr<GameObject> m_R_Arrow;
		vector<weak_ptr<GameObject>> m_Squares;
		weak_ptr<GameObject> m_Square;
		shared_ptr<SoundItem> m_Audio;
		bool m_IsFirstUpdate = true;
	public:
		SelectStage() :Stage(), m_CntrolLock(false),
			m_SpVec(3)
		{
		}
		virtual ~SelectStage() {}
		// 構築
		virtual void OnCreate()override;
		// 更新
		virtual void OnUpdate()override;

		virtual void DrawStage()override;
	};

}