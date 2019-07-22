/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------



	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight()
	{
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrTpsCamera = ObjectFactory::Create<TpsCamera>();
		PtrView->SetCamera(PtrTpsCamera);
		//�V���O�����C�g�̍쐬
		auto PtrSingleLight = CreateLight<SingleLight>();
		//���C�g�̐ݒ�
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	void GameStage::CreatePlayer()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Player>(L"Player");
		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/Player");
	}

	void GameStage::CreateEnemy()
	{
		auto group = GetSharedObjectGroup(L"EnemyGroup", false);
		if (group)
		{
			group->AllClear();
		}
		else
		{
			CreateSharedObjectGroup(L"EnemyGroup");
		}


		GameObjecttXMLBuilder builder;
		builder.Register<NormalEnemy>(L"NormalEnemy");
		builder.Register<CamereonEnemy>(L"CamereonEnemy");
		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/NormalEnemy");
	}

	void GameStage::CreatePlane()
	{
		Quat q;
		q.rotationX(XM_PIDIV2);
		// Transform�̐ݒ�
		StaticObject::InitializeParam status = {
			{ 0.0f, 0.0f, 0.0f },		// Position
					q           ,       // Quaternion
			{ 100.0f, 100.0f, 1.0f }		// Scale
		};
		Plane::InitializeParam planeParam = {
			L"STAGE_TX"
		};

		AddGameObject<Plane>(status,planeParam);
	}

	void GameStage::CreateWall()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Wall>(L"Wall");

		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/Wall");

		SetCellMapCost<Wall>(L"Wall");
	}
	///------------------------------------------------
	// �Z���}�b�v�̍쐬
	///------------------------------------------------
	void GameStage::CreateStageCellMap()
	{
		// CellMap�̃O���[�v���쐬
		auto group = CreateSharedObjectGroup(L"CellMap");
		// ���݂̃X�e�[�W��Xml�t�@�C���ԍ����擾
		wstring stageXmlPath = m_StageXmlPath;
		// Xml�t�@�C���̓ǂݍ���
		XmlDocReader xmlReader(stageXmlPath);
		// Xml�t�@�C������GameStage/CellMap��T��
		auto cellMapData = xmlReader.GetSelectNodes(L"GameStage/CellMap");
		// CellMap�̐����J�E���g
		long cellMapCount = XmlDocReader::GetLength(cellMapData);
		// CellMap�̐��������[�v
		for (long i = 0; i < cellMapCount; i++)
		{
			auto node = XmlDocReader::GetItem(cellMapData, i);
			// XML����PieceSize���擾(wstring)
			auto pieceSizeStr = XmlDocReader::GetAttribute(node, L"PieceSize");
			// wstring��float�̒l�ɕϊ�
			float pieceSize = MyUtil::wstrToFloat(pieceSizeStr);
			// XML����Position���擾(wstring)
			auto positionStr = XmlDocReader::GetAttribute(node, L"Pos");
			// wstring��Vec3�ɕϊ�
			Vec3 pos = MyUtil::unityVec3StrToBCVec3(positionStr);
			// XML����Row���擾
			auto rowStr = XmlDocReader::GetAttribute(node, L"Row");
			// wstring��int�ɕϊ�
			int row = MyUtil::wstrToInt(rowStr);
			// XML����Col���擾
			auto colStr = XmlDocReader::GetAttribute(node, L"Col");
			// wstring��int�ɕϊ�
			int col = MyUtil::wstrToInt(colStr);
			// StageCellMap���X�e�[�W�ɔz�u����
			auto stageCellMap = AddGameObject<StageCellMap>(pos, pieceSize, col, row);
			// �m�[�h�̖��O���擾
			auto nameStr = XmlDocReader::GetAttribute(node, L"Name");
			// �^�O�ǉ�
			stageCellMap->AddTag(nameStr);
			stageCellMap->AddTag(L"CellMap");
			SetSharedGameObject(nameStr, stageCellMap);
			// �O���[�v�ɒǉ�
			group->IntoGroup(stageCellMap);
		}
	}

	void GameStage::CreateRock()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Pillar>(L"Rock");
		builder.Register<CrystalTable>(L"CrystalTable");

		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/Rock");
		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/CrystalTable");

		SetCellMapCost<Pillar>(L"Pillar");
		SetCellMapCost<CrystalTable>(L"CrystalTable");
	}

	void GameStage::CreateTutorialBox()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<TutorialBox>(L"TutorialBox");

		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/TutorialBox");
	}
	
	template<typename T>
	void GameStage::SetCellMapCost(wstring TagName)
	{
		// CellMap�Ƃ������O�̃O���[�v���擾����
		auto group = GetSharedObjectGroup(L"CellMap");
		// �O���[�v�̒��̃Z���}�b�v�̐��������[�v����
		for (auto& cellMap : group->GetGroupVector())
		{
			// CellMap�̃|�C���^�擾
			auto mapPtr = dynamic_pointer_cast<StageCellMap>(cellMap.lock());
			if (mapPtr)
			{
				// �Z���}�b�v����Z���̔z����擾
				auto& cellVec = mapPtr->GetCellVec();

				vector<shared_ptr<GameObject>> ObjectVec;
				// �^�O���t���Ă���I�u�W�F�N�g���擾����vector�ɒǉ�
				GetUsedTagObjectVec(TagName, ObjectVec);
				vector<AABB> objectsAABBVec;
				for (auto& Obj : ObjectVec)
				{
					// �����Ŏw�肳�ꂽ�I�u�W�F�N�g���擾
					auto ObjPtr = dynamic_pointer_cast<T>(Obj);
					if (ObjPtr)
					{
						auto colPtr = ObjPtr->GetComponent<CollisionObb>();
						// �{�b�N�X�̏Փ˔��肩���b�s���O����AABB���擾���ĕۑ�
						AABB pushAABB = colPtr->GetWrappingAABB();
						pushAABB.m_Min.x += 0.1f;
						pushAABB.m_Min.z += 0.1f;
						pushAABB.m_Max.x -= 0.1f;
						pushAABB.m_Max.z -= 0.1f;
						objectsAABBVec.push_back(pushAABB);
					}
				}
				//�Z���z�񂩂�Z�����X�L����
				for (auto& row : cellVec) 
				{
					for (auto& col : row) 
					{
						for (auto& vObj : objectsAABBVec) 
						{
							//�Z���ɏ�Q�����z�u����Ă��邩
							if (HitTest::AABB_AABB_NOT_EQUAL(col.m_PieceRange, vObj)) 
							{
								// ��Q����-1�Őݒ肷��
								col.m_Cost = -1;
								break;
							}
						}
					}
				}
			}
		}
	}

	void GameStage::CreateGoal()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Goal>(L"Goal");

		builder.Build(GetThis<Stage>(), m_StageXmlPath, L"GameStage/Goal");
		SetCellMapCost<Goal>(L"Goal");
	}

	void GameStage::OnCreate()
	{
		try 
		{
			//�V�[���̎擾
			auto PtrScene = App::GetApp()->GetScene<Scene>();
			wstring dataDir;
			App::GetApp()->GetDataDirectory(dataDir);
			switch (PtrScene->GetStageNum())
			{
			case 0:
				m_StageXmlPath = dataDir + L"Stage\\1.xml";
				break;
			case 1:
				m_StageXmlPath = dataDir + L"Stage\\2.xml";
				break;
			case 2:
				m_StageXmlPath = dataDir + L"Stage\\3.xml";
				break;
			case 3:
				m_StageXmlPath = dataDir + L"Stage\\4.xml";
				break;
			case 4:
				m_StageXmlPath = dataDir + L"Stage\\5.xml";
				break;
			case 5:
				m_StageXmlPath = dataDir + L"Stage\\6.xml";
				break;
			case 6:
				m_StageXmlPath = dataDir + L"Stage\\7.xml";
				break;
			case 7:
				m_StageXmlPath = dataDir + L"Stage\\8.xml";
				break;
			default:
				m_StageXmlPath = dataDir + L"Stage\\2.xml";
				break;
			}
			//m_StageXmlPath = dataDir + L"Stage\\test.xml";

			//�G�t�F�N�g�̒ǉ�
			m_EfkInterface = ObjectFactory::Create<EfkInterface>();

			auto miniMap = AddGameObject<MiniMap>();
			miniMap->SetDrawActive(false);

			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			// �Z���}�b�v�̍쐬
			CreateStageCellMap();
			// �v���C���[�̍쐬
			CreatePlayer();
			// Enemy�̍쐬
			CreateEnemy();
			// ���̍쐬
			CreatePlane();
			//�ǂ̍쐬
			CreateWall();
			//��̍쐬
			CreateRock();
			//�S�[���̍쐬
			CreateGoal();

			CreateTutorialBox();

			auto skySphere = AddGameObject<SkySphere>();
		}
		catch (...) 
		{
			throw;
		}
	}

	void GameStage::OnUpdate()
	{
		Stage::OnUpdate();

		auto key = App::GetApp()->GetInputDevice().GetKeyState();

		if (key.m_bPressedKeyTbl['T'])
		{
			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn,Col4(0,0,0,1));
			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut, Col4(0, 0, 0, 1));
			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransition(transIn, transOut);
		}
		else if (key.m_bPressedKeyTbl['Y'])
		{
			auto transIn = ObjectFactory::Create<FadeMaskTransition>(3.0f,L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
			auto transOut = ObjectFactory::Create<FadeMaskTransition>(3.0f,L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransition(transIn, transOut);
		}
		
		m_EfkInterface->OnUpdate();
	}
	
	void GameStage::OnDraw()
	{

	}

	void GameStage::DrawStage()
	{
		//���C���[�̎擾�Ɛݒ�
		set<int> DrawLayers;
		//Sprite�������łȂ����𕪗�
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsDrawActive()) {
				//�`�惌�C���[�ɓo�^
				DrawLayers.insert(ptr->GetDrawLayer());
				//Sprite�����̔h���N���X�Ȃ番��

				auto sprite = dynamic_pointer_cast<Sprite>(ptr);

				if (ptr->GetDynamicComponent<SpriteBaseDraw>(false) || ptr->IsSpriteDraw() || sprite) {
					pImpl->m_SpriteVec.push_back(ptr);
				}
				else {
					pImpl->m_Object3DVec.push_back(ptr);
				}
			}
		}
		//3D�̓����Ɣ񓧖��𕪗�
		for (auto ptr : pImpl->m_Object3DVec) {
			if (ptr->IsDrawActive()) {
				if (ptr->IsAlphaActive()) {
					pImpl->m_Object3DAlphaVec.push_back(ptr);
				}
				else {
					pImpl->m_Object3DNormalVec.push_back(ptr);
				}
			}
		}

		auto PtrCamera = pImpl->m_ViewBase->GetTargetCamera();
		//�J�����̈ʒu
		bsm::Vec3 CameraEye = PtrCamera->GetEye();
		//������3D�I�u�W�F�N�g���J��������̋����Ń\�[�g
		//�ȉ��́A�I�u�W�F�N�g�������Ɏ��bool��Ԃ������_��
		//--------------------------------------------------------
		auto func = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool {
			auto PtrLeftTrans = Left->GetComponent<Transform>();
			auto PtrRightTrans = Right->GetComponent<Transform>();

			auto LeftPos = PtrLeftTrans->GetWorldMatrix().transInMatrix();
			auto RightPos = PtrRightTrans->GetWorldMatrix().transInMatrix();

			auto LeftLen = bsm::length(LeftPos - CameraEye);
			auto RightLen = bsm::length(RightPos - CameraEye);

			return (LeftLen > RightLen);
		};

		std::sort(pImpl->m_Object3DAlphaVec.begin(), pImpl->m_Object3DAlphaVec.end(), func);

		//3D�m�[�}���I�u�W�F�N�g�̕`�揀��
		for (auto ptr : pImpl->m_Object3DNormalVec) {
			ptr->OnPreDraw();
		}
		//3D�����I�u�W�F�N�g�̕`�揀��
		for (auto ptr : pImpl->m_Object3DAlphaVec) {
			ptr->OnPreDraw();
		}
		//�p�[�e�B�N���̕`�揀���i�����j
		GetParticleManager(false)->OnPreDraw();
		//�p�[�e�B�N���̕`�揀���i���Z�j
		GetParticleManager(true)->OnPreDraw();
		//�X�v���C�g�I�u�W�F�N�g�̕`�揀��
		for (auto ptr : pImpl->m_SpriteVec) {
			ptr->OnPreDraw();
		}


		//--------------------------------------------------------
		//�X�v���C�g��Z���W�����Ń\�[�g
		//�ȉ��́A�I�u�W�F�N�g�������Ɏ��bool��Ԃ������_��
		//--------------------------------------------------------
		auto funcSprite = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool {
			auto PtrLeftTrans = Left->GetComponent<Transform>();
			auto PtrRightTrans = Right->GetComponent<Transform>();

			auto LeftPos = PtrLeftTrans->GetWorldMatrix().transInMatrix();
			auto RightPos = PtrRightTrans->GetWorldMatrix().transInMatrix();

			float LeftZ = LeftPos.z;
			float RightZ = RightPos.z;
			return (LeftZ > RightZ);
		};
		std::sort(pImpl->m_SpriteVec.begin(), pImpl->m_SpriteVec.end(), funcSprite);

		bool isOnece = false;
		for (auto it = DrawLayers.begin(); it != DrawLayers.end(); it++) {
			int Tgt = *it;
			//3D�m�[�}���I�u�W�F�N�g�̕`��
			for (auto ptr : pImpl->m_Object3DNormalVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
			//3D�����I�u�W�F�N�g�̕`��
			for (auto ptr : pImpl->m_Object3DAlphaVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}

			if (!isOnece)
			{
				auto& camera = GetView()->GetTargetCamera();
				m_EfkInterface->SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());
				m_EfkInterface->OnDraw();

				isOnece = true;
			}

			//�p�[�e�B�N���̕`��
			//�p�[�e�B�N���}�l�[�W���͕`�惌�C���[���Ƃɏ����������̂�
			//�����C���[�`�悷��
			//��������
			GetParticleManager(false)->OnDraw();
			//���Z����
			GetParticleManager(true)->OnDraw();
			//�X�v���C�g�I�u�W�F�N�g�̕`��
			for (auto ptr : pImpl->m_SpriteVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
		}
		//�X�e�[�W��Draw();
		OnDraw();
		//���[�N�p�z��̃N���A
		//���[�N�z��͖��^�[�����Ƃɏ���������邪�A
		//�ő�l�͌���Ȃ��̂�2��ڂ̃^�[���ȍ~�͍����ɓ��삷��
		pImpl->m_Object3DVec.clear();
		pImpl->m_SpriteVec.clear();
		pImpl->m_Object3DNormalVec.clear();
		pImpl->m_Object3DAlphaVec.clear();
	}

	void GameStage::RenderStage()
	{
		vector<shared_ptr<GameObject>> objectVec;
		GetUsedTagObjectVec(L"MiniMap", objectVec);
		for (auto miniMap : objectVec)
		{
			auto lock = dynamic_pointer_cast<MiniMap>(miniMap);
			if (lock)
				lock->MiniMapDraw();
		}

		Stage::RenderStage();
	}

	void GameStage::OnEvent(const shared_ptr<Event>& event)
	{
		Stage::OnEvent(event);

		if (event->m_MsgStr == L"CreatePillar")
		{
			auto pillarParam = static_pointer_cast<pair<StaticObject::InitializeParam, Pillar::InitializeParam>>(event->m_Info);
			AddGameObject<Pillar>(pillarParam->first, pillarParam->second);
		}
		else if (event->m_MsgStr == L"CreateCrystalTable")
		{
			auto tableParam = static_pointer_cast<StaticObject::InitializeParam>(event->m_Info);
			AddGameObject<CrystalTable>(*tableParam.get());
		}
	}

	void GameStage::StopAllEnemies()
	{
		vector<shared_ptr<GameObject>> enemies;
		GetUsedTagObjectVec(L"Enemy", enemies);

		for (auto enemy : enemies)
		{
			enemy->SetUpdateActive(false);
		}
	}

	void GameStage::ActiveAllEnemies()
	{
		vector<shared_ptr<GameObject>> enemies;
		GetUsedTagObjectVec(L"Enemy", enemies);

		for (auto enemy : enemies)
		{
			enemy->SetUpdateActive(true);
		}
	}

	bool GameStage::IsEnemyFindPlayer()
	{
		auto enemies = GetSharedObjectGroup(L"EnemyGroup", false);
		bool isFind = false;;
		for (auto enemy : enemies->GetGroupVector())
		{
			auto normal = dynamic_pointer_cast<NormalEnemy>(enemy.lock());
			auto camereon = dynamic_pointer_cast<CamereonEnemy>(enemy.lock());
			if (normal)
			{
				if (normal->GetStateMachine()->GetCurrentState() == NormalEnemyChasePlayer::Instance())
				{
					isFind = true;
					break;
				}
			}
			else if (camereon)
			{
				if (camereon->GetStateMachine()->GetCurrentState() == CamereonEnemyChasePlayer::Instance())
				{
					isFind = true;
					break;
				}
			}
		}

		return isFind;
	}
}
//end basecross
