#pragma once
#include "stdafx.h"

namespace basecross
{
	class Goal :public StaticObject
	{
		//�G�t�F�N�g
		shared_ptr<EfkEffect> m_DownEffect;
		shared_ptr<EfkEffect> m_GardEffect;

		//�G�t�F�N�g���s�I�u�W�F�N�g
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

		// �\�z
		void OnCreate() override;

		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
		// �X�V
		void OnUpdate() override;

		void OnEvent(const shared_ptr<Event>& msg) override;

		shared_ptr<MeshResource> GetMeshResourceForMinimap();
	};
}