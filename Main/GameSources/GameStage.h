/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage
	{
	protected:
		wstring m_StageXmlPath;

		//エフェクトのインターフェイス
		shared_ptr<EfkInterface> m_EfkInterface;

		//ビューとライトの作成
		virtual void CreateViewLight();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		//エフェクトのインターフェイスの取得
		shared_ptr<EfkInterface> GetEfkInterface() const {
			return m_EfkInterface;
		}
		// プレイヤーを作る
		void CreatePlayer();
		// 敵を作る
		void CreateEnemy();
		//床を作る
		void CreatePlane();
		//壁を作る
		void CreateWall();
		//岩を作る
		void CreateRock();
		//ゴールを作る
		void CreateGoal();
		void CreateTutorialBox();

		virtual void OnUpdate()override;
		//描画
		virtual void OnDraw() override;

		virtual void RenderStage()override;

		virtual void DrawStage() override;

		virtual void OnEvent(const shared_ptr<Event>& event)override;
		// セルマップの作成
		void CreateStageCellMap();
		// ボックスのコストをセルマップに反映
		template<typename T>
		void SetCellMapCost(wstring TagName);

		void StopAllEnemies();
		void ActiveAllEnemies();
		bool IsEnemyFindPlayer();
	};


}
//end basecross

