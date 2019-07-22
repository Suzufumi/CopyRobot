#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	///	徘徊ステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NormalEnemyWanderState)
	
	void NormalEnemyWanderState::Enter(const shared_ptr<NormalEnemy>& Obj)
	{
		Obj->GetComponent<Collision>()->SetFixed(false);
		Obj->SetEnemyState(Enemy::EnemyState::Normal);
	}

	void NormalEnemyWanderState::Execute(const shared_ptr<NormalEnemy>& Obj)
	{
		// プレイヤーを見つめた時間を取得
		unsigned int findTime = Obj->GetFindTime();
		// 現在のWayPoint(目的地)を取得
		auto targetPos = Obj->GetWayPoint();
		// 徘徊行動
		Obj->GetCellSearchBehavior()->WanderingBehavior(targetPos);
		// プレイヤーを見つけたら
		if (Obj->CheckFindPlayer())
		{
			// 見つめた時間の設定
			Obj->SetFindTime(++findTime);
		}
		// プレイヤーを見つめた時間が一定以上になったら
		if (findTime >= Obj->GetFindPlayerTime())
		{
			// ステートの切り替え
			Obj->GetStateMachine()->ChangeState(NormalEnemyChasePlayer::Instance());
		}
		
		if (!Obj->GetIsAlived())
		{
			Obj->GetStateMachine()->ChangeState(NormalEnemyCopied::Instance());
		}

	}

	void NormalEnemyWanderState::Exit(const shared_ptr<NormalEnemy>& Obj)
	{
		auto rigidComp = Obj->GetComponent<Rigidbody>();
		rigidComp->SetVelocityZero();
		Obj->SetFindTime(0);
	}


	//--------------------------------------------------------------------------------------
	///	ChasePlayerステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NormalEnemyChasePlayer)

	void NormalEnemyChasePlayer::Enter(const shared_ptr<NormalEnemy>& Obj)
	{
		const Col4 SPRITECOLOR = Col4(1.0f, 1.0f, 1.0f, 0.8f);
		const float SE_VOLUME = 0.6f;
		// アニメーションの切り替え
		Obj->GetAnimStateMachine()->ChangeState(NormalEnemyChaseAnim::Instance());
		// SEの再生
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Start(L"WARNING_SE", 0, SE_VOLUME);
		// スプライトの表示
		auto redSpritePtr = Obj->GetRedLineSprite();
		redSpritePtr.lock()->SetDiffuse(SPRITECOLOR);

		Obj->SetEnemyState(Enemy::EnemyState::Chase);
	}

	void NormalEnemyChasePlayer::Execute(const shared_ptr<NormalEnemy>& Obj)
	{
		// 行動の秒数カウント
		unsigned int secondCount = Obj->GetBehaviorConut();
		Obj->SetBehaviorCount(++secondCount);
		// PlayerのPositionを取得
		auto playerPos = Obj->GetPlayerPos();
		// 追跡行動
		Obj->GetCellSearchBehavior()->ChasePlayer(playerPos);
		// Enemyの視界を取得
		auto deg = Obj->GetEnemyEyeSight(playerPos);
		// プレイヤーの現在のStateMachineを取得
		auto playerCurrentState = Obj->GetPlayerCurrentStateMachine();
		
		if (Rad2Deg(deg) > Obj->GetEyesightLimit() && playerCurrentState == PlayerCamereonAssimililation::Instance() ||
			Rad2Deg(deg) > Obj->GetEyesightLimit() && secondCount >= Obj->GetChaseTime())
		{
			Obj->GetStateMachine()->ChangeState(NormalEnemyLostPlayer::Instance());
		}

		if (!Obj->GetIsAlived())
		{
			Obj->GetStateMachine()->ChangeState(NormalEnemyCopied::Instance());
		}
	}

	void NormalEnemyChasePlayer::Exit(const shared_ptr<NormalEnemy>& Obj)
	{
		// 秒数のリセット
		Obj->SetBehaviorCount(0);
	}


	//--------------------------------------------------------------------------------------
	///	LostPlayerステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NormalEnemyLostPlayer)

	void NormalEnemyLostPlayer::Enter(const shared_ptr<NormalEnemy>& Obj)
	{
		// アニメーションの切り替え
		Obj->GetAnimStateMachine()->ChangeState(NormalEnemyReleaseAnim::Instance());
		// スプライトのポインタを取得
		auto redSpritePtr = Obj->GetRedLineSprite();
		// スプライトのフェードアウト
		redSpritePtr.lock()->FadeOut(1.0f);
		auto rigidComp = Obj->GetComponent<Rigidbody>();
		rigidComp->SetVelocityZero();

		Obj->SetEnemyState(Enemy::EnemyState::Lost);

	}

	void NormalEnemyLostPlayer::Execute(const shared_ptr<NormalEnemy>& Obj)
	{
		// 行動の秒数カウント
		unsigned int secondCount = Obj->GetBehaviorConut();
		Obj->SetBehaviorCount(++secondCount);
		// 秒数が一定以上になったら
		if (secondCount >= Obj->GetLostPlayerTime())
		{
			// 徘徊行動へ戻る
			Obj->GetStateMachine()->ChangeState(NormalEnemyWanderState::Instance());
		}

		if (!Obj->GetIsAlived())
		{
			Obj->GetStateMachine()->ChangeState(NormalEnemyCopied::Instance());
		}

	}

	void NormalEnemyLostPlayer::Exit(const shared_ptr<NormalEnemy>& Obj)
	{
		Obj->SetBehaviorCount(0);
	}

	//--------------------------------------------------------------------------------------
	///	Copiedステート
	//--------------------------------------------------------------------------------------

	IMPLEMENT_SINGLETON_INSTANCE(NormalEnemyCopied)

	void NormalEnemyCopied::Enter(const shared_ptr<NormalEnemy>& Obj)
	{
		const float FADEOUT_TIME = 3.0f;
		const float RESPAWN_TIME = 7.0f;
		if (Obj->GetStateMachine()->GetPreviousState() == NormalEnemyChasePlayer::Instance())
		{
			Obj->GetAnimStateMachine()->ChangeState(NormalEnemyDefaultAnim::Instance());
			auto redSpritePtr = Obj->GetRedLineSprite();
			// スプライトのフェードアウト
			redSpritePtr.lock()->FadeOut(FADEOUT_TIME);
		}
		// リスポーンを7秒後に実行
		Obj->CreatePostEvent(RESPAWN_TIME, L"Respawn");
		Obj->SetEnemyState(Enemy::EnemyState::Copied);
	}

	void NormalEnemyCopied::Execute(const shared_ptr<NormalEnemy>& Obj)
	{
		// 行動の秒数カウント
		unsigned int secondCount = Obj->GetBehaviorConut();
		Obj->SetBehaviorCount(++secondCount);
		// 一定以上の秒数になったら
		if (Obj->GetIsAlived() && secondCount >= Obj->GetReturnWanderTime())
		{
			Obj->GetStateMachine()->ChangeState(NormalEnemyWanderState::Instance());
		}
	}
	void NormalEnemyCopied::Exit(const shared_ptr<NormalEnemy>& Obj)
	{
		Obj->SetBehaviorCount(0);
	}

}