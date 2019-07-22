#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Plane::Plane(const shared_ptr<Stage>& stagePtr, StaticObject::InitializeParam& initParam, InitializeParam& InitializeParam)
		: StaticObject(stagePtr, initParam),m_InitializeParam(InitializeParam)
	{}

	Plane::~Plane()
	{}

	void Plane::ChangePlaneTX(wstring texKey)
	{
		auto drawComp = GetComponent<PNTStaticDraw>();

		drawComp->SetTextureResource(texKey);
		
	}

	void Plane::OnCreate()
	{
		float right= m_initParam.startScale.x / m_TileSize;
		float bottom = m_initParam.startScale.y / m_TileSize;

		vector<VertexPositionNormalTexture> vertices =
		{
			{ VertexPositionNormalTexture(Vec3(-1.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f,0.0f),Vec2(0,0)) },
			{ VertexPositionNormalTexture(Vec3(1.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f,0.0f), Vec2(right,0)) },
			{ VertexPositionNormalTexture(Vec3(-1.0f, -1.0f,0.0f), Vec3(0.0f, 1.0f,0.0f),Vec2(0,bottom)) },
			{ VertexPositionNormalTexture(Vec3(1.0f, -1.0f,0.0f), Vec3(0.0f, 1.0f,0.0f), Vec2(right,bottom)) }
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2
		};
		 

		//�V���ȃ��b�V�������B
		MeshResource::CreateMeshResource(vertices, indices, false);
		
		// Transform�R���|�[�l���g�̎擾
		auto transComp = AddComponent<Transform>();

		
		// Position,Quaternion,Scale�̐ݒ�
		transComp->SetPosition(StaticObject::m_initParam.startPos);
		transComp->SetQuaternion(StaticObject::m_initParam.startQuat);
		transComp->SetScale(StaticObject::m_initParam.startScale);

		// �`��R���|�[�l���g�̎擾
		auto drawComp = AddComponent<PNTStaticDraw>();
		//��������Ƃ��Ȃ��Ƃ����Ƃ����Ԗڂɂ͂Ȃ�Ȃ��B
		drawComp->SetSamplerState(SamplerState::LinearWrap);

		//�^�C�����O�����̗v
		drawComp->CreateOriginalMesh(vertices, indices);
		drawComp->SetOriginalMeshUse(true);

		drawComp->SetTextureResource(m_InitializeParam.texKey);
		// ���b�V���̌`��̐ݒ�
		drawComp->SetMeshResource(L"DEFAULT_SQUARE");

		drawComp->SetDiffuse(Col4(1, 1, 1, 1));
		drawComp->SetEmissive(Col4(0, 0, 0, 0));
		drawComp->SetSpecular(Col4(0, 0, 0, 0));
		drawComp->SetLightingActive(false);

		//�^�O�t��
		AddTag(L"Plane");
	}

	void Plane::OnUpdate()
	{
		
	}
}
// end basecross