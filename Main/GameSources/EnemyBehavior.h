#pragma once
#include "stdafx.h"

namespace basecross
{
	//-----------------------------------------------------------------------------
	/// セル検索を使った行動クラス
	//-----------------------------------------------------------------------------
	template<typename T>
	class CellSearchBehavior : public ObjBehavior<T>
	{
	private:
		// 徘徊ポイントの距離
		const float WANDER_DISTANCE = 50.0f;
		// 徘徊スピード(減速)
		const float SLOWING_SPEED = 0.83f;
		// プレイヤーを追いかけるスピード(減速)
		const float CHASE_SPEED = 0.93f;
		// セルマップのポインタ
		weak_ptr<StageCellMap> m_cellMap;
		// 経路の情報を格納
		vector<CellIndex> m_cellPath;
		//現在の自分のセルインデックス
		int m_cellIndex = -1;
		//めざす（次の）のセルインデックス
		int m_nextCellIndex = -1;
		//ターゲットのセルインデックス
		int m_targetCellIndex = -1;
	public:
		CellSearchBehavior(const shared_ptr<T>& GameObjectPtr, const weak_ptr<StageCellMap>& CellMap)
			:ObjBehavior(GameObjectPtr), m_cellMap(CellMap)
		{}
		virtual ~CellSearchBehavior()
		{}

		// 目的の場所をサーチする
		bool SearchNextPoint(const Vec3& TargetPos, shared_ptr<StageCellMap>& mapPtr)
		{
			// セルマップのポインタを取得できていたら
			if (mapPtr)
			{
				// 経路を見つける
				SearchCellPath(TargetPos);
				return true;
			}
			else
			{
				// 失敗した
				m_cellIndex = -1;
				m_nextCellIndex = -1;
				m_targetCellIndex = -1;
			}
			return false;
		}
		// 経路を見つける
		void SearchCellPath(const Vec3& TargetPos)
		{
			auto pathPtr = GetGameObject()->GetComponent<PathSearch>();
			// 目的ポイントの取得
			Vec3 targetPos = TargetPos;
			// 経路の情報をクリア
			m_cellPath.clear();
			// パス捜索をかける
			if (pathPtr->SearchCell(targetPos, m_cellPath))
			{
				// 検索が成功した
				m_cellIndex = 0;
				m_targetCellIndex = m_cellPath.size() - 1;
				if (m_cellIndex == m_targetCellIndex)
				{
					// すでに同じセルにいる
					m_nextCellIndex = m_cellIndex;
				}
				else
				{
					// 離れている
					m_nextCellIndex = m_cellIndex + 1;
				}
			}
		}
		// 次の目的地へ移動する
		void GoToNextWayPoint(const Vec3& TargetPos, shared_ptr<StageCellMap>& mapPtr)
		{
			// 自分のポジションを取得
			auto myPos = GetGameObject()->GetComponent<Transform>()->GetPosition();
			if (mapPtr)
			{
				// 大きめに設定しておいて急にスピードが速くならないように設定
				if (length(myPos - TargetPos) <= WANDER_DISTANCE)
				{
					auto rigidComp = GetGameObject()->GetComponent<Rigidbody>();
					auto velocity = rigidComp->GetVelocity();
					// 減速処理
					velocity *= SLOWING_SPEED;
					rigidComp->SetVelocity(velocity);
				}
				auto ptrSeek = GetGameObject()->GetBehavior<SeekSteering>();
				AABB ret;
				mapPtr->FindAABB(m_cellPath[m_nextCellIndex], ret);
				auto pos = ret.GetCenter();
				pos.y = TargetPos.y;
				ptrSeek->Execute(pos);
			}

		}
		// 徘徊行動をする
		void WanderingBehavior(const Vec3& TargetPos)
		{
			auto myPos = GetGameObject()->GetComponent<Transform>()->GetPosition();
			// セルマップデータの読み込み
			auto map = m_cellMap.lock();
			if (map)
			{
				// 次の徘徊ポイントを探す
				if (SearchNextPoint(TargetPos, map))
				{
					GoToNextWayPoint(TargetPos, map);
				}
				else
				{
					auto ptrSeek = GetGameObject()->GetBehavior<SeekSteering>();
					CellIndex targetCell;
					if (map->FindCell(TargetPos, targetCell))
					{
						AABB ret;
						map->FindAABB(targetCell, ret);
						auto pos = ret.GetCenter();
						pos.y = TargetPos.y;
						ptrSeek->Execute(pos);
					}
				}
			}
		}
		// プレイヤーを追跡する
		void ChasePlayer(const Vec3& PlayerPos)
		{
			auto myPos = GetGameObject()->GetComponent<Transform>()->GetPosition();
			auto map = m_cellMap.lock();
			if (map)
			{
				if (SearchNextPoint(PlayerPos, map))
				{
					HeadToPlayer(PlayerPos, myPos, map);
				}
			}
		}

		// プレイヤーに向かう
		void HeadToPlayer(const Vec3& PlayerPos, const Vec3& EnemyPos, shared_ptr<StageCellMap>& mapPtr)
		{
			// 追いかける距離
			const float CHASEDISTANCE = 15.0f;
			auto ptrSeek = GetGameObject()->GetBehavior<SeekSteering>();
			// 目的地に着いたら
			if (m_nextCellIndex == 0) {
				// 減速
				auto ptrRigid = GetGameObject()->GetComponent<Rigidbody>();
				auto velocity = ptrRigid->GetVelocity();
				velocity *= CHASE_SPEED;
				ptrRigid->SetVelocity(velocity);
				ptrSeek->Execute(PlayerPos);
			}
			else {
				// EnemyからPlayerの距離を取得する
				auto distance = length(EnemyPos - PlayerPos);
				// 距離が15.0f以下だったら
				if (distance <= CHASEDISTANCE) {
					// 減速
					auto ptrRigid = GetGameObject()->GetComponent<Rigidbody>();
					auto velocity = ptrRigid->GetVelocity();
					velocity *= CHASE_SPEED;
					ptrRigid->SetVelocity(velocity);
				}
				AABB ret;
				mapPtr->FindAABB(m_cellPath[m_nextCellIndex], ret);
				auto pos = ret.GetCenter();
				pos.y = PlayerPos.y;
				ptrSeek->Execute(Vec3(pos.x, 0.5f, pos.z));
			}
		}
	};
}