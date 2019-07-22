#pragma once
#include "stdafx.h"

namespace basecross
{
	class Plane :public StaticObject
	{
	public:
		struct InitializeParam 
		{
			wstring texKey;
		};

	private:
		InitializeParam m_InitializeParam;

		
		float m_TileSize = 0.5f;

		//�o�b�N�A�b�v���_�f�[�^
		vector<VertexPositionColor> m_BackupVertices;

	public:
		Plane(const shared_ptr<Stage>& stagePtr, StaticObject::InitializeParam& initParam, InitializeParam& InitializeParam);
		~Plane();

		void ChangePlaneTX(wstring texKey);
		// �\�z
		void OnCreate() override;
		// �X�V
		void OnUpdate() override;
	};
}