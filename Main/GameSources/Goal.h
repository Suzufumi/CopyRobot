#pragma once
#include "stdafx.h"

namespace basecross
{
	class Goal :public StaticObject
	{
		//エフェクト
		shared_ptr<EfkEffect> m_DownEffect;
		shared_ptr<EfkEffect> m_GardEffect;

		//エフェクト実行オブジェクト
		shared_ptr<EfkPlay> m_EfkPlay;
		shared_ptr<EfkPlay> m_EfkPlay2;

		unique_ptr<InputHandler> m_InputHandler;

		shared_ptr<MeshResource> m_MeshResForMinimap;

		shared_ptr<SoundItem> m_BGM;

		bool m_IsSendEndWarning;
		bool m_IsWarning;

	public:
		struct InitializeParam
		{
			wstring texKey;
		};
	private:

		InitializeParam m_InitializeParam;

	public:
		Goal(const shared_ptr<Stage>& stagePtr, StaticObject::InitializeParam& initParam, InitializeParam& InitializeParam);
		Goal(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		~Goal();

		// 構築
		void OnCreate() override;

		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
		// 更新
		void OnUpdate() override;

		void OnEvent(const shared_ptr<Event>& msg) override;

		shared_ptr<MeshResource> GetMeshResourceForMinimap();
	};
}