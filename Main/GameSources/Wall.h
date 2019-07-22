#pragma once
#include "stdafx.h"

namespace basecross
{
	class Wall :public StaticObject
	{
	public:
		struct InitializeParam
		{
			wstring texKey;
		};

	private:
		InitializeParam m_InitializeParam;


		float m_TileSize=1;
		shared_ptr<MeshResource> m_MeshResource;
		shared_ptr<MeshResource> m_MeshResForMinimap;

	public:
		Wall(const shared_ptr<Stage>& stagePtr, StaticObject::InitializeParam& initParam, InitializeParam& InitializeParam);
		Wall(const shared_ptr<Stage>& stagePtr,IXMLDOMNodePtr pNode);
		~Wall();

		// ç\íz
		void OnCreate() override;
		// çXêV
		void OnUpdate() override;

		shared_ptr<MeshResource> GetMeshResourceForMinimap();
	};


}