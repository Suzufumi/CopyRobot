#pragma once
#include "stdafx.h"

namespace basecross
{
	class Enemy : public AliveGameObject
	{
	public:
		enum EnemyState
		{
			None,
			Normal,
			Chase,
			Lost,
			Copied
		};
		struct InitializeParam
		{
			Vec3 m_startPos;
			Quat m_startQuat;
			Vec3 m_startScale;
		};
	private:
		weak_ptr<Sprite> m_sprite;
		// エフェクト
		shared_ptr<EfkEffect> m_downEffect;
		// エフェクトの実行オブジェクト
		shared_ptr<EfkPlay> m_effectPlay;
		// 次の徘徊ポイントのとの距離
		const float NEXT_WAYPOINT = 1.0f;
		// プレイヤーとの距離
		const float PLAYER_DISTANCE = 10.0f;
		// プレイヤーのvelocitの長さ
		const float PLAYER_VELOCITY_LENGTH = 3.0f;
		// 敵の視界
		const float ENEMY_EYESIGHT = 50.0f;
		// プレイヤーを見つける時間
		const unsigned int FIND_PLAYER_TIME = 60;
		// 追いかける時間
		const unsigned int CHASE_TIME = 200;
		// 見失ってから徘徊行動に戻る時間
		const unsigned int  LOST_PLAYER_TIME = 60;
		// 敵の視界の限界
		const float EYESIGHT_LIMIT = 80.0f;
		// リスポーンしてから徘徊行動に戻る時間
		const unsigned int RETUNRN_WANDER_TIME = 720;

	protected:
		// 初期化変数
		InitializeParam m_initParam;
		// 死亡フラグ
		bool m_isAlived;
		// セルマップを保持する
		weak_ptr<StageCellMap> m_cellMap;
		// 次の徘徊ポイントの番号を保持する
		int m_nextWayPointNum;
		// 徘徊ポイント
		vector<Vec3> m_wayPoints;
		// セル検索の行動クラス
		unique_ptr<CellSearchBehavior<Enemy>> m_cellSearchBehavior;
		EnemyState m_enemyState = EnemyState::None;
		// ミニマップに描画するMeshを保持する
		shared_ptr<MeshResource> m_meshResForMinimap;
		// 行動の秒数管理
		unsigned int m_behaviorSecondCount;
		// プレイヤーを見つめている秒数をカウント
		unsigned int m_findTime;
	public:
		Enemy(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		Enemy(const shared_ptr<Stage>& stagePtr, const shared_ptr<StageCellMap>& cellMap, InitializeParam& initParam);

		virtual ~Enemy();
		// 構築
		virtual void OnCreate()override;
		// 更新
		virtual void OnUpdate()override;
		// 後更新
		virtual void OnUpdate2()override;
		// 描画
		virtual void OnDraw()override;
		// アニメーションの更新
		virtual void UpdateAnimation();

		// 動きを止める
		virtual void Stop() override;
		// PostEvnetを取得する
		virtual void OnEvent(const shared_ptr<Event>& event) override;
		// PostEventを設定する
		void CreatePostEvent(float delayTime, const wstring keyName);
		// プレイヤーを見つける
		bool CheckFindPlayer();
		// 次の徘徊ポイントの番号を追加
		void NextWayPointNumAdd();
		// 現在の徘徊ポイントとの距離
		void CheckWayPointDistance();
		// エネミーをリスポーンさせる
		void RespawnEnemy();
		// 敵同士が重ならないようにする
		void SeparationEnemy();
		// プレイヤーとの距離を取得
		float GetPlayerDistance() const;
		// エネミーの視界を取得
		float GetEnemyEyeSight(Vec3 playerPos) const;
		// プレイヤーの座標を取得
		Vec3 GetPlayerPos() const;
		// 自分の座標を取得
		inline Vec3 GetMyPos() const
		{
			auto myPos = this->GetComponent<Transform>()->GetPosition();
			return myPos;
		}
		// 徘徊ポイントの座標の取得
		inline Vec3 GetWayPoint() const { return m_wayPoints[m_nextWayPointNum]; }
		// 自身が生きているかどうかを取得
		inline bool GetIsAlived() const { return m_isAlived; }
		inline void SetIsAlived(bool isAlived) { m_isAlived = isAlived; }
		// プレイヤーが変身途中か変身中かどうか判断する
		bool IsPlayerAssimililation();
		// プレイヤーとの間に壁があるかどうか
		bool IsWallBetweenPlayerAndEnemy(Vec3 playerPos, Vec3 enemyPos);
		// ステートを取得
		EnemyState GetEnemyState() { return m_enemyState; }
		// ステートを設定
		inline void SetEnemyState(EnemyState state) { m_enemyState = state; }
		// セル検索の行動を得る
		inline unique_ptr<CellSearchBehavior<Enemy>>& GetCellSearchBehavior()
		{ return m_cellSearchBehavior; }
		// スプライトの取得
		inline weak_ptr<Sprite> GetRedLineSprite()
		{ return m_sprite; }
		// プレイヤーのステートを取得
		Player::CopyType GetPlayerCopyType() const;
		
		// プレイヤーの現在のステート取得
		shared_ptr<ObjState<Player>> GetPlayerCurrentStateMachine();

		virtual shared_ptr<MeshResource> GetMeshResourceForMinimap();

		inline Vec3 GetStartPosition() { return m_initParam.m_startPos; }
		// プレイヤーを発見する時間を取得
		inline unsigned int GetFindPlayerTime() const
		{ return FIND_PLAYER_TIME;}
		// 追いかける時間を取得
		inline unsigned int GetChaseTime() const
		{ return CHASE_TIME;}
		// プレイヤーを見失ってから徘徊に戻る時間を取得
		inline unsigned int GetLostPlayerTime() const
		{ return LOST_PLAYER_TIME;}
		// 敵の視界限界を取得
		inline float GetEyesightLimit() const
		{ return EYESIGHT_LIMIT;}
		// リスポーン後に徘徊に戻る時間を取得
		inline unsigned int GetReturnWanderTime() const
		{ return RETUNRN_WANDER_TIME;}
		// 現在の秒数を取得
		inline unsigned int GetBehaviorConut() const
		{ return m_behaviorSecondCount;}
		// 秒数の設定
		inline void SetBehaviorCount(unsigned int countTime)
		{ m_behaviorSecondCount = countTime;}
		// 現在の見つめた秒数の取得
		inline unsigned int GetFindTime() const
		{ return m_findTime;}
		// 見つめた秒数の設定
		inline void SetFindTime(unsigned int findTime)
		{ m_findTime = findTime;}
	};

}
// end basecross