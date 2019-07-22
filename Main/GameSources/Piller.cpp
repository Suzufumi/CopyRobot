#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Pillar::Pillar(const shared_ptr<Stage>& stagePtr, StaticObject::InitializeParam& initParam, InitializeParam& InitializeParam)
		: StaticObject(stagePtr, initParam), m_InitializeParam(InitializeParam),
		m_ShotIntervalCount(0.0f)
	{}

	Pillar::Pillar(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode):
		StaticObject(stagePtr, StaticObject::InitializeParam())
	{
		// XmlからPillerの情報取得
		StaticObject::m_initParam;
		LoadXmlParam(pNode, m_initParam.startPos, m_initParam.startQuat, m_initParam.startScale);


		Pillar::InitializeParam rockInitParam;
		rockInitParam.texKey = L"PILLAR_BONE_MESH";
		rockInitParam.m_ShotInterval = 2.0f;
		rockInitParam.m_ShotBulletSpeed = 5.0f;

		Pillar::m_InitializeParam = rockInitParam;
	}

	Pillar::~Pillar()
	{

	}

	void Pillar::OnCreate()
	{

		// Transformコンポーネントの取得
		auto transComp = AddComponent<Transform>();

		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		Coll->SetMakedSize(1.0f);
		Coll->SetMakedScale(Vec3(1, 2.5f, 1));
		Coll->SetPositionDiff(Vec3(0, 1.25f, 0));

		// Position,Quaternion,Scaleの設定
		transComp->SetPosition(StaticObject::m_initParam.startPos);
		transComp->SetQuaternion(StaticObject::m_initParam.startQuat);
		transComp->SetScale(StaticObject::m_initParam.startScale);

		// モデルの行列設定
		auto spanMat = SetModelMatrix(Vec3(1.0f), Vec3(0.0f), Vec3(0.0f), Vec3(0.0f));

		// 描画コンポーネントの取得
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		// メッシュの形状の設定
		drawComp->SetMeshResource(m_InitializeParam.texKey);
		drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetLineTextureResource(L"PILLAR_LINE_TX");
		drawComp->SetBlendState(BlendState::AlphaBlend);
		drawComp->SetRasterizerState(RasterizerState::DoubleDraw);
		drawComp->SetOwnShadowActive(false);

		Vec3 firstScale = StaticObject::m_initParam.startScale;
		firstScale.x = 0.0f;
		firstScale.y *= 2.0f;
		firstScale.z = 0.0f;
		transComp->SetScale(firstScale);
		auto action = AddComponent<Action>();
		action->AddScaleTo(0.25f, StaticObject::m_initParam.startScale, Lerp::Easein);
		action->Run();

		//タグ付け
		AddTag(L"CopyObj");
		AddTag(L"Pillar");
		AddTag(L"CopyableObj");

		SetAlphaActive(true);
	}

	void Pillar::OnUpdate()
	{
	}

	bool Pillar::CheckIsWarning()
	{
		vector<shared_ptr<GameObject>> enemies;
		GetStage()->GetUsedTagObjectVec(L"Enemy", enemies);
		for (auto enemy : enemies)
		{
			auto dynamicEnemy = dynamic_pointer_cast<Enemy>(enemy);
			if (dynamicEnemy->GetEnemyState() == Enemy::EnemyState::Chase)
				return true;
		}

		return false;
	}

	void Pillar::ShotBulletToPlayer()
	{
		if (!IsScenePlayer())
			return;

		vector<shared_ptr<GameObject>> players;
		GetStage()->GetUsedTagObjectVec(L"Player", players);

		if (players.size() <= 0)
			return;

		Vec3 playerPos = players[0]->GetComponent<Transform>()->GetPosition();
		Vec3 dir = playerPos - GetComponent<Transform>()->GetPosition();
		dir.normalize();

		Bullet::InitializeParam blInit;
		blInit.m_Position = GetComponent<Transform>()->GetPosition();
		blInit.m_Velocity = dir * m_InitializeParam.m_ShotBulletSpeed;

		GetStage()->AddGameObject<Bullet>(blInit);
	}

	void Pillar::SHotBulletToPillar()
	{
		const Vec3 myPos = GetComponent<Transform>()->GetPosition();

		vector<shared_ptr<GameObject>> pillars;
		GetStage()->GetUsedTagObjectVec(L"Pillar", pillars);
		std::sort(pillars.begin(), pillars.end(), [myPos](shared_ptr<GameObject> a, shared_ptr<GameObject> b) {
			float aDis = (myPos - a->GetComponent<Transform>()->GetPosition()).length();
			float bDis = (myPos - b->GetComponent<Transform>()->GetPosition()).length();
			return aDis < bDis;
		});

		if (pillars.size() < 1)
			return;

		for (size_t i = 1; i < 5; i++)
		{
			if (i >= pillars.size())
				break;


			Vec3 dir = pillars[i]->GetComponent<Transform>()->GetPosition() - myPos;
			dir.normalize();

			Bullet::InitializeParam blInit;
			blInit.m_Position = GetComponent<Transform>()->GetPosition() + Vec3(0,0.5f,0);
			blInit.m_Velocity = dir * m_InitializeParam.m_ShotBulletSpeed;

			GetStage()->AddGameObject<Bullet>(blInit);
		}
	}

	bool Pillar::IsScenePlayer()
	{
		vector<shared_ptr<GameObject>> players;
		GetStage()->GetUsedTagObjectVec(L"Player", players);

		if (players.size() <= 0)
			return false;

		auto trans = players[0]->GetComponent<Transform>();

		vector<shared_ptr<GameObject>> walls;
		GetStage()->GetUsedTagObjectVec(L"Wall", walls);

		for (auto wall : walls)
		{
			if (HitTest::SEGMENT_OBB(GetComponent<Transform>()->GetPosition(), trans->GetPosition(),wall->GetComponent<CollisionObb>()->GetObb()))
				return false;
		}

		return true;
	}

	shared_ptr<MeshResource> Pillar::GetMeshResourceForMinimap()
	{
		if (m_MeshResForMinimap)
			return m_MeshResForMinimap;

		Col4 color = Col4(0.8f, 0.8f, 0.1f, 1);
		float size = 0.5f;
		vector<VertexPositionColor> vertices{
			{ Vec3(-size,0.0f,-size),color },
			{ Vec3(-size,0.0f,size),color },
			{ Vec3(size,0.0f,size),color },
			{ Vec3(size,0.0f,-size),color }
		};

		vector<uint16_t> indices{
			0,1,2,
			2,3,0
		};

		m_MeshResForMinimap = MeshResource::CreateMeshResource(vertices, indices, true);
		return m_MeshResForMinimap;
	}

	void Pillar::EndTakeIn()
	{
		GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
		shared_ptr<pair<StaticObject::InitializeParam, Pillar::InitializeParam>> pillarInfo = make_shared<pair<StaticObject::InitializeParam, Pillar::InitializeParam>>();
		pillarInfo->first = StaticObject::m_initParam;
		pillarInfo->second = Pillar::m_InitializeParam;
		PostEvent(3.0f, GetThis<ObjectInterface>(), GetStage()->GetThis<ObjectInterface>(), L"CreatePillar",static_cast<shared_ptr<void>>(pillarInfo));
	}

	CrystalTable::CrystalTable(const shared_ptr<Stage>& stagePtr,StaticObject::InitializeParam initParam):
		StaticObject(stagePtr,initParam)
	{

	}

	CrystalTable::CrystalTable(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode) :
		StaticObject(stagePtr, StaticObject::InitializeParam())
	{
		// XmlからCrystalTableを取得
		LoadXmlParam(pNode, m_initParam.startPos, m_initParam.startQuat, m_initParam.startScale);

	}

	CrystalTable::~CrystalTable()
	{}

	void CrystalTable::OnCreate()
	{

		// Transformコンポーネントの取得
		auto transComp = AddComponent<Transform>();

		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		Coll->SetMakedSize(1.0f);
		Coll->SetMakedScale(Vec3(1, 2.5f, 1));
		Coll->SetPositionDiff(Vec3(0, 1.25f, 0));

		// Position,Quaternion,Scaleの設定
		transComp->SetPosition(m_initParam.startPos);
		transComp->SetQuaternion(m_initParam.startQuat);
		transComp->SetScale(m_initParam.startScale);

		// モデルとトランスフォームの間の差分行列
		auto spanMat = SetModelMatrix(Vec3(1.0f), Vec3(0.0f), Vec3(0.0f), Vec3(0.0f));

		// 描画コンポーネントの取得
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		// メッシュの形状の設定
		drawComp->SetMeshResource(L"CRYSTALTABLE_MESH");
		drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetLineTextureResource(L"CRYSTALTABLE_LINE_TEX");
		drawComp->SetBlendState(BlendState::AlphaBlend);
		drawComp->SetRasterizerState(RasterizerState::DoubleDraw);
		drawComp->SetOwnShadowActive(false);
		drawComp->BoneInit();
		drawComp->AddAnimation(L"DEFAULT", 0, 119, true);
		drawComp->ChangeCurrentAnimation(L"DEFAULT");

		Vec3 firstScale = StaticObject::m_initParam.startScale;
		firstScale.x = 0.0f;
		firstScale.y *= 2.0f;
		firstScale.z = 0.0f;
		transComp->SetScale(firstScale);
		auto action = AddComponent<Action>();
		action->AddScaleTo(0.25f, StaticObject::m_initParam.startScale, Lerp::Easein);
		action->Run();

		//タグ付け
		AddTag(L"CopyObj");
		AddTag(L"CrystalTable");
		AddTag(L"CopyableObj");

		SetAlphaActive(true);
	}

	void CrystalTable::OnUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(delta);
	}

	shared_ptr<MeshResource> CrystalTable::GetMeshResourceForMinimap()
	{
		if (m_MeshResForMinimap)
			return m_MeshResForMinimap;

		Col4 color = Col4(0.8f, 0.8f, 0.1f, 1);
		float size = 0.5f;
		vector<VertexPositionColor> vertices{
			{ Vec3(-size,0.0f,-size),color },
			{ Vec3(-size,0.0f,size),color },
			{ Vec3(size,0.0f,size),color },
			{ Vec3(size,0.0f,-size),color }
		};

		vector<uint16_t> indices{
			0,1,2,
			2,3,0
		};

		m_MeshResForMinimap = MeshResource::CreateMeshResource(vertices, indices, true);
		return m_MeshResForMinimap;
	}

	void CrystalTable::EndTakeIn()
	{
		GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
		shared_ptr<StaticObject::InitializeParam> pillarInfo = make_shared<StaticObject::InitializeParam>(CrystalTable::m_initParam);
		PostEvent(3.0f, GetThis<ObjectInterface>(), GetStage()->GetThis<ObjectInterface>(), L"CreateCrystalTable", static_cast<shared_ptr<void>>(pillarInfo));

	}
}
// end basecross