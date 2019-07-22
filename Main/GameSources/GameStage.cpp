/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------



	//ビューとライトの作成
	void GameStage::CreateViewLight()
	{
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrTpsCamera = ObjectFactory::Create<TpsCamera>();
		PtrView->SetCamera(PtrTpsCamera);
		//シングルライトの作成
		auto PtrSingleLight = CreateLight<SingleLight>();
		//ライトの設定
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	void GameStage::CreatePlayer()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Player>(L"Player");
		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/Player");
	}

	void GameStage::CreateEnemy()
	{
		auto group = GetSharedObjectGroup(L"EnemyGroup", false);
		if (group)
		{
			group->AllClear();
		}
		else
		{
			CreateSharedObjectGroup(L"EnemyGroup");
		}


		GameObjecttXMLBuilder builder;
		builder.Register<NormalEnemy>(L"NormalEnemy");
		builder.Register<CamereonEnemy>(L"CamereonEnemy");
		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/NormalEnemy");
	}

	void GameStage::CreatePlane()
	{
		Quat q;
		q.rotationX(XM_PIDIV2);
		// Transformの設定
		StaticObject::InitializeParam status = {
			{ 0.0f, 0.0f, 0.0f },		// Position
					q           ,       // Quaternion
			{ 100.0f, 100.0f, 1.0f }		// Scale
		};
		Plane::InitializeParam planeParam = {
			L"STAGE_TX"
		};

		AddGameObject<Plane>(status,planeParam);
	}

	void GameStage::CreateWall()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Wall>(L"Wall");

		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/Wall");

		SetCellMapCost<Wall>(L"Wall");
	}
	///------------------------------------------------
	// セルマップの作成
	///------------------------------------------------
	void GameStage::CreateStageCellMap()
	{
		// CellMapのグループを作成
		auto group = CreateSharedObjectGroup(L"CellMap");
		// 現在のステージのXmlファイル番号を取得
		wstring stageXmlPath = m_StageXmlPath;
		// Xmlファイルの読み込み
		XmlDocReader xmlReader(stageXmlPath);
		// XmlファイルからGameStage/CellMapを探す
		auto cellMapData = xmlReader.GetSelectNodes(L"GameStage/CellMap");
		// CellMapの数をカウント
		long cellMapCount = XmlDocReader::GetLength(cellMapData);
		// CellMapの数だけループ
		for (long i = 0; i < cellMapCount; i++)
		{
			auto node = XmlDocReader::GetItem(cellMapData, i);
			// XMLからPieceSizeを取得(wstring)
			auto pieceSizeStr = XmlDocReader::GetAttribute(node, L"PieceSize");
			// wstringをfloatの値に変換
			float pieceSize = MyUtil::wstrToFloat(pieceSizeStr);
			// XMLからPositionを取得(wstring)
			auto positionStr = XmlDocReader::GetAttribute(node, L"Pos");
			// wstringをVec3に変換
			Vec3 pos = MyUtil::unityVec3StrToBCVec3(positionStr);
			// XMLからRowを取得
			auto rowStr = XmlDocReader::GetAttribute(node, L"Row");
			// wstringをintに変換
			int row = MyUtil::wstrToInt(rowStr);
			// XMLからColを取得
			auto colStr = XmlDocReader::GetAttribute(node, L"Col");
			// wstringをintに変換
			int col = MyUtil::wstrToInt(colStr);
			// StageCellMapをステージに配置する
			auto stageCellMap = AddGameObject<StageCellMap>(pos, pieceSize, col, row);
			// ノードの名前を取得
			auto nameStr = XmlDocReader::GetAttribute(node, L"Name");
			// タグ追加
			stageCellMap->AddTag(nameStr);
			stageCellMap->AddTag(L"CellMap");
			SetSharedGameObject(nameStr, stageCellMap);
			// グループに追加
			group->IntoGroup(stageCellMap);
		}
	}

	void GameStage::CreateRock()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Pillar>(L"Rock");
		builder.Register<CrystalTable>(L"CrystalTable");

		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/Rock");
		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/CrystalTable");

		SetCellMapCost<Pillar>(L"Pillar");
		SetCellMapCost<CrystalTable>(L"CrystalTable");
	}

	void GameStage::CreateTutorialBox()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<TutorialBox>(L"TutorialBox");

		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/TutorialBox");
	}
	
	template<typename T>
	void GameStage::SetCellMapCost(wstring TagName)
	{
		// CellMapという名前のグループを取得する
		auto group = GetSharedObjectGroup(L"CellMap");
		// グループの中のセルマップの数だけループする
		for (auto& cellMap : group->GetGroupVector())
		{
			// CellMapのポインタ取得
			auto mapPtr = dynamic_pointer_cast<StageCellMap>(cellMap.lock());
			if (mapPtr)
			{
				// セルマップからセルの配列を取得
				auto& cellVec = mapPtr->GetCellVec();

				vector<shared_ptr<GameObject>> ObjectVec;
				// タグが付いているオブジェクトを取得してvectorに追加
				GetUsedTagObjectVec(TagName, ObjectVec);
				vector<AABB> objectsAABBVec;
				for (auto& Obj : ObjectVec)
				{
					// 引数で指定されたオブジェクトを取得
					auto ObjPtr = dynamic_pointer_cast<T>(Obj);
					if (ObjPtr)
					{
						auto colPtr = ObjPtr->GetComponent<CollisionObb>();
						// ボックスの衝突判定かラッピングするAABBを取得して保存
						AABB pushAABB = colPtr->GetWrappingAABB();
						pushAABB.m_Min.x += 0.1f;
						pushAABB.m_Min.z += 0.1f;
						pushAABB.m_Max.x -= 0.1f;
						pushAABB.m_Max.z -= 0.1f;
						objectsAABBVec.push_back(pushAABB);
					}
				}
				//セル配列からセルをスキャン
				for (auto& row : cellVec) 
				{
					for (auto& col : row) 
					{
						for (auto& vObj : objectsAABBVec) 
						{
							//セルに障害物が配置されているか
							if (HitTest::AABB_AABB_NOT_EQUAL(col.m_PieceRange, vObj)) 
							{
								// 障害物を-1で設定する
								col.m_Cost = -1;
								break;
							}
						}
					}
				}
			}
		}
	}

	void GameStage::CreateGoal()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Goal>(L"Goal");

		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/Goal");
		SetCellMapCost<Goal>(L"Goal");
	}

	void GameStage::OnCreate()
	{
		try 
		{
			//シーンの取得
			auto PtrScene = App::GetApp()->GetScene<Scene>();
			wstring dataDir;
			App::GetApp()->GetDataDirectory(dataDir);
			switch (PtrScene->GetStageNum())
			{
			case 0:
				m_StageXmlPath = dataDir + L"Stage\\1.xml";
				break;
			case 1:
				m_StageXmlPath = dataDir + L"Stage\\2.xml";
				break;
			case 2:
				m_StageXmlPath = dataDir + L"Stage\\3.xml";
				break;
			case 3:
				m_StageXmlPath = dataDir + L"Stage\\4.xml";
				break;
			case 4:
				m_StageXmlPath = dataDir + L"Stage\\5.xml";
				break;
			case 5:
				m_StageXmlPath = dataDir + L"Stage\\6.xml";
				break;
			case 6:
				m_StageXmlPath = dataDir + L"Stage\\7.xml";
				break;
			case 7:
				m_StageXmlPath = dataDir + L"Stage\\8.xml";
				break;
			default:
				m_StageXmlPath = dataDir + L"Stage\\2.xml";
				break;
			}
			//m_StageXmlPath = dataDir + L"Stage\\test.xml";

			//エフェクトの追加
			m_EfkInterface = ObjectFactory::Create<EfkInterface>();

			auto miniMap = AddGameObject<MiniMap>();
			miniMap->SetDrawActive(false);

			//ビューとライトの作成
			CreateViewLight();
			// セルマップの作成
			CreateStageCellMap();
			// プレイヤーの作成
			CreatePlayer();
			// Enemyの作成
			CreateEnemy();
			// 床の作成
			CreatePlane();
			//壁の作成
			CreateWall();
			//岩の作成
			CreateRock();
			//ゴールの作成
			CreateGoal();

			CreateTutorialBox();

			auto skySphere = AddGameObject<SkySphere>();
		}
		catch (...) 
		{
			throw;
		}
	}

	void GameStage::OnUpdate()
	{
		Stage::OnUpdate();

		auto key = App::GetApp()->GetInputDevice().GetKeyState();

		if (key.m_bPressedKeyTbl['T'])
		{
			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn,Col4(0,0,0,1));
			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut, Col4(0, 0, 0, 1));
			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransition(transIn, transOut);
		}
		else if (key.m_bPressedKeyTbl['Y'])
		{
			auto transIn = ObjectFactory::Create<FadeMaskTransition>(3.0f,L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
			auto transOut = ObjectFactory::Create<FadeMaskTransition>(3.0f,L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransition(transIn, transOut);
		}
		
		m_EfkInterface->OnUpdate();
	}
	
	void GameStage::OnDraw()
	{

	}

	void GameStage::DrawStage()
	{
		//レイヤーの取得と設定
		set<int> DrawLayers;
		//Spriteかそうでないかを分離
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsDrawActive()) {
				//描画レイヤーに登録
				DrawLayers.insert(ptr->GetDrawLayer());
				//Spriteかその派生クラスなら分離

				auto sprite = dynamic_pointer_cast<Sprite>(ptr);

				if (ptr->GetDynamicComponent<SpriteBaseDraw>(false) || ptr->IsSpriteDraw() || sprite) {
					pImpl->m_SpriteVec.push_back(ptr);
				}
				else {
					pImpl->m_Object3DVec.push_back(ptr);
				}
			}
		}
		//3Dの透明と非透明を分離
		for (auto ptr : pImpl->m_Object3DVec) {
			if (ptr->IsDrawActive()) {
				if (ptr->IsAlphaActive()) {
					pImpl->m_Object3DAlphaVec.push_back(ptr);
				}
				else {
					pImpl->m_Object3DNormalVec.push_back(ptr);
				}
			}
		}

		auto PtrCamera = pImpl->m_ViewBase->GetTargetCamera();
		//カメラの位置
		bsm::Vec3 CameraEye = PtrCamera->GetEye();
		//透明の3Dオブジェクトをカメラからの距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto func = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool {
			auto PtrLeftTrans = Left->GetComponent<Transform>();
			auto PtrRightTrans = Right->GetComponent<Transform>();

			auto LeftPos = PtrLeftTrans->GetWorldMatrix().transInMatrix();
			auto RightPos = PtrRightTrans->GetWorldMatrix().transInMatrix();

			auto LeftLen = bsm::length(LeftPos - CameraEye);
			auto RightLen = bsm::length(RightPos - CameraEye);

			return (LeftLen > RightLen);
		};

		std::sort(pImpl->m_Object3DAlphaVec.begin(), pImpl->m_Object3DAlphaVec.end(), func);

		//3Dノーマルオブジェクトの描画準備
		for (auto ptr : pImpl->m_Object3DNormalVec) {
			ptr->OnPreDraw();
		}
		//3D透明オブジェクトの描画準備
		for (auto ptr : pImpl->m_Object3DAlphaVec) {
			ptr->OnPreDraw();
		}
		//パーティクルの描画準備（透明）
		GetParticleManager(false)->OnPreDraw();
		//パーティクルの描画準備（加算）
		GetParticleManager(true)->OnPreDraw();
		//スプライトオブジェクトの描画準備
		for (auto ptr : pImpl->m_SpriteVec) {
			ptr->OnPreDraw();
		}


		//--------------------------------------------------------
		//スプライトをZ座標距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto funcSprite = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool {
			auto PtrLeftTrans = Left->GetComponent<Transform>();
			auto PtrRightTrans = Right->GetComponent<Transform>();

			auto LeftPos = PtrLeftTrans->GetWorldMatrix().transInMatrix();
			auto RightPos = PtrRightTrans->GetWorldMatrix().transInMatrix();

			float LeftZ = LeftPos.z;
			float RightZ = RightPos.z;
			return (LeftZ > RightZ);
		};
		std::sort(pImpl->m_SpriteVec.begin(), pImpl->m_SpriteVec.end(), funcSprite);

		bool isOnece = false;
		for (auto it = DrawLayers.begin(); it != DrawLayers.end(); it++) {
			int Tgt = *it;
			//3Dノーマルオブジェクトの描画
			for (auto ptr : pImpl->m_Object3DNormalVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
			//3D透明オブジェクトの描画
			for (auto ptr : pImpl->m_Object3DAlphaVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}

			if (!isOnece)
			{
				auto& camera = GetView()->GetTargetCamera();
				m_EfkInterface->SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());
				m_EfkInterface->OnDraw();

				isOnece = true;
			}

			//パーティクルの描画
			//パーティクルマネージャは描画レイヤーごとに初期化されるので
			//毎レイヤー描画する
			//透明処理
			GetParticleManager(false)->OnDraw();
			//加算処理
			GetParticleManager(true)->OnDraw();
			//スプライトオブジェクトの描画
			for (auto ptr : pImpl->m_SpriteVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
		}
		//ステージのDraw();
		OnDraw();
		//ワーク用配列のクリア
		//ワーク配列は毎ターンごとに初期化されるが、
		//最大値は減らないので2回目のターン以降は高速に動作する
		pImpl->m_Object3DVec.clear();
		pImpl->m_SpriteVec.clear();
		pImpl->m_Object3DNormalVec.clear();
		pImpl->m_Object3DAlphaVec.clear();
	}

	void GameStage::RenderStage()
	{
		vector<shared_ptr<GameObject>> objectVec;
		GetUsedTagObjectVec(L"MiniMap", objectVec);
		for (auto miniMap : objectVec)
		{
			auto lock = dynamic_pointer_cast<MiniMap>(miniMap);
			if (lock)
				lock->MiniMapDraw();
		}

		Stage::RenderStage();
	}

	void GameStage::OnEvent(const shared_ptr<Event>& event)
	{
		Stage::OnEvent(event);

		if (event->m_MsgStr == L"CreatePillar")
		{
			auto pillarParam = static_pointer_cast<pair<StaticObject::InitializeParam, Pillar::InitializeParam>>(event->m_Info);
			AddGameObject<Pillar>(pillarParam->first, pillarParam->second);
		}
		else if (event->m_MsgStr == L"CreateCrystalTable")
		{
			auto tableParam = static_pointer_cast<StaticObject::InitializeParam>(event->m_Info);
			AddGameObject<CrystalTable>(*tableParam.get());
		}
	}

	void GameStage::StopAllEnemies()
	{
		vector<shared_ptr<GameObject>> enemies;
		GetUsedTagObjectVec(L"Enemy", enemies);

		for (auto enemy : enemies)
		{
			enemy->SetUpdateActive(false);
		}
	}

	void GameStage::ActiveAllEnemies()
	{
		vector<shared_ptr<GameObject>> enemies;
		GetUsedTagObjectVec(L"Enemy", enemies);

		for (auto enemy : enemies)
		{
			enemy->SetUpdateActive(true);
		}
	}

	bool GameStage::IsEnemyFindPlayer()
	{
		auto enemies = GetSharedObjectGroup(L"EnemyGroup", false);
		bool isFind = false;;
		for (auto enemy : enemies->GetGroupVector())
		{
			auto normal = dynamic_pointer_cast<NormalEnemy>(enemy.lock());
			auto camereon = dynamic_pointer_cast<CamereonEnemy>(enemy.lock());
			if (normal)
			{
				if (normal->GetStateMachine()->GetCurrentState() == NormalEnemyChasePlayer::Instance())
				{
					isFind = true;
					break;
				}
			}
			else if (camereon)
			{
				if (camereon->GetStateMachine()->GetCurrentState() == CamereonEnemyChasePlayer::Instance())
				{
					isFind = true;
					break;
				}
			}
		}

		return isFind;
	}
}
//end basecross
