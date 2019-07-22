#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void CamereonEnemy::OnCreate()
	{
		// 親クラスのOnCreate呼び出し
		Enemy::OnCreate();

		// 描画コンポーネントの設定
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		drawComp->SetMeshResource(L"CAMELEON_MESH");
		drawComp->SetLineTextureResource(L"CAMEL_LINE_TX");
		drawComp->SetBlendState(BlendState::AlphaBlend);
		drawComp->SetOwnShadowActive(false);
		// 通常時のアニメーションの長さ
		const int CamelDefaultAnimLength = 80;
		// 歩いている時のアニメーション
		const int CamelStayAnimStart = 161;
		const int CamelStayAnimLength = 159;
		// プレイヤーを見つけた時のアニメーション
		const int CamelFindAnimStart = 321;
		const int CamelFindAnimLength = 27;

		// アニメーションの設定
		drawComp->AddAnimation(L"CamelDefault", 0, CamelDefaultAnimLength, true, 90.0f);
		drawComp->AddAnimation(L"CamelStay", CamelStayAnimStart, CamelStayAnimLength, true);
		drawComp->AddAnimation(L"CamelFind", CamelFindAnimStart, CamelFindAnimLength, true, 60.0f);
		drawComp->ChangeCurrentAnimation(L"CamelDefault");
		// ステートマシンの構築
		m_stateMachine.reset(new StateMachine<CamereonEnemy>(GetThis<CamereonEnemy>()));
		m_stateMachine->ChangeState(CamereonEnemyWander::Instance());
		// アニメーションステートマシンの構築
		m_camelAnimStateMachine.reset(new StateMachine<CamereonEnemy>(GetThis<CamereonEnemy>()));
		m_camelAnimStateMachine->ChangeState(CamelEnemyDefaultAnim::Instance());

		// タグ付け
		AddTag(L"CamereonEnemy");

	}

	void CamereonEnemy::OnUpdate()
	{
		// 次の徘徊ポイントとの距離を調べる
		CheckWayPointDistance();
		// 分離行動(敵同士が離れるように設定)
		SeparationEnemy();
		// StateMachineのアップデート
		m_stateMachine->Update();
		// アニメーションの更新
		UpdateAnimation();

	}

	void CamereonEnemy::OnDraw()
	{
		Enemy::OnDraw();

	}

	void CamereonEnemy::UpdateAnimation()
	{
		float delta = App::GetApp()->GetElapsedTime();

		// アニメーションのアップデート
		m_camelAnimStateMachine->Update();
		GetDynamicComponent<SmBaseDraw>()->UpdateAnimation(delta);

	}
}