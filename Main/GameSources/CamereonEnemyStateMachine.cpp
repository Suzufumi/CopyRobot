#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	unsigned int secondCount = 0;
	// 見つける時間
	unsigned int FindTime = 0;
	//------------------------------------------------------------------------------
	/// 徘徊ステート
	//------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CamereonEnemyWander)

	void CamereonEnemyWander::Enter(const shared_ptr<CamereonEnemy>& Obj)
	{
		// 固定衝突オブジェクトにするかどうか
		Obj->GetComponent<Collision>()->SetFixed(false);
		// ステートをNormalに設定
		Obj->SetEnemyState(Enemy::EnemyState::Normal);
	}

	void CamereonEnemyWander::Execute(const shared_ptr<CamereonEnemy>& Obj)
	{
		// 徘徊ポイントの取得
		auto wayPoint = Obj->GetWayPoint();
		// 徘徊行動
		Obj->GetCellSearchBehavior()->WanderingBehavior(wayPoint);
		
		// 死亡フラグがfalseなら
		if (!Obj->GetIsAlived())
		{
			// StateMachineの切り替え
			Obj->GetStateMachine()->ChangeState(CamereonEnemyCopied::Instance());
		}
		
		// プレイヤーが近くにいるかどうか
		if (Obj->CheckFindPlayer())
		{	
			// 見つける時間をプラス
			FindTime++;
		}

		if (FindTime >= Obj->GetFindPlayerTime())
		{
			// StateMachineの切り替え
			Obj->GetStateMachine()->ChangeState(CamereonEnemyChasePlayer::Instance());
		}
	}

	void CamereonEnemyWander::Exit(const shared_ptr<CamereonEnemy>& Obj)
	{		
		// 変数の初期化
		FindTime = 0;
		auto rigidComp = Obj->GetComponent<Rigidbody>();
		// velocityのリセット
		rigidComp->SetVelocityZero();
	}

	//------------------------------------------------------------------------------
	/// コピーされたステート
	//------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CamereonEnemyCopied)


	void CamereonEnemyCopied::Enter(const shared_ptr<CamereonEnemy>& Obj)
	{
		const float FADEOUT_TIME = 3.0f;
		const float RESPAWN_TIME = 7.0f;
		// 以前のステートがChaseだったら
		if(Obj->GetStateMachine()->GetPreviousState() == CamereonEnemyChasePlayer::Instance())
		{
			// スプライトを透明にする
			auto redSpritePtr = Obj->GetRedLineSprite();
			redSpritePtr.lock()->FadeOut(FADEOUT_TIME);
		}
		// ステートをCopiedに設定
		Obj->SetEnemyState(Enemy::EnemyState::Copied);
		// リスポーンの処理を7秒後に呼ぶ
		Obj->CreatePostEvent(RESPAWN_TIME, L"Respawn");
		// アニメーションの切り替え
		Obj->GetCamelAnimStateMachine()->ChangeState(CamelEnemyStayAnim::Instance());
	}

	void CamereonEnemyCopied::Execute(const shared_ptr<CamereonEnemy>& Obj)
	{
		secondCount++;
		// 生きているかつ指定の秒数になったら
		if (Obj->GetIsAlived() && secondCount >= Obj->GetReturnWanderTime())
		{
			// StateMachineの切り替え
			Obj->GetStateMachine()->ChangeState(CamereonEnemyWander::Instance());
		}
	}

	void CamereonEnemyCopied::Exit(const shared_ptr<CamereonEnemy>& Obj)
	{
		// 変数を初期化
		secondCount = 0;
		// アニメーションの切り替え
		Obj->GetCamelAnimStateMachine()->ChangeState(CamelEnemyDefaultAnim::Instance());
	}

	//------------------------------------------------------------------------------
	/// ChasePlayerステート
	//------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CamereonEnemyChasePlayer)


	void CamereonEnemyChasePlayer::Enter(const shared_ptr<CamereonEnemy>& Obj)
	{
		const float SE_VOLUME = 0.6f;
		const Col4 SPRITECOLOR = Col4(1.0f, 1.0f, 1.0f, 0.8f);
		// ステートをChaseに設定
		Obj->SetEnemyState(Enemy::EnemyState::Chase);
		auto xaPtr = App::GetApp()->GetXAudio2Manager();
		// 効果音の再生
		xaPtr->Start(L"WARNING_SE", 0, SE_VOLUME);
		// 画面を赤くする
		auto redSpritePtr = Obj->GetRedLineSprite();
		redSpritePtr.lock()->SetDiffuse(SPRITECOLOR);
		// アニメーションの切り替え
		Obj->GetCamelAnimStateMachine()->ChangeState(CamelEnemyFindAnim::Instance());
	}

	void CamereonEnemyChasePlayer::Execute(const shared_ptr<CamereonEnemy>& Obj)
	{
		secondCount++;
		// プレイヤーのPositionを取得
		auto playerPos = Obj->GetPlayerPos();
		// 追跡行動の設定
		Obj->GetCellSearchBehavior()->ChasePlayer(playerPos);
		// 自身の視界を取得
		float deg = Obj->GetEnemyEyeSight(playerPos);
		// プレイヤーのStateMachineを取得
		auto playerCurrentState = Obj->GetPlayerCurrentStateMachine();
		// プレイヤーの視界から外れた、Playerが変身していたら
		if (Rad2Deg(deg) > Obj->GetEyesightLimit() && playerCurrentState == PlayerCamereonAssimililation::Instance() ||
			Rad2Deg(deg) > Obj->GetEyesightLimit() && secondCount >= Obj->GetChaseTime())
		{
			// StateMachineの切り替え
			Obj->GetStateMachine()->ChangeState(CamereonEnemyLostPlayer::Instance());
		}
		// 死亡フラグがfalseなら
		if (!Obj->GetIsAlived())
		{
			Obj->GetStateMachine()->ChangeState(CamereonEnemyCopied::Instance());
		}

	}

	void CamereonEnemyChasePlayer::Exit(const shared_ptr<CamereonEnemy>& Obj)
	{
		secondCount = 0;
	}

	//------------------------------------------------------------------------------
	/// LostPlayerステート
	//------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CamereonEnemyLostPlayer)


	void CamereonEnemyLostPlayer::Enter(const shared_ptr<CamereonEnemy>& Obj)
	{
		// ステートをLostに設定
		Obj->SetEnemyState(Enemy::EnemyState::Lost);
		// 赤いスプライトの透明化
		auto redSpritePtr = Obj->GetRedLineSprite();
		redSpritePtr.lock()->FadeOut(1.0f);
		// Rigidbodyコンポーネントの取得
		auto rigidComp = Obj->GetComponent<Rigidbody>();
		// velocityを0にする
		rigidComp->SetVelocityZero();
	}

	void CamereonEnemyLostPlayer::Execute(const shared_ptr<CamereonEnemy>& Obj)
	{
		secondCount++;
		// ５秒以内かつ、プレイヤーを見つけたら
		if (secondCount >= Obj->GetLostPlayerTime())
		{

			Obj->GetStateMachine()->ChangeState(CamereonEnemyWander::Instance());
		}

		if (!Obj->GetIsAlived())
		{
			Obj->GetStateMachine()->ChangeState(CamereonEnemyCopied::Instance());
		}

	}

	void CamereonEnemyLostPlayer::Exit(const shared_ptr<CamereonEnemy>& Obj)
	{
		secondCount = 0;
		Obj->GetCamelAnimStateMachine()->ChangeState(CamelEnemyDefaultAnim::Instance());
	}


}