#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void NormalEnemy::OnCreate()
	{
		// 親クラスのOnCreateを呼び出し
		Enemy::OnCreate();
		// 通常アニメーションの長さ
		const int DefalutAnimLength = 119;
		const int StartFindAnimFlame = 120;
		// プレイヤーを見つけたアニメーションの長さ
		const int FindAnimLength = 39;
		const int StartChaseAnimFlame = 160;
		// 追いかけるアニメーションの長さ
		const int ChaseAnimLength = 79;
		const int StartReleaseFlame = 240;
		// プレイヤーを見失った時のアニメーションの長さ
		const int ReleaseAnimLength = 59;


		// 描画コンポーネントの設定
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		// メッシュの形状の設定
		drawComp->SetMeshResource(L"ENEMY_MESH");
		drawComp->SetLineTextureResource(L"ENEMY_LINE_TX");
		drawComp->SetBlendState(BlendState::AlphaBlend);
		drawComp->SetOwnShadowActive(false);

		// アニメーションの登録
		drawComp->AddAnimation(L"Default", 0, DefalutAnimLength, true);
		drawComp->AddAnimation(L"Find", StartFindAnimFlame, FindAnimLength, false);
		drawComp->AddAnimation(L"Chase", StartChaseAnimFlame, ChaseAnimLength, true);
		drawComp->AddAnimation(L"Release", StartReleaseFlame, ReleaseAnimLength, false);
		drawComp->ChangeCurrentAnimation(L"Default");
		// ステートマシンの構築
		m_stateMachine.reset(new StateMachine<NormalEnemy>(GetThis<NormalEnemy>()));
		m_stateMachine->ChangeState(NormalEnemyWanderState::Instance());

		// アニメーションステートマシンの構築
		m_animStateMachine.reset(new StateMachine<NormalEnemy>(GetThis<NormalEnemy>()));
		m_animStateMachine->ChangeState(NormalEnemyDefaultAnim::Instance());

		// タグ付け
		AddTag(L"NormalEnemy");
	}

	void NormalEnemy::OnUpdate()
	{
		// 徘徊ポイントのチェック
		CheckWayPointDistance();
		// 分離行動(敵同士が離れるように設定)
		SeparationEnemy();
		// StateMachineのアップデート
		m_stateMachine->Update();
		// アニメーションの更新
		UpdateAnimation();
	}

	void NormalEnemy::OnDraw()
	{
		Enemy::OnDraw();
	}

	// アニメーションの更新
	void NormalEnemy::UpdateAnimation()
	{
		float delta = App::GetApp()->GetElapsedTime();

		// アニメーションのアップデート
		m_animStateMachine->Update();

		GetDynamicComponent<SmBaseDraw>()->UpdateAnimation(delta);
		auto drawComp = GetComponent<PNTBoneModelDraw>();
		// アニメーションが終了したら
		if (drawComp->IsTargetAnimeEnd())
		{
			// DefaultAnimationへ移行する
			m_animStateMachine->ChangeState(NormalEnemyDefaultAnim::Instance());
		}

	}
}