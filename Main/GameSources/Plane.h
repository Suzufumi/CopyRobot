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

		//バックアップ頂点データ
		vector<VertexPositionColor> m_BackupVertices;

	public:
		Plane(const shared_ptr<Stage>& stagePtr, StaticObject::InitializeParam& initParam, InitializeParam& InitializeParam);
		~Plane();

		void ChangePlaneTX(wstring texKey);
		// 構築
		void OnCreate() override;
		// 更新
		void OnUpdate() override;
	};
}