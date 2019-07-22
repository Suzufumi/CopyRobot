#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Goal::Goal(const shared_ptr<Stage>& stagePtr, StaticObject::InitializeParam& initParam, InitializeParam& InitializeParam/*,bool TangentUse*/)
		: StaticObject(stagePtr, initParam), m_InitializeParam(InitializeParam),
		  m_IsWarning(false),
		m_IsSendEndWarning(false)
	{}

	Goal::Goal(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
		: StaticObject(stagePtr,StaticObject::InitializeParam())
	{
		auto scaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");
		auto rotStr = XmlDocReader::GetAttribute(pNode, L"Quat");
		auto posStr = XmlDocReader::GetAttribute(pNode, L"Pos");

		Vec3 scale = MyUtil::unityVec3StrToBCVec3(scaleStr);
		Quat quat = MyUtil::unityQuatStrToBCQuat(rotStr);
		Vec3 pos = MyUtil::unityVec3StrToBCVec3(posStr);

		StaticObject::InitializeParam initParam;
		initParam.startScale = scale;
		initParam.startQuat = quat;
		initParam.startPos = pos;

		StaticObject::m_initParam = initParam;
	}

	Goal::~Goal()
	{}

	void Goal::OnCreate()
	{

		// Transformコンポーネントの取得
		auto transComp = AddComponent<Transform>();

		auto Coll = AddComponent<CollisionObb>();
		Coll->SetIsHitAction(IsHitAction::None);

		// Position,Quaternion,Scaleの設定
		transComp->SetPosition(StaticObject::m_initParam.startPos);
		transComp->SetQuaternion(StaticObject::m_initParam.startQuat);
		transComp->SetScale(StaticObject::m_initParam.startScale);

		Mat4x4 transMat;
		transMat.affineTransformation(
			Vec3(1.1f, 1.1f, 1.1f),
			Vec3(0, 0, 0),
			Vec3(0, 0, 0),
			Vec3(0, 2.5, 0)
		);

		auto drawComp = AddComponent<PNTBoneModelDraw>();
		drawComp->SetMeshResource(L"GOAL_MESH");
		drawComp->SetMeshToTransformMatrix(transMat);
		drawComp->SetLineTextureResource(L"GOAL_TX");
		drawComp->AddAnimation(L"DEFAULT", 0, 360, true, 60.0f);
		drawComp->ChangeCurrentAnimation(L"DEFAULT");

		//エフェクトの初期化
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring GoalEffectStr = DataDir + L"Effects\\Goal_Effect_v1.2.efk";
		auto ShEfkInterface = GetTypeStage<GameStage>()->GetEfkInterface();

		m_DownEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, GoalEffectStr);

		wstring gardEffect = DataDir + L"Effects\\Goal_Gardner.efk";
		m_GardEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, gardEffect);

		m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_DownEffect, GetComponent<Transform>()->GetPosition()+Vec3(0,-2,0));
		//m_EfkPlay2 = ObjectFactory::Create<EfkPlay>(m_GardEffect, GetComponent<Transform>()->GetPosition() + Vec3(0, -2, 0));
		//タグ付け
		AddTag(L"Goal");
	}

	void Goal::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		if (m_IsWarning)
			return;

		for (auto& obj : OtherVec)
		{
			if (obj->FindTag(L"Player"))
			{
				auto player = dynamic_pointer_cast<Player>(obj);
				player->Goal(GetComponent<Transform>()->GetPosition());
				PostEvent(0.4f, GetThis<ObjectInterface>(), GetThis<ObjectInterface>(),L"Clear");
				return;
			}
		}
	}

	void Goal::OnUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();
		GetDynamicComponent<SmBaseDraw>()->UpdateAnimation(delta);

		bool isFind = dynamic_pointer_cast<GameStage>(GetStage())->IsEnemyFindPlayer();
		auto collision = GetComponent<CollisionObb>();

		if (m_IsWarning)
		{
			if (!isFind)
			{
				if (!m_IsSendEndWarning)
				{
					m_IsSendEndWarning = true;
					PostEvent(3.0f, GetThis<ObjectInterface>(), GetThis<ObjectInterface>(), L"EndWarning");
				}
			}
		}
		else
		{
			if (isFind)
			{
				m_IsWarning = true;
				m_EfkPlay2 = ObjectFactory::Create<EfkPlay>(m_GardEffect, GetComponent<Transform>()->GetPosition() + Vec3(0, -2, 0));
				collision->SetFixed(true);
				collision->SetIsHitAction(IsHitAction::Slide);
			}
		}
	}

	void Goal::OnEvent(const shared_ptr<Event>& msg)
	{
		if (msg->m_MsgStr == L"Clear" )
		{
			//BGMの再生
			auto XAPtr = App::GetApp()->GetXAudio2Manager();
			m_BGM = XAPtr->Start(L"GAMECLEAR_JG", 0, 0.5f);

			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>()->GetThis<ObjectInterface>(), L"StopBGM");
		}
		else if (msg->m_MsgStr == L"EndWarning")
		{
			auto collision = GetComponent<CollisionObb>();
			m_IsWarning = false;
			m_EfkPlay2->StopEffect();
			collision->SetFixed(false);
			collision->SetIsHitAction(IsHitAction::None);
			collision->SetMakedSize(1.0f);

			m_IsSendEndWarning = false;
		}
	}

	shared_ptr<MeshResource> Goal::GetMeshResourceForMinimap()
	{
		if (m_MeshResForMinimap)
			return m_MeshResForMinimap;

		Col4 color = Col4(0.1f, 0.8f, 0.8f, 1);
		float size = 1.0f;
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
}
// end basecross