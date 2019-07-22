#pragma once
#include "stdafx.h"

namespace basecross
{
	class Pillar :public StaticObject
	{

	public:
		struct InitializeParam
		{
			wstring texKey;
			float m_ShotInterval;
			float m_ShotBulletSpeed;
		};

	private:
		InitializeParam m_InitializeParam;

		shared_ptr<MeshResource> m_MeshResForMinimap;

		float m_ShotIntervalCount;
	public:
		Pillar(const shared_ptr<Stage>& stagePtr, StaticObject::InitializeParam& initParam, InitializeParam& InitializeParam);
		Pillar(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);

		~Pillar();

		// 構築
		void OnCreate() override;
		// 更新
		void OnUpdate() override;

		shared_ptr<MeshResource> GetMeshResourceForMinimap();

		virtual void EndTakeIn()override;

		bool CheckIsWarning();
		void ShotBulletToPlayer();
		void SHotBulletToPillar();
		bool IsScenePlayer();
	};

	class CrystalTable :public StaticObject
	{
		shared_ptr<MeshResource> m_MeshResForMinimap;
	public:
		CrystalTable(const shared_ptr<Stage>& stagePtr, StaticObject::InitializeParam initParam);
		CrystalTable(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);

		~CrystalTable();

		// 構築
		void OnCreate() override;
		// 更新
		void OnUpdate() override;

		shared_ptr<MeshResource> GetMeshResourceForMinimap();

		virtual void EndTakeIn()override;
	};
}