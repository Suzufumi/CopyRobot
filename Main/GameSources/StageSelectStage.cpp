#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void StageSelectStage::CreateViewLight()
	{
		auto PtrView = CreateView<SingleView>();
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 15.0f, -3.0f));
		PtrCamera->SetAt(Vec3(10.0f, 0.0f, 10.0f));

		auto PtrSingleLight = CreateLight<SingleLight>();
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	void StageSelectStage::OnCreate()
	{
		try
		{
			wstring dataDir;
			App::GetApp()->GetDataDirectory(dataDir);
			m_StageXmlPath = dataDir + L"Stage\\" + Util::IntToWStr(m_StageNum) + L".xml";

			XmlDocReader xmlReader(m_StageXmlPath);
			auto wayNodes = xmlReader.GetSelectNodes(L"GameStage/CameraMove/WayPoint");
			long wayCounts = XmlDocReader::GetLength(wayNodes);
			if (wayCounts > 0)
			{
				for (long i = 0; i < wayCounts; i++)
				{
					auto node = XmlDocReader::GetItem(wayNodes, i);
					wstring posStr = XmlDocReader::GetAttribute(node, L"Pos");
					Vec3 pos = MyUtil::unityVec3StrToBCVec3(posStr);
					m_CameraWays.push_back(pos);
				}
			}

			auto cameraNode = xmlReader.GetSelectSingleNode(L"GameStage/CameraMove");
			wstring posStr = XmlDocReader::GetAttribute(cameraNode, L"Pos");
			m_CameraStartPos = MyUtil::unityVec3StrToBCVec3(posStr);
			m_StartPos = m_CameraStartPos;

			//エフェクトの追加
			m_EfkInterface = ObjectFactory::Create<EfkInterface>();

			//ビューとライトの作成
			CreateViewLight();
			// セルマップの作成
			CreateStageCellMap();
			// Enemyの作成
			CreateEnemy();
			// 床の作成
			CreatePlane();
			//壁の作成
			CreateWall();
			//岩の作成
			CreateRock();
			//ゴールの作成
			CreateGoal();

			auto skySphere = AddGameObject<SkySphere>();

			m_RenderTarget = make_shared<PostEffectRenderTarget>();
			m_RenderTarget->ClearViews();

			auto camera = GetView()->GetTargetCamera();
			camera->SetEye(m_CameraStartPos);
			camera->SetAt(m_CameraWays.at(0));

			m_AtDir = (camera->GetAt() - camera->GetEye());
			m_AtDir.normalize();

			m_Transition = ObjectFactory::Create<Transition>();
		}
		catch (...)
		{
			throw;
		}
	}

	void StageSelectStage::OnUpdate()
	{
		m_EfkInterface->OnUpdate();
		m_Transition->OnUpdate();

		auto camera = GetView()->GetTargetCamera();
		Vec3 eye = camera->GetEye();
		Vec3 at = camera->GetAt();
		Vec3 nextPos = m_CameraWays.at(m_WayIndex);

		float delta = App::GetApp()->GetElapsedTime();
		m_TimeCount += delta;

		float distance = (m_StartPos - nextPos).length() * 0.5f;
		eye = Lerp::CalculateLerp(m_StartPos, nextPos, 0, distance, m_TimeCount, Lerp::Linear);
		at = eye + m_AtDir;

		if (m_TimeCount / (distance * 0.5f) > 0.8f)
		{
			if (m_WayIndex < static_cast<signed>(m_CameraWays.size()) - 1)
			{
				Vec3 nextAtDir = (m_CameraWays[m_WayIndex + 1] - eye);
				nextAtDir.normalize();
				Vec3 nowDir = (m_CameraWays[m_WayIndex]-eye);
				nowDir.normalize();
				Vec3 newDir = Lerp::CalculateLerp(nowDir, nextAtDir, distance*0.5f*0.8f, distance, m_TimeCount, Lerp::Linear);
				at = eye + newDir;
			}
		}

		camera->SetEye(eye);
		camera->SetAt(at);

		if (m_TimeCount > distance * 0.9f)
		{
			if (m_WayIndex + 1 >= static_cast<signed>(m_CameraWays.size()))
			{
				if (!m_Transition->GetIsFadeNow())
				{
					auto transOut = ObjectFactory::Create<NormalFadeTransition>(distance*0.1f, TransitionState::FadeOut, Col4(0, 0, 0, 1));
					m_Transition->PlayFadeOut(transOut,true);
				}
			}
		}

		if (m_TimeCount > distance)
		{
			m_WayIndex++;
			m_TimeCount = 0.0f;
			if (m_WayIndex >= static_cast<signed>(m_CameraWays.size())) 
			{
				m_StartPos = m_CameraStartPos;
				m_WayIndex = 0;
				camera->SetEye(m_CameraStartPos);
				camera->SetAt(m_CameraWays.at(0));

				m_AtDir = (camera->GetAt() - camera->GetEye());
				m_AtDir.normalize();

				auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn, Col4(0, 0, 0, 1));
				m_Transition->PlayFadeIn(transIn);
			}
			else
			{
				m_StartPos = m_CameraWays[m_WayIndex-1];
				m_AtDir = (m_CameraWays[m_WayIndex] - m_StartPos);
				m_AtDir.normalize();
			}
		}
	}

	void StageSelectStage::OnDraw()
	{
		auto dev = App::GetApp()->GetDeviceResources();
		dev->ClearDefaultViews();

		m_RenderTarget->ClearViews();
		m_RenderTarget->StartRenderTarget();

		m_EfkInterface->OnDraw();

		for (auto obj : GetGameObjectVec())
		{
			if(!obj->FindTag(L"CellMap"))
				obj->OnDraw();
		}

		m_RenderTarget->EndRenderTarget();

		m_Transition->OnDraw();

		dev->ClearDefaultViews();
	}
}