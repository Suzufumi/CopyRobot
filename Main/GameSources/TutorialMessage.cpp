#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void TutorialMessage::OnCreate()
	{
		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_InitParam.m_StartPos);
		trans->SetScale(Vec3(1280.0f*0.8f, 800.0f*0.0f, 0.0f));

		float HelfSize = 0.5f;
		//頂点配列(縦横5個ずつ表示)
		vector<VertexPositionTexture> vertices = {
			{ VertexPositionTexture(Vec3(-HelfSize, HelfSize, 0),Vec2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Vec3(HelfSize, HelfSize, 0),  Vec2(1.0f, 0.0f)) },
			{ VertexPositionTexture(Vec3(-HelfSize, -HelfSize, 0),Vec2(0.0f, 1.0f)) },
			{ VertexPositionTexture(Vec3(HelfSize, -HelfSize, 0),  Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };

		auto drawComp = AddComponent<PTSpriteDraw>(vertices,indices);
		drawComp->SetTextureResource(m_InitParam.m_TexKey);
		drawComp->SetBlendState(BlendState::AlphaBlend);

		auto action = AddComponent<Action>();
		action->AddScaleTo(0.2f, Vec3(1280.0f*0.6f, 800.0f*0.6f, 0.0f), Lerp::EaseOut);
		action->Run();

		auto item = App::GetApp()->GetXAudio2Manager();
		item->Start(L"UI_OPEN_SE", 0);

		vector<shared_ptr<GameObject>> player;
		GetStage()->GetUsedTagObjectVec(L"Player", player);
		player[0]->SetUpdateActive(false);

		dynamic_pointer_cast<GameStage>(GetStage())->StopAllEnemies();

		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto tpsCam = dynamic_pointer_cast<TpsCamera>(camera);
		if (tpsCam)
		{
			tpsCam->Fix(tpsCam->GetEye(), tpsCam->GetAt());
		}

		SetDrawLayer(500);
	}

	void TutorialMessage::OnUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();

		m_TimeCount += delta;
		if (m_TimeCount > m_ButtonEnableTime)
		{
			auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
			if (pad.bConnected)
			{
				if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
				{
					Close();
				}
			}
		}
	}

	void TutorialMessage::OnEvent(const shared_ptr<Event>& event)
	{
		if (event->m_MsgStr == L"Destroy")
		{
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
		}
	}

	void TutorialMessage::Close()
	{
		auto action = AddComponent<Action>();
		if (!action->GetArrived())
			return;

		action->AllActionClear();
		action->AddScaleTo(0.2f, Vec3(1280.0f*0.8f, 800.0f*0.0f, 0.0f), Lerp::Easein);
		action->Run();

		vector<shared_ptr<GameObject>> player;
		GetStage()->GetUsedTagObjectVec(L"Player", player);
		player[0]->SetUpdateActive(true);

		dynamic_pointer_cast<GameStage>(GetStage())->ActiveAllEnemies();

		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto tpsCam = dynamic_pointer_cast<TpsCamera>(camera);
		if (tpsCam)
		{
			tpsCam->EndClose();
		}

		auto item = App::GetApp()->GetXAudio2Manager();
		item->Start(L"UI_CLOSE_SE", 0);

		PostEvent(0.2f, GetThis<ObjectInterface>(), GetThis<ObjectInterface>(), L"Destroy");
	}
}