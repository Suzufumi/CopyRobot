#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	BillboardSquare::BillboardSquare(const shared_ptr<Stage>& stagePtr, BillboardSquare::InitParam initParam) :
		GameObject(stagePtr),
		m_InitParam(initParam)
	{

	}

	void BillboardSquare::OnCreate()
	{
		float halfSize = 0.5f;
		float xScale = m_InitParam.m_AspectWdivH * 0.5f;

		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-halfSize * xScale, halfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(halfSize * xScale, halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(1, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-halfSize * xScale, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 1)) },
			{ VertexPositionColorTexture(Vec3(halfSize * xScale, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(1, 1)) },
		};

		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);

		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_InitParam.m_Position);
		trans->SetScale(m_InitParam.m_Scale);

		auto drawComp = AddComponent<PCTStaticDraw>();
		drawComp->SetTextureResource(m_InitParam.m_TexKey);
		drawComp->SetMeshResource(m_SquareMesh);
		drawComp->SetBlendState(BlendState::AlphaBlend);
		drawComp->SetDepthStencilState(DepthStencilState::None);
	}

	void BillboardSquare::OnUpdate()
	{
		auto ptrCamera = GetStage()->GetView()->GetTargetCamera();
		Quat qt;
		//向きをビルボードにする
		qt = MyUtil::createBillboard(ptrCamera->GetAt() - ptrCamera->GetEye());
		GetComponent<Transform>()->SetQuaternion(qt);
	}

	void BillboardSquare::SetMeshVertexPositionUvX(float value)
	{
		m_Value = value;

		float halfSize = 0.5f;
		float xScale = m_InitParam.m_AspectWdivH * 0.5f;

		float width = halfSize * xScale * 2.0f;
		float widthHalf = width / 2.0f;

		float setValue = width * value - widthHalf;

		for (size_t i = 0; i < m_BackupVertices.size(); i++)
		{
			if (i == 1 || i == 3)
			{
				m_BackupVertices[i].position.x = setValue;
				m_BackupVertices[i].textureCoordinate.x = value;
			}
		}

		m_SquareMesh->UpdateVirtexBuffer(m_BackupVertices);
	}


	CopySuccessGauge::CopySuccessGauge(const shared_ptr<Stage>& stagePtr, CopySuccessGauge::InitParam initParam) :
		GameObject(stagePtr),
		m_InitParam(initParam),
		m_IsVisible(false),
		m_IsRunCloseAction(false),
		m_IsRunOpenAction(false),
		m_IsOpened(false),
		m_IsPlaySuccessVoice(false)
	{

	}

	void CopySuccessGauge::OnCreate()
	{
		auto target = m_InitParam.m_Target.lock();
		if (!target)
			return;

		m_SquareInitParam.m_Scale = Vec3(0.5f, 0.5f, 0.5f);
		m_SquareInitParam.m_AspectWdivH = 4.04487179;
		m_SquareInitParam.m_Position = target->GetPosition() + m_InitParam.m_PositionDiff;
		m_SquareInitParam.m_TexKey = L"GAUGE_BASE_TX";

		auto trans = GetComponent<Transform>();
		trans->SetPosition(target->GetPosition() + m_InitParam.m_PositionDiff);
		trans->SetScale(Vec3(0.5f, 0.5f, 0.5f));

		m_BackSquare = ObjectFactory::Create<BillboardSquare>(GetStage(), m_SquareInitParam);
		m_BackSquare->GetComponent<Transform>()->SetParent(GetThis<GameObject>());
		m_Squares.push_back(m_BackSquare);

		m_SquareInitParam.m_AspectWdivH = 3.4f;
		m_SquareInitParam.m_TexKey = L"GAUGE_COPYING_TX";
		m_FrontSquare = ObjectFactory::Create<BillboardSquare>(GetStage(), m_SquareInitParam);
		m_FrontSquare->GetComponent<Transform>()->SetParent(GetThis<GameObject>());
		m_Squares.push_back(m_FrontSquare);

		m_SquareInitParam.m_AspectWdivH = 4.04487179;
		m_SquareInitParam.m_TexKey = L"BLUE_SUCCESS_FRAME";
		m_FrameSquare = ObjectFactory::Create<BillboardSquare>(GetStage(), m_SquareInitParam);
		m_FrameSquare->GetComponent<Transform>()->SetParent(GetThis<GameObject>());
		m_Squares.push_back(m_FrameSquare);

		m_SquareInitParam.m_TexKey = L"SUCCESS_GAUGE_PROGRESS";
		m_StateSquare = ObjectFactory::Create<BillboardSquare>(GetStage(), m_SquareInitParam);
		m_StateSquare->GetComponent<Transform>()->SetParent(GetThis<GameObject>());
		m_Squares.push_back(m_StateSquare);

		SetDrawLayer(200);
	}

	void CopySuccessGauge::OnUpdate()
	{
		if (m_IsRunCloseAction)
		{
			if (GetComponent<Action>()->IsArrived())
			{
				m_IsVisible = false;
				m_IsRunCloseAction = false;
			}
		}

		if (m_IsRunOpenAction)
		{
			if (GetComponent<Action>()->IsArrived())
			{
				m_IsRunOpenAction = false;
			}
		}
	}

	void CopySuccessGauge::OnUpdate2()
	{
		auto target = m_InitParam.m_Target.lock();
		if (!target)
			return;

		Vec3 setPos = target->GetPosition() + m_InitParam.m_PositionDiff;
		setPos.y = m_InitParam.m_PositionDiff.y;
		GetComponent<Transform>()->SetPosition(setPos);

		auto trans = GetComponent<Transform>();
		for (auto square : m_Squares)
		{
			square->GetComponent<Transform>()->SetScale(trans->GetScale());

			square->OnUpdate();
			square->ComponentUpdate();
		}
	}

	void CopySuccessGauge::OnDraw()
	{
		if (!m_IsVisible)
			return;

		for (auto square : m_Squares)
			square->OnDraw();
	}

	void CopySuccessGauge::OnEvent(const shared_ptr<Event>& event)
	{
		if (event->m_MsgStr == L"Close")
		{
			RunCloseAction();
		}
	}

	void CopySuccessGauge::SetGaugeValue(float value)
	{
		if (value < 1.0f)
		{
			m_FrontSquare->SetMeshVertexPositionUvX(value);

			if (value <= 0.0f)
				RunCloseAction();
		}
		else
		{
			m_FrontSquare->GetDynamicComponent<SmBaseDraw>()->SetTextureResource(L"GAUGE_ABILITY_BAR_SUCCESS");
			m_FrameSquare->GetDynamicComponent<SmBaseDraw>()->SetTextureResource(L"ORANGE_SUCCESS_FRAME");
			m_StateSquare->GetDynamicComponent<SmBaseDraw>()->SetTextureResource(L"SUCCESS_GAUGE_FINISH");

			if (!m_IsPlaySuccessVoice)
			{
				auto item = App::GetApp()->GetXAudio2Manager();
				item->Start(L"SUCCESS_SE", 0);
				m_IsPlaySuccessVoice = true;
			}

			PostEvent(1.0f, GetThis<ObjectInterface>(), GetThis<ObjectInterface>(), L"Close");
		}
	}

	void CopySuccessGauge::RunOpenAction()
	{
		if (m_IsVisible && m_IsOpened)
			return;

		if (m_IsRunOpenAction)
			return;

		auto item = App::GetApp()->GetXAudio2Manager();
		item->Start(L"UI_OPEN_SE", 0);

		if (m_IsRunCloseAction)
		{
			GetComponent<Action>()->Stop();
			m_IsRunCloseAction = false;
		}

		m_IsVisible = true;
		m_IsRunOpenAction = true;
		m_IsOpened = true;

		m_FrontSquare->GetDynamicComponent<SmBaseDraw>()->SetTextureResource(L"GAUGE_COPYING_TX");
		m_FrameSquare->GetDynamicComponent<SmBaseDraw>()->SetTextureResource(L"BLUE_SUCCESS_FRAME");
		m_StateSquare->GetDynamicComponent<SmBaseDraw>()->SetTextureResource(L"SUCCESS_GAUGE_PROGRESS");

		RemoveComponent<Action>();

		GetComponent<Transform>()->SetScale(Vec3(0.8f, 0, 0.8f));

		auto action = AddComponent<Action>();
		action->AddScaleTo(0.2f, Vec3(0.5f, 0.5f, 0.5f),Lerp::EaseOut);
		action->Run();
	}

	void CopySuccessGauge::RunCloseAction()
	{
		if (m_IsRunCloseAction || !m_IsOpened)
			return;

		auto item = App::GetApp()->GetXAudio2Manager();
		item->Start(L"UI_CLOSE_SE", 0);

		m_IsRunOpenAction = false;
		m_IsRunCloseAction = true;
		m_IsOpened = false;
		m_IsPlaySuccessVoice = false;

		RemoveComponent<Action>();

		auto action = AddComponent<Action>();
		action->AddScaleTo(0.2f, Vec3(0.8f, 0, 0.8f),Lerp::Easein);
		action->Run();
	}

	UsageTimeGauge::UsageTimeGauge(const shared_ptr<Stage>& stagePtr, CopySuccessGauge::InitParam initParam) :
		CopySuccessGauge(stagePtr, initParam)
	{

	}

	void UsageTimeGauge::OnCreate()
	{
		auto target = m_InitParam.m_Target.lock();
		if (!target)
			return;

		m_SquareInitParam.m_Scale = Vec3(0.5f, 0.5f, 0.5f);
		m_SquareInitParam.m_AspectWdivH = 10;
		m_SquareInitParam.m_Position = target->GetPosition() + m_InitParam.m_PositionDiff;
		m_SquareInitParam.m_TexKey = L"COPY_TIME_GAUGE_BASE_TX";

		auto trans = GetComponent<Transform>();
		trans->SetPosition(target->GetPosition() + m_InitParam.m_PositionDiff);
		trans->SetScale(Vec3(0.4f, 0.4f, 0.4f));

		m_BackSquare = ObjectFactory::Create<BillboardSquare>(GetStage(), m_SquareInitParam);
		m_BackSquare->GetComponent<Transform>()->SetParent(GetThis<GameObject>());
		m_Squares.push_back(m_BackSquare);

		m_SquareInitParam.m_AspectWdivH = 10.5;
		m_SquareInitParam.m_TexKey = L"COPY_TIME_GAUGE_BAR_TX";
		m_FrontSquare = ObjectFactory::Create<BillboardSquare>(GetStage(), m_SquareInitParam);
		m_FrontSquare->GetComponent<Transform>()->SetParent(GetThis<GameObject>());
		m_Squares.push_back(m_FrontSquare);

		SetDrawLayer(200);
	}

	void UsageTimeGauge::RunOpenAction()
	{
		if (m_IsVisible && m_IsOpened)
			return;

		if (m_IsRunOpenAction)
			return;

		auto item = App::GetApp()->GetXAudio2Manager();
		item->Start(L"UI_OPEN_SE", 0);

		if (m_IsRunCloseAction)
		{
			GetComponent<Action>()->Stop();
			m_IsRunCloseAction = false;
		}

		m_IsVisible = true;
		m_IsRunOpenAction = true;
		m_IsOpened = true;

		RemoveComponent<Action>();

		GetComponent<Transform>()->SetScale(Vec3(0.8f, 0, 0.8f));

		auto action = AddComponent<Action>();
		action->AddScaleTo(0.2f, Vec3(0.3f, 0.3f, 0.3f), Lerp::EaseOut);
		action->Run();
	}

	void UsageTimeGauge::SetGaugeValue(float value)
	{
		float result = value * 0.75f + 0.2f;

		m_FrontSquare->SetMeshVertexPositionUvX(result);
	}
}