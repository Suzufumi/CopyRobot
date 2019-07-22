#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	MiniMap::MiniMap(const shared_ptr<Stage>& stage):
		GameObject(stage)
	{

	}

	void MiniMap::OnCreate()
	{
		m_RenderTarget = make_shared<PostEffectRenderTarget>();

		m_PosDiff = Vec3(0, 40.0f, 0);
		m_LookAtDiff = Vec3(0, 0, 0.0f);

		m_Camera = ObjectFactory::Create<Camera>();
		m_Camera->SetEye(m_PosDiff);
		m_Camera->SetAt(m_LookAtDiff);
		m_Camera->SetUp(Vec3(0, 0, 1));
		m_Camera->SetWidth(640.0f);
		m_Camera->SetHeight(480.0f);
		m_Camera->SetPers(true);

		m_Sprite = ObjectFactory::Create<PostEffectSprite>(GetStage(),m_RenderTarget);
		auto spriteTrans = m_Sprite->GetComponent<Transform>();
		spriteTrans->SetPosition(Vec3(-500.0f, 290.0f, 0));
		spriteTrans->SetScale(Vec3(640.0f * 0.35f, 480.0f * 0.35f, 0.0f));
		m_Sprite->ChangePostEffectColor(PostEffectColor::Color);

		m_BackSprite = ObjectFactory::Create<Sprite>(GetStage(),
			L"Minimap_Back_TX",
			Vec2(1280.0f, 800.0f),
			0.0f,
			Vec2(0, 0),
			1, 1
			);
		m_BackSprite->SetDrawLayer(-100);

		m_FrameSprite = ObjectFactory::Create<Sprite>(GetStage(),
			L"Minimap_Frame_TX",
			Vec2(640.0f * 0.38f, 480.0f * 0.39f),
			0.0f,
			Vec2(-500.0f, 286.0f),
			1, 1);

		auto drawComp = AddComponent<PCStaticDraw>();
		drawComp->SetRasterizerState(RasterizerState::DoubleDraw);

		AddTag(L"MiniMap");

		SetDrawLayer(10);
	}

	void MiniMap::OnUpdate()
	{
		if (!m_Target.lock())
			return;

		Vec3 setPos = m_Target.lock()->GetPosition();
		setPos.y = 0.0f;
		setPos += m_PosDiff;

		m_Camera->SetEye(setPos);
		m_Camera->SetAt(m_Target.lock()->GetPosition() + m_LookAtDiff);
		
		//auto defaultCam = GetStage()->GetView()->GetTargetCamera();
		//Vec3 forward = defaultCam->GetAt() - defaultCam->GetEye();
		//forward.y = 0.0f;
		//forward.normalize();

		//m_Camera->SetUp(forward);
	}

	void MiniMap::OnDraw()
	{
		m_Sprite->OnDraw();
		m_FrameSprite->OnDraw();
	}

	void MiniMap::MiniMapDraw()
	{
		auto dev = App::GetApp()->GetDeviceResources();

		auto view = GetStage()->GetView();
		auto singleView = dynamic_pointer_cast<SingleView>(view);

		auto originalCam = singleView->GetCamera();
		singleView->SetCamera(m_Camera);

		m_RenderTarget->ClearViews();
		m_RenderTarget->StartRenderTarget();
		//////////////////////////////////////////////////////////////////////////
		m_BackSprite->OnDraw();

		//壁
		vector<shared_ptr<GameObject>> walls;
		GetStage()->GetUsedTagObjectVec(L"Wall", walls);
		for (auto wall : walls)
			MapObjectDraw<Wall>(wall);

		//敵
		vector<shared_ptr<GameObject>> enemies;
		GetStage()->GetUsedTagObjectVec(L"Enemy", enemies);
		for (auto enemy : enemies)
			MapObjectDraw<Enemy>(enemy,1.25f);

		//ゴール
		vector<shared_ptr<GameObject>> goals;
		GetStage()->GetUsedTagObjectVec(L"Goal", goals);
		for (auto goal : goals)
			MapObjectDraw<Goal>(goal);

		//岩
		vector<shared_ptr<GameObject>> rocks;
		GetStage()->GetUsedTagObjectVec(L"Pillar", rocks);
		for (auto rock : rocks)
			MapObjectDraw<Pillar>(rock);

		//クリスタルテーブル
		vector<shared_ptr<GameObject>> crystalTables;
		GetStage()->GetUsedTagObjectVec(L"CrystalTable", crystalTables);
		for (auto crystalTable : crystalTables)
			MapObjectDraw<CrystalTable>(crystalTable);

		//プレイヤー
		auto target = m_Target.lock()->GetGameObject();
		MapObjectDraw<Player>(target,1.25f);
		//////////////////////////////////////////////////////////////////////////
		m_RenderTarget->EndRenderTarget();

		singleView->SetCamera(originalCam);

		dev->ClearDefaultViews();
	}
}