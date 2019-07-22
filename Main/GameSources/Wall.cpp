#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Wall::Wall(const shared_ptr<Stage>& stagePtr, StaticObject::InitializeParam& initParam, InitializeParam& InitializeParam)
		: StaticObject(stagePtr, initParam), m_InitializeParam(InitializeParam)
	{}

	Wall::Wall(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
		: StaticObject(stagePtr, StaticObject::InitializeParam())
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

		Wall::InitializeParam wInitParam;
		wInitParam.texKey = L"STAGE_TX";

		Wall::m_InitializeParam = wInitParam;
	}


	Wall::~Wall()
	{

	}


	void Wall::OnCreate()
	{
		float right = m_initParam.startScale.x / m_TileSize;
		float bottom = m_initParam.startScale.y / m_TileSize;
		float austenite = m_initParam.startScale.z / m_TileSize;



		vector<VertexPositionNormalTexture> vertices;
		//インデックス配列
		vector<uint16_t> indices;


		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);

		MeshUtill::CreateCube(1, vertices, indices);

		for (auto& v : vertices)
		{
			Vec3 norm = v.normal;

			//上
			if (norm.angle(Vec3(0, 1, 0)) < 0.01f)
			{
				if (v.textureCoordinate.x >= 1.0f)
					v.textureCoordinate.x = StaticObject::m_initParam.startScale.x / m_TileSize;
				if (v.textureCoordinate.y >= 1.0f)
					v.textureCoordinate.y = StaticObject::m_initParam.startScale.z / m_TileSize;
			}
			//下
			else if (norm.angle(Vec3(0, -1, 0)) < 0.01f)
			{
				if (v.textureCoordinate.x >= 1.0f)
					v.textureCoordinate.x = StaticObject::m_initParam.startScale.x / m_TileSize;
				if (v.textureCoordinate.y >= 1.0f)
					v.textureCoordinate.y = StaticObject::m_initParam.startScale.z / m_TileSize;
			}
			//右
			else if (norm.angle(Vec3(1, 0, 0)) < 0.01f)
			{
				if (v.textureCoordinate.x >= 1.0f)
					v.textureCoordinate.x = StaticObject::m_initParam.startScale.z / m_TileSize;
				if (v.textureCoordinate.y >= 1.0f)
					v.textureCoordinate.y = StaticObject::m_initParam.startScale.y / m_TileSize;
			}
			//左
			else if (norm.angle(Vec3(-1, 0, 0)) < 0.01f)
			{
				if (v.textureCoordinate.x >= 1.0f)
					v.textureCoordinate.x = StaticObject::m_initParam.startScale.z / m_TileSize;
				if (v.textureCoordinate.y >= 1.0f)
					v.textureCoordinate.y = StaticObject::m_initParam.startScale.y / m_TileSize;
			}
			//奥
			else if (norm.angle(Vec3(0, 0, 1)) < 0.01f)
			{
				if (v.textureCoordinate.x >= 1.0f)
					v.textureCoordinate.x = StaticObject::m_initParam.startScale.x / m_TileSize;
				if (v.textureCoordinate.y >= 1.0f)
					v.textureCoordinate.y = StaticObject::m_initParam.startScale.y / m_TileSize;
			}
			//手前
			else if (norm.angle(Vec3(0, 0, -1)) < 0.01f)
			{
				if (v.textureCoordinate.x >= 1.0f)
					v.textureCoordinate.x = StaticObject::m_initParam.startScale.x / m_TileSize;
				if (v.textureCoordinate.y >= 1.0f)
					v.textureCoordinate.y = StaticObject::m_initParam.startScale.y / m_TileSize;
			}
		}

		float Ucount = StaticObject::m_initParam.startScale.x;

		//新たなメッシュを作る。
		m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, true);

		//Transformコンポーネントの取得
		auto transComp = AddComponent<Transform>();


		// Position,Quaternion,Scaleの設定
		transComp->SetPosition(StaticObject::m_initParam.startPos);
		transComp->SetQuaternion(StaticObject::m_initParam.startQuat);
		transComp->SetScale(StaticObject::m_initParam.startScale);

		// 描画コンポーネントの取得
		auto drawComp = AddComponent<PNTStaticDraw>();
		//こいついれとかないとちゃんとした網目にはならない。
		drawComp->SetSamplerState(SamplerState::LinearWrap);
		drawComp->SetBlendState(BlendState::AlphaBlend);
		drawComp->SetRasterizerState(RasterizerState::DoubleDraw);
		drawComp->SetDiffuse(Col4(1, 1, 1, 1));
		drawComp->SetEmissive(Col4(0, 0, 0, 0));
		drawComp->SetSpecular(Col4(0, 0, 0, 0));
		drawComp->SetLightingActive(false);

		//タイリング処理の要
		drawComp->CreateOriginalMesh(vertices, indices);
		drawComp->SetOriginalMeshUse(true);

		drawComp->SetTextureResource(m_InitializeParam.texKey);
		//drawComp->SetLineTextureResource(L"STAGE_LINE_TX");

		// メッシュの形状の設定
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//タグ付け
		AddTag(L"Wall");
	}

	void Wall::OnUpdate()
	{

	}


	shared_ptr<MeshResource> Wall::GetMeshResourceForMinimap()
	{
		if (m_MeshResForMinimap)
			return m_MeshResForMinimap;

		vector<VertexPositionColor> thisVertices;
		vector<uint16_t> thisIndices;

		auto wallVertices = m_MeshResource->GetBackupVerteces<VertexPositionNormalTexture>();
		auto wallIndices = m_MeshResource->GetBackupIndices<VertexPositionNormalTexture>();

		size_t faceNum = 5;
		size_t faceVertexCount = 4;
		size_t faceIndexCount = 6;
		for (size_t i = faceNum * faceVertexCount; i < faceVertexCount + faceNum * faceVertexCount; i++)
		{
			VertexPositionColor vpc;
			vpc.color = Col4(0.1f, 0.8f, 0.1f, 1);
			vpc.position = wallVertices[i].position;

			//したが空いてたらちょっと色を暗くする
			Vec4 worldPos(vpc.position.x, vpc.position.y, vpc.position.z, 1.0f);
			worldPos *= GetComponent<Transform>()->GetWorldMatrix();
			if (worldPos.y > 0.1f)
				vpc.color *= 0.6f;

			vpc.position.y = 0.0f;
			thisVertices.push_back(vpc);
		}

		for (size_t i = 0; i < faceIndexCount; i++)
		{
			thisIndices.push_back(wallIndices[i]);
		}

		m_MeshResForMinimap = MeshResource::CreateMeshResource<VertexPositionColor>(thisVertices, thisIndices, true);

		return m_MeshResForMinimap;
	}
}
	