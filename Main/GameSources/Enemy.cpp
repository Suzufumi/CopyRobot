#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Enemy::Enemy(const shared_ptr<Stage>& stagePtr, const shared_ptr<StageCellMap>& cellMap, InitializeParam& initParam)
		: AliveGameObject(stagePtr), m_cellMap(cellMap), m_initParam(initParam), m_nextWayPointNum(1)
	{

	}

	Enemy::Enemy(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
		: AliveGameObject(stagePtr),m_nextWayPointNum(1), m_isAlived(true), 
		m_behaviorSecondCount(0), m_findTime(0)
	{
		// XmlからPlayerの情報を取得
		LoadXmlParam(pNode, m_initParam.m_startPos, m_initParam.m_startQuat, m_initParam.m_startScale);
		// 徘徊ポイントのXMLのデータを代入
		auto wayPointNodes = XmlDocReader::GetSelectNodes(pNode,L"WayPoint");
		// 徘徊ポイントがいくつあるのかを調べる
		long countNode = XmlDocReader::GetLength(wayPointNodes);
		for (long i = 0; i < countNode; i++)
		{
			// XMLのwayPointのデータを1つ取得
			auto node = XmlDocReader::GetItem(wayPointNodes, i);
			// wayPointのXMLの座標を取得
			auto wayPosStr = XmlDocReader::GetAttribute(node, L"Pos");
			// XMLの座標をVec3に変換する
			Vec3 wayPos = MyUtil::unityVec3StrToBCVec3(wayPosStr);
			// WayPointsのvector配列に格納
			m_wayPoints.push_back(wayPos);
		}

		// CellMapのデータを取得
		auto cellMapStr = XmlDocReader::GetAttribute(pNode, L"CellMapName");
		// Stageに登録されているCellMapの文字列を探して変数に格納
		m_cellMap = GetStage()->GetSharedGameObject<StageCellMap>(cellMapStr);
	}

	Enemy::~Enemy()
	{
	}

	void Enemy::OnCreate()
	{
		// Transformコンポーネントの取得
		auto transComp = AddComponent<Transform>();
		// Transformの設定
		transComp->SetPosition(m_initParam.m_startPos);
		transComp->SetQuaternion(m_initParam.m_startQuat);
		transComp->SetScale(m_initParam.m_startScale);

		// タグの設定
		AddTag(L"Enemy");
		AddTag(L"CopyableObj");

		// Rigidbodyコンポーネントの取得
		auto rigidComp = AddComponent<Rigidbody>();
		// 衝突判定
		auto ptrCapsule = AddComponent<CollisionCapsule>();
		// 固定衝突オブジェクトの設定(trueにすると当たった時に動かない)
		ptrCapsule->SetFixed(false);
		// 衝突したときのHitAction(None, Auto , Slide, Stop)
		ptrCapsule->SetIsHitAction(IsHitAction::Auto);

		// オブジェクトのグループを得る
		auto group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		// 自分をグループに入れる
		group->IntoGroup(GetThis<Enemy>());
		// 分離行動をする
		auto ptrSep = GetBehavior<SeparationSteering>();
		ptrSep->SetGameObjectGroup(group);

		// セルマップの取得
		auto mapPtr = m_cellMap.lock();
		// 経路探索コンポーネントの取得
		auto pathPtr = AddComponent<PathSearch>(mapPtr);

		auto ptrSeek = GetBehavior<SeekSteering>();
		// 重さの設定
		ptrSeek->SetWeight(3.0f);
		const Vec3 Rotation = Vec3(0.0f, XM_PI, 0.0f);
		const Vec3 Position = Vec3(0.0f, -0.5f, 0.0f);
		// モデルの行列設定
		auto spanMat = SetModelMatrix(Vec3(1.0f), Vec3(0.0f), Rotation, Position);

		// 描画コンポーネントの取得
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		drawComp->SetMeshToTransformMatrix(spanMat);
		// スプライトの設定
		m_sprite = GetStage()->AddGameObject<Sprite>(L"REDFLAME_TX", Vec2(1280, 800), 0.0f, Vec2(0, 0), 1, 1);
		// 最初は透明にしておく
		m_sprite.lock()->SetDiffuse(Col4(1, 1, 1, 0));
		m_sprite.lock()->SetBlendState(Sprite::BlendState::Trace);

		//エフェクトの初期化
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		wstring effectStr = dataDir + L"Effects\\Down_Copirobo.efk";
		auto ShEfkInterface = GetTypeStage<GameStage>()->GetEfkInterface();
		m_downEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, effectStr);

		// セル検索の行動の構築
		m_cellSearchBehavior.reset(new CellSearchBehavior<Enemy>(GetThis<Enemy>(), m_cellMap));
	}

	void Enemy::OnUpdate()
	{
	}

	void Enemy::OnUpdate2()
	{
		auto ptrUtil = GetBehavior<UtilBehavior>();
		// 進行方向に向かせる
		ptrUtil->RotToHead(0.1f);
	}

	void Enemy::OnDraw()
	{	
		if (GetComponent<PNTBoneModelDraw>()->GetDiffuse().w > 0)
		{
			// 親クラスのOnDrawを呼ばないと描画されない
			AliveGameObject::OnDraw();
		}
	}

	void Enemy::UpdateAnimation()
	{
	}

	void Enemy::Stop()
	{
		// 死亡フラグをfalseに設定
		m_isAlived = false;
	}

	void Enemy::OnEvent(const shared_ptr<Event>& event)
	{
		if (event->m_MsgStr == L"Respawn")
		{
			// Enemyのリスポーン
			RespawnEnemy();
		}
	}

	void Enemy::CreatePostEvent(float delayTime, const wstring keyName)
	{
		// delayTime : 何秒遅らせるか
		// keyName : イベントを呼ぶときの名前を設定
		PostEvent(delayTime, GetThis<ObjectInterface>(), GetThis<ObjectInterface>(), keyName);
	}

	void Enemy::NextWayPointNumAdd()
	{
		// m_NextWayPointNumがm_WayPointsのサイズを超えたら
		if (static_cast<unsigned>(m_nextWayPointNum) >= (m_wayPoints.size() - 1))
		{
			// 0で初期化
			m_nextWayPointNum = 0;
		}
		else
		{
			++m_nextWayPointNum;
		}

	}

	void Enemy::CheckWayPointDistance()
	{
		// 自分のPosを取得
		auto myPos = GetMyPos();
		// 目的地と自分のPosとの距離を求める
		auto distance = bsm::length(m_wayPoints[m_nextWayPointNum] - myPos);
		if (distance <= NEXT_WAYPOINT)
		{
			// 次の徘徊番号に設定
			NextWayPointNumAdd();
		}
	}

	void Enemy::RespawnEnemy()
	{
		// 変数を初期の値に戻す(必ず1つめの経路に行くように)
		m_nextWayPointNum = 1;
		//エフェクトのプレイ
		m_effectPlay = ObjectFactory::Create<EfkPlay>(m_downEffect, Vec3(m_initParam.m_startPos.x, 0.0f, m_initParam.m_startPos.z));
		auto rigidComp = GetComponent<Rigidbody>();
		// velocityをゼロにセットする
		rigidComp->SetVelocityZero();
		auto transComp = GetComponent<Transform>();
		// 最初のポジションに設定
		transComp->ResetPosition(m_initParam.m_startPos);
		// スケールを1に設定する
		transComp->SetScale(Vec3(m_initParam.m_startScale));
		// リスポーン最中は衝突されても動かないようにする
		GetComponent<Collision>()->SetFixed(true);
		// 死亡フラグをtrueにする
		SetIsAlived(true);
	}
	// 敵が重ならないようにする
	void Enemy::SeparationEnemy()
	{
		// 分離行動(敵同士が離れるように)
		auto ptrSep = GetBehavior<SeparationSteering>();
		ptrSep->Execute();
	}

	bool Enemy::CheckFindPlayer()
	{
		// 自分のPositionを取得
		auto myPos = GetMyPos();
		// プレイヤーのPositionを取得
		auto playerPos = GetPlayerPos();

		 // 自分とプレイヤーの距離を取得
		float distance = GetPlayerDistance();
		// プレイヤーが離れていたら
		if (distance > PLAYER_DISTANCE)
		{
			return false;
		}

		// プレイヤーが変身しているかどうか
		if (IsPlayerAssimililation())
		{
			return false;
		}

		// Enemyの視界を取得
		float deg = GetEnemyEyeSight(playerPos);
		// 視界が45度以上なら
		if (Rad2Deg(deg) > ENEMY_EYESIGHT)
		{
			return false;
		}

		// EnemyとPlayerの間に壁があるかどうか
		if (IsWallBetweenPlayerAndEnemy(playerPos, myPos))
		{
			return false;
		}

		return true;
	}

	float Enemy::GetPlayerDistance() const
	{
		// 自分のPositionを取得
		auto myPos = GetMyPos();
		// プレイヤーのPositionを取得
		auto playerPos = GetPlayerPos();
		// 自分とプレイヤーとの距離を返す
		return bsm::length(playerPos - myPos);
	}

	float Enemy::GetEnemyEyeSight(Vec3 playerPos) const
	{
		// 自身のPositionを取得
		auto myPos = GetMyPos();
		// 視界に入ってるか
		auto dir = playerPos - myPos;
		// 正規化
		dir.normalize();
		// 前方のベクトルを取得
		auto forward = GetComponent<Transform>()->Forward();
		// 内積を取得
		float dot = bsm::dot(dir, forward);
		auto deg = acos(dot);

		return deg;
	}

	bool Enemy::IsPlayerAssimililation()
	{
		// stageのオブジェクトを取得
		auto stageObj = GetStage()->GetGameObjectVec();
		for (auto& obj : stageObj)
		{
			// Playerのオブジェクトを取得
			auto player = dynamic_pointer_cast<Player>(obj);
			if (player)
			{
				auto rigidComp = player->GetComponent<Rigidbody>();
				// velocityの取得
				auto velocity = rigidComp->GetVelocity();
				// velocityの長さを取得
				auto dir = velocity.length();
				// Playerの現在のステートを取得
				auto stateMachineType = player->GetStateMachine()->GetCurrentState();
				// Playerの現在の状態を取得
				auto nowCopyType = player->GetPlayerCopytype();
				switch (nowCopyType)
				{
				case Player::CopyType::Normal:
					break;
				case Player::CopyType::NormalEnemy:
				case Player::CopyType::CamereonEnemy:
					// Playerが変身中または変身途中かどうか
					// 動いているか
					if (stateMachineType == PlayerCamereonTransform::Instance() ||
						stateMachineType == PlayerCamereonAssimililation::Instance() &&
						dir >= PLAYER_VELOCITY_LENGTH)
					{
						return true;
					}
					break;
				case Player::CopyType::Pillar:
				case Player::CopyType::CrystalTable:
					// Playerが変身中または変身途中かどうか
					// 止まっているか
					if (stateMachineType == PlayerCamereonTransform::Instance() ||
						stateMachineType == PlayerCamereonAssimililation::Instance() &&
						dir <= PLAYER_VELOCITY_LENGTH)
					{
						return true;
					}
					break;
				}
			}
		}
		return false;
	}		

	bool Enemy::IsWallBetweenPlayerAndEnemy(Vec3 playerPos, Vec3 enemyPos)
	{
		// wallのオブジェクトを保持する変数
		vector<shared_ptr<GameObject>> wallVec;
		// Wallのタグが付いているオブジェクトを取得
		GetStage()->GetUsedTagObjectVec(L"Wall", wallVec);
		for (auto& wallObj : wallVec)
		{
			// wallのオブジェクトを取得
			auto wall = dynamic_pointer_cast<Wall>(wallObj);
			if (wall)
			{
				auto wallCol = wall->GetComponent<CollisionObb>();
				// wallのObbを取得
				auto wallObb = wallCol->GetObb();
				// 線分を引いて壁があるかどうか調べる
				if (HitTest::SEGMENT_OBB(enemyPos, playerPos, wallObb))
				{
					return true;
				}
			}
		}

		return false;
	}

	Vec3 Enemy::GetPlayerPos() const
	{
		// stageのオブジェクトを取得
		auto stageObj = GetStage()->GetGameObjectVec();
		for (auto obj : stageObj)
		{
			// Playerのタグが付いているオブジェクトを探す
			if (obj->FindTag(L"Player"))
			{
				// PlayerのPositionを取得
				auto playerPos = obj->GetComponent<Transform>()->GetPosition();
				return playerPos;
			}
		}
		return Vec3(0.0f, 0.0f, 0.0f);
	}

	shared_ptr<ObjState<Player>> Enemy::GetPlayerCurrentStateMachine()
	{
		shared_ptr<ObjState<Player>> currentState = NULL;
		// stageのオブジェクトを取得
		auto stageObj = GetStage()->GetGameObjectVec();
		for (auto& obj : stageObj)
		{
			auto player = dynamic_pointer_cast<Player>(obj);
			if (player)
			{
				// Playerの現在のステートマシーンを取得
				currentState = player->GetStateMachine()->GetCurrentState();
			}
		}
		return currentState;
	}

	Player::CopyType Enemy::GetPlayerCopyType() const
	{
		// stageのオブジェクトを取得
		auto stageObj = GetStage()->GetGameObjectVec();
		for (auto& obj : stageObj)
		{
			auto player = dynamic_pointer_cast<Player>(obj);
			if (player)
			{
				auto playerCopyType = player->GetPlayerCopytype();
				return playerCopyType;
			}
		}

		return Player::CopyType::Normal;
	}

	shared_ptr<MeshResource> Enemy::GetMeshResourceForMinimap()
	{
		if (m_meshResForMinimap)
			return m_meshResForMinimap;

		Col4 color = Col4(0.8f, 0.1f, 0.1f, 1);

		vector<VertexPositionColor> vertices{
			{ Vec3(-0.75f,0.0f,-1.0f),color },
			{ Vec3(0.0f,0.0f,1.0f),color },
			{ Vec3(0.0f,0.0f,-0.5f),color },
			{ Vec3(0.75f,0.0f,-1.0f),color }
		};

		vector<uint16_t> indices{
			0,1,2,
			2,1,3
		};

		m_meshResForMinimap = MeshResource::CreateMeshResource(vertices, indices, true);
		return m_meshResForMinimap;
	}

}
// end basecross