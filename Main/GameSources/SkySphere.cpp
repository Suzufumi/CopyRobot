#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void SkySphere::OnCreate()
	{
		auto camera = GetStage()->GetView()->GetTargetCamera();

		auto trans = GetComponent<Transform>();
		trans->SetScale(Vec3(200, 200, 200));
		trans->SetPosition(camera->GetEye());

		vector<VertexPositionNormalTexture> vertices;
		//’¸“_’è‹`‚ğ•Ï‚¦‚é
		vector<VertexPositionColorTexture> new_vertices;

		vector<uint16_t> indices;
		//‹…‘Ì‚Ìì¬
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++)
		{
			VertexPositionColorTexture temp;
			temp.position = vertices[i].position;
			temp.textureCoordinate = vertices[i].textureCoordinate;
			temp.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			new_vertices.push_back(temp);
		}

		auto PtrDraw = AddComponent<PCTStaticDraw>();
		PtrDraw->CreateOriginalMesh(new_vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetSamplerState(SamplerState::LinearClamp);
		PtrDraw->SetDepthStencilState(DepthStencilState::Read);
		SetAlphaActive(true);
	}

	void SkySphere::OnUpdate()
	{
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto trans = GetComponent<Transform>();
		trans->SetPosition(camera->GetEye());
	}
}