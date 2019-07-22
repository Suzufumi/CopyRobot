/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Player::Player(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
		: AliveGameObject(stagePtr), 
		m_CopySuccessGaugeValue(0.0f),
		m_CopyValueDecreaseSpeedStaticObj(0.1f),
		m_CopyValueDecreaseSpeedEnemy(0.15f),
		m_CopyAbilityGaugeValue(0.0f)

	{
		// XmlからPlayerの情報を取得
		LoadXmlParam(pNode, m_initParam.m_startPos, m_initParam.m_startQuat, m_initParam.m_startScale);
	}


	//構築
	void Player::OnCreate()
	{
		// Transformコンポーネントの取得
		auto trans = AddComponent<Transform>();
		// Transformの設定
		trans->SetPosition(m_initParam.m_startPos);
		trans->SetQuaternion(m_initParam.m_startQuat);
		trans->SetScale(m_initParam.m_startScale);

		//Rigidbodyをつける
		m_InputHandler.reset(new InputHandler());
		auto PtrRedid = AddComponent<Rigidbody>();
		auto PtrColl = AddComponent<CollisionObb>();
		PtrColl->SetIsHitAction(IsHitAction::Slide);
		PtrColl->SetMakedScale(Vec3(1,2,1));
		PtrColl->SetPositionDiff(Vec3(0, 0.5f, 0));

		// プレイヤーの変換行列を作成
		auto spanMat = SetModelMatrix(Vec3(1.0f), Vec3(0.0f), Vec3(0.0f, XM_PI, 0.0f), Vec3(0.0f, -1.0f, 0.0f));

		auto PtrDraw = AddComponent<PNTBoneModelDraw>();
		PtrDraw->SetMeshResource(L"PLAYER_MESH");
		PtrDraw->SetRasterizerState(RasterizerState::CullBack);
		PtrDraw->SetBlendState(BlendState::AlphaBlend);
		PtrDraw->SetMeshToTransformMatrix(spanMat);
		PtrDraw->SetLineTextureResource(L"PLAYER_LINE_TEX");
		PtrDraw->AddAnimation(L"Default", 0, 119, true);
		PtrDraw->AddAnimation(L"CopyOpen", 200, 59, false,60.0f);
		PtrDraw->AddAnimation(L"CopyClose", 260, 59, false,60.0f);
		PtrDraw->AddAnimation(L"Goal", 950, 89, false,45.0f);
		SetAlphaActive(true);

		auto PtrCamera = dynamic_pointer_cast<TpsCamera>(OnGetDrawCamera());
		if (PtrCamera)
		{
			PtrCamera->SetTarget(trans);
		}

		auto stageObj = GetStage()->GetGameObjectVec();
		for (auto obj : stageObj)
		{
			auto miniMap = dynamic_pointer_cast<MiniMap>(obj);
			if (miniMap)
			{
				miniMap->SetTarget(this->GetComponent<Transform>());
			}
		}

		AddTag(L"Player");

		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		m_StateMachine->ChangeState(PlayerStartState::Instance());

		m_AnimationStateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		m_AnimationStateMachine->ChangeState(PlayerDefaultAnim::Instance());

		m_AssimililationObject = ObjectFactory::Create<PlayerAssimililationObject>(GetStage());
		m_AssimililationObject->GetComponent<Transform>()->SetParent(GetThis<GameObject>());
		m_AssimililationObject->GetComponent<Transform>()->SetPosition(Vec3(0, 0, 0));
		m_AssimililationObject->GetComponent<Transform>()->SetRotation(Vec3(0,0,0));

		//エフェクトの初期化
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring TestEffectStr = DataDir + L"Effects\\Down_Copirobo.efk";
		auto ShEfkInterface = GetTypeStage<GameStage>()->GetEfkInterface();
		m_DownEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, TestEffectStr);
		
		CopySuccessGauge::InitParam init;
		init.m_Target = trans;
		init.m_PositionDiff = Vec3(0, 1.5f + trans->GetPosition().y, 0);
		m_CopySuccessGauge = GetStage()->AddGameObject<CopySuccessGauge>(init);

		init.m_PositionDiff = Vec3(0, 1.85f + trans->GetPosition().y, 0);
		m_UsageTimeGauge = GetStage()->AddGameObject<UsageTimeGauge>(init);

		m_TargetCursor = GetStage()->AddGameObject<TargetCursor>();
		m_TargetCursor.lock()->SetDrawActive(false);

		AddComponent<Action>();

		m_Egg = GetStage()->AddGameObject<PlayerEgg>();
		m_Egg.lock()->GetComponent<Transform>()->SetPosition(trans->GetPosition()+Vec3(0,-0.75f,0));

		Square::InitParam sqInit;
		sqInit.m_AspectWdivH = 2.0f;
		sqInit.m_Position = trans->GetPosition();
		sqInit.m_Rotate = Vec3(XM_PIDIV2, 0.0f, 0.0f);
		sqInit.m_Scale = Vec3(1.5f, 1.5f, 0);
		sqInit.m_TexKey = L"CIRCLE_SHADOW_TX";
		m_Shadow = GetStage()->AddGameObject<Square>(sqInit);
		m_Shadow.lock()->SetAlphaActive(true);
		m_Shadow.lock()->GetDynamicComponent<SmBaseDraw>()->SetDiffuse(Col4(1, 1, 1, 0.4f));
	}

	//更新
	void Player::OnUpdate()
	{
		float Delta = App::GetApp()->GetElapsedTime();
		m_InputHandler->PushHandle();
		m_InputHandler->StickHandle();
		//ステートマシン更新
		m_StateMachine->Update();

		m_AnimationStateMachine->Update();

		GetDynamicComponent<SmBaseDraw>()->UpdateAnimation(Delta);

		auto trans = GetComponent<Transform>();
		Vec3 shadowPos = trans->GetPosition();
		shadowPos.y = 0.001f;
		m_Shadow.lock()->GetComponent<Transform>()->SetPosition(shadowPos);
	}

	void Player::OnUpdate2()
	{
		float Delta = App::GetApp()->GetElapsedTime();

		m_CopySuccessGauge.lock()->SetGaugeValue(m_CopySuccessGaugeValue);
		m_UsageTimeGauge.lock()->SetGaugeValue(m_CopyAbilityGaugeValue);

		if (GetComponent<CollisionObb>()->GetUpdateActive())
		{
			auto trans = GetComponent<Transform>();
			Vec3 pos = trans->GetPosition();
			pos.y = m_initParam.m_startPos.y;
			trans->SetPosition(pos);
		}
	}

	void Player::OnDraw()
	{
		if(GetComponent<PNTBoneModelDraw>()->GetDiffuse().w > 0)
			AliveGameObject::OnDraw();

		if(m_AssimililationObject->GetDynamicComponent<SmBaseDraw>()->GetDiffuse().w > 0)
			m_AssimililationObject->OnDraw();
	}

	// 移動
	void Player::Move(Vec3 direction)
	{
		auto PtrRedid = AddComponent<Rigidbody>();
		PtrRedid->SetVelocity(direction);

		GetBehavior<UtilBehavior>()->RotToHead(direction, 0.1f);
	}

	vector<shared_ptr<GameObject>> Player::GetNearObjectVec(wstring searchTag)
	{

			vector<shared_ptr<GameObject>> enemies;
			vector<shared_ptr<GameObject>> nearEnemy;
			GetStage()->GetUsedTagObjectVec(searchTag, enemies);
			
		for (auto enemy : enemies )
		{
			Vec3  EnemyPos = enemy->GetComponent<Transform>()->GetPosition();
			Vec3 PlayerPos = GetComponent<Transform>()->GetPosition();
			Vec3 dir = EnemyPos - PlayerPos;
			float dis = dir.length();
			if (dis <= m_BehindDistance)
				nearEnemy.push_back(enemy);
		}

		std::sort(nearEnemy.begin(), nearEnemy.end(), [&](shared_ptr<GameObject> a, shared_ptr<GameObject> b) {
			Vec3 playerPos = GetComponent<Transform>()->GetPosition();
			float aDis = (a->GetComponent<Transform>()->GetPosition() - playerPos).length();
			float bDis = (b->GetComponent<Transform>()->GetPosition() - playerPos).length();
			return aDis < bDis;
		});
		return nearEnemy;
	}

	bool Player::CheckIsBehindEnemy(const shared_ptr<Transform>& enemyTrans)
	{
		auto trans = GetComponent<Transform>();
		Vec3 dir = enemyTrans->GetPosition() - trans->GetPosition();
		float distance = dir.length();
		dir.normalize();

		Vec3 back = -enemyTrans->Forward();
		if (back.angle(dir) < Deg2Rad(90.0f))
			return false;

		return true;
	}

	void Player::ChangeAlpha(float alpha)
	{
		auto drawComp = GetComponent<PNTBoneModelDraw>();
		Col4 color = drawComp->GetDiffuse();
		color.w = alpha;
		drawComp->SetDiffuse(color);
	}

	void Player::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		if (m_StateMachine->GetCurrentState() == PlayerDeadState::Instance())
			return;

		for (auto& obj : OtherVec)
		{
			if (obj->FindTag(L"NormalEnemy"))
			{
				auto enemy = dynamic_pointer_cast<basecross::NormalEnemy>(obj);
				if (!enemy)
					MessageBox(NULL, L"", L"Player::OnCollision\n エネミータグがつけられたNormalEnemyではないオブジェクトです", MB_OK);
				if (enemy->GetStateMachine()->GetCurrentState() == NormalEnemyChasePlayer::Instance())
				{
					m_StateMachine->ChangeState(PlayerDeadState::Instance());
					return;
				}
			}
			else if (obj->FindTag(L"CamereonEnemy"))
			{
				auto enemy = dynamic_pointer_cast<basecross::CamereonEnemy>(obj);
				if (!enemy)
					MessageBox(NULL, L"", L"Player::OnCollision\n エネミータグがつけられたCamereonEnemyではないオブジェクトです", MB_OK);
				if (enemy->GetStateMachine()->GetCurrentState() == CamereonEnemyChasePlayer::Instance())
				{
					m_StateMachine->ChangeState(PlayerDeadState::Instance());
					return;
				}
			}
		}
	}

	shared_ptr<MeshResource> Player::GetMeshResourceForMinimap()
	{
		if (m_MeshResForMinimap)
			return m_MeshResForMinimap;

		Col4 color = Col4(0.9f, 0.9f, 0.9f, 1);

		vector<VertexPositionColor> vertices{
			{ Vec3(-0.75f,0.0f,-1.0f),color},
			{ Vec3(0.0f,0.0f,1.0f),color },
			{ Vec3(0.0f,0.0f,-0.5f),color },
			{ Vec3(0.75f,0.0f,-1.0f),color }
		};

		vector<uint16_t> indices{
			0,1,2,
			2,1,3
		};

		m_MeshResForMinimap = MeshResource::CreateMeshResource(vertices, indices, true);
		return m_MeshResForMinimap;
	}

	float Player::DecreaseCopyValue()
	{
		float delta = App::GetApp()->GetElapsedTime();
		if(m_NowCopyType == CopyType::Pillar || m_NowCopyType == CopyType::CrystalTable)
			m_CopyAbilityGaugeValue -= delta * m_CopyValueDecreaseSpeedStaticObj;
		else if(m_NowCopyType == CopyType::NormalEnemy || m_NowCopyType == CopyType::CamereonEnemy)
			m_CopyAbilityGaugeValue -= delta * m_CopyValueDecreaseSpeedEnemy;

		if (m_CopyAbilityGaugeValue < 0)
			m_CopyAbilityGaugeValue = 0.0f;

		return m_CopyAbilityGaugeValue;
	}

	bool Player::SetCursorToCopyableObject()
	{
		auto copyableObjects = GetNearObjectVec(L"CopyableObj");
		if (copyableObjects.size() <= 0)
		{
			m_TargetCursor.lock()->SetDrawActive(false);
			return false;
		}

		Vec3 smallVec(0.01f, 0.01f, 0.01f);

		for (size_t i = 0; i < copyableObjects.size(); i++)
		{
			if (copyableObjects[i]->FindTag(L"Enemy"))
			{
				if (copyableObjects[i]->GetComponent<Transform>()->GetScale().length() <= smallVec.length())
					continue;

				if (CheckIsBehindEnemy(copyableObjects[i]->GetComponent<Transform>()))
				{
					m_TargetCursor.lock()->SetDrawActive(true);
					m_TargetCursor.lock()->SetTarget(copyableObjects[i]);
					break;
				}
				else
				{
					m_TargetCursor.lock()->SetDrawActive(false);
					continue;
				}
			}
			else if (copyableObjects[i]->FindTag(L"CopyObj"))
			{
				m_TargetCursor.lock()->SetDrawActive(true);
				m_TargetCursor.lock()->SetTarget(copyableObjects[i]);
				break;
			}
		}

		return true;
	}

	void Player::Goal(Vec3 goalPos)
	{
		m_GoalPos = goalPos;
		m_StateMachine->ChangeState(PlayerGoalState::Instance());
	}

	void PlayerEgg::OnCreate()
	{
		Mat4x4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.affineTransformation(
			Vec3(0.5f, 0.5f, 0.5f),//サイズ
			Vec3(0.0f, 0.0f, 0.0f),//原点?
			Vec3(0.0f, 0.0f, 0.0f),//角度
			Vec3(0.0f, 0.0f, 0.0f)//位置
		);

		auto drawComp = AddComponent<PNTBoneModelDraw>();
		drawComp->SetMeshResource(L"EGG_MESH");
		drawComp->SetLineTextureResource(L"EGG_LINE_TX");
		drawComp->AddAnimation(L"DEFAULT", 0, 120,false, 40);
		drawComp->ChangeCurrentAnimation(L"DEFAULT");
		drawComp->SetMeshToTransformMatrix(SpanMat);
		drawComp->SetBlendState(BlendState::AlphaBlend);
		
		SetDrawLayer(1);
		SetAlphaActive(true);
	}

	void PlayerEgg::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		auto drawComp = GetDynamicComponent<SmBaseDraw>();
		drawComp->UpdateAnimation(delta);

		if (drawComp->GetCurrentAnimationTime() >= 120.0f / 40.0f)
		{
			Col4 col = drawComp->GetDiffuse();
			col.w -= delta * 0.5f;
			if (col.w < 0)
			{
				SetDrawActive(false);
				col.w = 0;
			}
			drawComp->SetDiffuse(col);
		}
	}
}

//end basecross

