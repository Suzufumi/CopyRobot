#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Enemy::Enemy(const shared_ptr<Stage>& stagePtr, const shared_ptr<StageCellMap>& cellMap, InitializeParam& initParam)
		: AliveGameObject(stagePtr), m_cellMap(cellMap), m_initParam(initParam), m_nextWayPointNum(1)
	{

	}

	Enemy::Enemy(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
		: AliveGameObject(stagePtr),m_nextWayPointNum(1), m_isAlived(true), 
		m_behaviorSecondCount(0), m_findTime(0)
	{
		// Xml����Player�̏����擾
		LoadXmlParam(pNode, m_initParam.m_startPos, m_initParam.m_startQuat, m_initParam.m_startScale);
		// �p�j�|�C���g��XML�̃f�[�^����
		auto wayPointNodes = XmlDocReader::GetSelectNodes(pNode,L"WayPoint");
		// �p�j�|�C���g����������̂��𒲂ׂ�
		long countNode = XmlDocReader::GetLength(wayPointNodes);
		for (long i = 0; i < countNode; i++)
		{
			// XML��wayPoint�̃f�[�^��1�擾
			auto node = XmlDocReader::GetItem(wayPointNodes, i);
			// wayPoint��XML�̍��W���擾
			auto wayPosStr = XmlDocReader::GetAttribute(node, L"Pos");
			// XML�̍��W��Vec3�ɕϊ�����
			Vec3 wayPos = MyUtil::unityVec3StrToBCVec3(wayPosStr);
			// WayPoints��vector�z��Ɋi�[
			m_wayPoints.push_back(wayPos);
		}

		// CellMap�̃f�[�^���擾
		auto cellMapStr = XmlDocReader::GetAttribute(pNode, L"CellMapName");
		// Stage�ɓo�^����Ă���CellMap�̕������T���ĕϐ��Ɋi�[
		m_cellMap = GetStage()->GetSharedGameObject<StageCellMap>(cellMapStr);
	}

	Enemy::~Enemy()
	{
	}

	void Enemy::OnCreate()
	{
		// Transform�R���|�[�l���g�̎擾
		auto transComp = AddComponent<Transform>();
		// Transform�̐ݒ�
		transComp->SetPosition(m_initParam.m_startPos);
		transComp->SetQuaternion(m_initParam.m_startQuat);
		transComp->SetScale(m_initParam.m_startScale);

		// �^�O�̐ݒ�
		AddTag(L"Enemy");
		AddTag(L"CopyableObj");

		// Rigidbody�R���|�[�l���g�̎擾
		auto rigidComp = AddComponent<Rigidbody>();
		// �Փ˔���
		auto ptrCapsule = AddComponent<CollisionCapsule>();
		// �Œ�Փ˃I�u�W�F�N�g�̐ݒ�(true�ɂ���Ɠ����������ɓ����Ȃ�)
		ptrCapsule->SetFixed(false);
		// �Փ˂����Ƃ���HitAction(None, Auto , Slide, Stop)
		ptrCapsule->SetIsHitAction(IsHitAction::Auto);

		// �I�u�W�F�N�g�̃O���[�v�𓾂�
		auto group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		// �������O���[�v�ɓ����
		group->IntoGroup(GetThis<Enemy>());
		// �����s��������
		auto ptrSep = GetBehavior<SeparationSteering>();
		ptrSep->SetGameObjectGroup(group);

		// �Z���}�b�v�̎擾
		auto mapPtr = m_cellMap.lock();
		// �o�H�T���R���|�[�l���g�̎擾
		auto pathPtr = AddComponent<PathSearch>(mapPtr);

		auto ptrSeek = GetBehavior<SeekSteering>();
		// �d���̐ݒ�
		ptrSeek->SetWeight(3.0f);
		const Vec3 Rotation = Vec3(0.0f, XM_PI, 0.0f);
		const Vec3 Position = Vec3(0.0f, -0.5f, 0.0f);
		// ���f���̍s��ݒ�
		auto spanMat = SetModelMatrix(Vec3(1.0f), Vec3(0.0f), Rotation, Position);

		// �`��R���|�[�l���g�̎擾
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		drawComp->SetMeshToTransformMatrix(spanMat);
		// �X�v���C�g�̐ݒ�
		m_sprite = GetStage()->AddGameObject<Sprite>(L"REDFLAME_TX", Vec2(1280, 800), 0.0f, Vec2(0, 0), 1, 1);
		// �ŏ��͓����ɂ��Ă���
		m_sprite.lock()->SetDiffuse(Col4(1, 1, 1, 0));
		m_sprite.lock()->SetBlendState(Sprite::BlendState::Trace);

		//�G�t�F�N�g�̏�����
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		wstring effectStr = dataDir + L"Effects\\Down_Copirobo.efk";
		auto ShEfkInterface = GetTypeStage<GameStage>()->GetEfkInterface();
		m_downEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, effectStr);

		// �Z�������̍s���̍\�z
		m_cellSearchBehavior.reset(new CellSearchBehavior<Enemy>(GetThis<Enemy>(), m_cellMap));
	}

	void Enemy::OnUpdate()
	{
	}

	void Enemy::OnUpdate2()
	{
		auto ptrUtil = GetBehavior<UtilBehavior>();
		// �i�s�����Ɍ�������
		ptrUtil->RotToHead(0.1f);
	}

	void Enemy::OnDraw()
	{	
		if (GetComponent<PNTBoneModelDraw>()->GetDiffuse().w > 0)
		{
			// �e�N���X��OnDraw���Ă΂Ȃ��ƕ`�悳��Ȃ�
			AliveGameObject::OnDraw();
		}
	}

	void Enemy::UpdateAnimation()
	{
	}

	void Enemy::Stop()
	{
		// ���S�t���O��false�ɐݒ�
		m_isAlived = false;
	}

	void Enemy::OnEvent(const shared_ptr<Event>& event)
	{
		if (event->m_MsgStr == L"Respawn")
		{
			// Enemy�̃��X�|�[��
			RespawnEnemy();
		}
	}

	void Enemy::CreatePostEvent(float delayTime, const wstring keyName)
	{
		// delayTime : ���b�x�点�邩
		// keyName : �C�x���g���ĂԂƂ��̖��O��ݒ�
		PostEvent(delayTime, GetThis<ObjectInterface>(), GetThis<ObjectInterface>(), keyName);
	}

	void Enemy::NextWayPointNumAdd()
	{
		// m_NextWayPointNum��m_WayPoints�̃T�C�Y�𒴂�����
		if (static_cast<unsigned>(m_nextWayPointNum) >= (m_wayPoints.size() - 1))
		{
			// 0�ŏ�����
			m_nextWayPointNum = 0;
		}
		else
		{
			++m_nextWayPointNum;
		}

	}

	void Enemy::CheckWayPointDistance()
	{
		// ������Pos���擾
		auto myPos = GetMyPos();
		// �ړI�n�Ǝ�����Pos�Ƃ̋��������߂�
		auto distance = bsm::length(m_wayPoints[m_nextWayPointNum] - myPos);
		if (distance <= NEXT_WAYPOINT)
		{
			// ���̜p�j�ԍ��ɐݒ�
			NextWayPointNumAdd();
		}
	}

	void Enemy::RespawnEnemy()
	{
		// �ϐ��������̒l�ɖ߂�(�K��1�߂̌o�H�ɍs���悤��)
		m_nextWayPointNum = 1;
		//�G�t�F�N�g�̃v���C
		m_effectPlay = ObjectFactory::Create<EfkPlay>(m_downEffect, Vec3(m_initParam.m_startPos.x, 0.0f, m_initParam.m_startPos.z));
		auto rigidComp = GetComponent<Rigidbody>();
		// velocity���[���ɃZ�b�g����
		rigidComp->SetVelocityZero();
		auto transComp = GetComponent<Transform>();
		// �ŏ��̃|�W�V�����ɐݒ�
		transComp->ResetPosition(m_initParam.m_startPos);
		// �X�P�[����1�ɐݒ肷��
		transComp->SetScale(Vec3(m_initParam.m_startScale));
		// ���X�|�[���Œ��͏Փ˂���Ă������Ȃ��悤�ɂ���
		GetComponent<Collision>()->SetFixed(true);
		// ���S�t���O��true�ɂ���
		SetIsAlived(true);
	}
	// �G���d�Ȃ�Ȃ��悤�ɂ���
	void Enemy::SeparationEnemy()
	{
		// �����s��(�G���m�������悤��)
		auto ptrSep = GetBehavior<SeparationSteering>();
		ptrSep->Execute();
	}

	bool Enemy::CheckFindPlayer()
	{
		// ������Position���擾
		auto myPos = GetMyPos();
		// �v���C���[��Position���擾
		auto playerPos = GetPlayerPos();

		 // �����ƃv���C���[�̋������擾
		float distance = GetPlayerDistance();
		// �v���C���[������Ă�����
		if (distance > PLAYER_DISTANCE)
		{
			return false;
		}

		// �v���C���[���ϐg���Ă��邩�ǂ���
		if (IsPlayerAssimililation())
		{
			return false;
		}

		// Enemy�̎��E���擾
		float deg = GetEnemyEyeSight(playerPos);
		// ���E��45�x�ȏ�Ȃ�
		if (Rad2Deg(deg) > ENEMY_EYESIGHT)
		{
			return false;
		}

		// Enemy��Player�̊Ԃɕǂ����邩�ǂ���
		if (IsWallBetweenPlayerAndEnemy(playerPos, myPos))
		{
			return false;
		}

		return true;
	}

	float Enemy::GetPlayerDistance() const
	{
		// ������Position���擾
		auto myPos = GetMyPos();
		// �v���C���[��Position���擾
		auto playerPos = GetPlayerPos();
		// �����ƃv���C���[�Ƃ̋�����Ԃ�
		return bsm::length(playerPos - myPos);
	}

	float Enemy::GetEnemyEyeSight(Vec3 playerPos) const
	{
		// ���g��Position���擾
		auto myPos = GetMyPos();
		// ���E�ɓ����Ă邩
		auto dir = playerPos - myPos;
		// ���K��
		dir.normalize();
		// �O���̃x�N�g�����擾
		auto forward = GetComponent<Transform>()->Forward();
		// ���ς��擾
		float dot = bsm::dot(dir, forward);
		auto deg = acos(dot);

		return deg;
	}

	bool Enemy::IsPlayerAssimililation()
	{
		// stage�̃I�u�W�F�N�g���擾
		auto stageObj = GetStage()->GetGameObjectVec();
		for (auto& obj : stageObj)
		{
			// Player�̃I�u�W�F�N�g���擾
			auto player = dynamic_pointer_cast<Player>(obj);
			if (player)
			{
				auto rigidComp = player->GetComponent<Rigidbody>();
				// velocity�̎擾
				auto velocity = rigidComp->GetVelocity();
				// velocity�̒������擾
				auto dir = velocity.length();
				// Player�̌��݂̃X�e�[�g���擾
				auto stateMachineType = player->GetStateMachine()->GetCurrentState();
				// Player�̌��݂̏�Ԃ��擾
				auto nowCopyType = player->GetPlayerCopytype();
				switch (nowCopyType)
				{
				case Player::CopyType::Normal:
					break;
				case Player::CopyType::NormalEnemy:
				case Player::CopyType::CamereonEnemy:
					// Player���ϐg���܂��͕ϐg�r�����ǂ���
					// �����Ă��邩
					if (stateMachineType == PlayerCamereonTransform::Instance() ||
						stateMachineType == PlayerCamereonAssimililation::Instance() &&
						dir >= PLAYER_VELOCITY_LENGTH)
					{
						return true;
					}
					break;
				case Player::CopyType::Pillar:
				case Player::CopyType::CrystalTable:
					// Player���ϐg���܂��͕ϐg�r�����ǂ���
					// �~�܂��Ă��邩
					if (stateMachineType == PlayerCamereonTransform::Instance() ||
						stateMachineType == PlayerCamereonAssimililation::Instance() &&
						dir <= PLAYER_VELOCITY_LENGTH)
					{
						return true;
					}
					break;
				}
			}
		}
		return false;
	}		

	bool Enemy::IsWallBetweenPlayerAndEnemy(Vec3 playerPos, Vec3 enemyPos)
	{
		// wall�̃I�u�W�F�N�g��ێ�����ϐ�
		vector<shared_ptr<GameObject>> wallVec;
		// Wall�̃^�O���t���Ă���I�u�W�F�N�g���擾
		GetStage()->GetUsedTagObjectVec(L"Wall", wallVec);
		for (auto& wallObj : wallVec)
		{
			// wall�̃I�u�W�F�N�g���擾
			auto wall = dynamic_pointer_cast<Wall>(wallObj);
			if (wall)
			{
				auto wallCol = wall->GetComponent<CollisionObb>();
				// wall��Obb���擾
				auto wallObb = wallCol->GetObb();
				// �����������ĕǂ����邩�ǂ������ׂ�
				if (HitTest::SEGMENT_OBB(enemyPos, playerPos, wallObb))
				{
					return true;
				}
			}
		}

		return false;
	}

	Vec3 Enemy::GetPlayerPos() const
	{
		// stage�̃I�u�W�F�N�g���擾
		auto stageObj = GetStage()->GetGameObjectVec();
		for (auto obj : stageObj)
		{
			// Player�̃^�O���t���Ă���I�u�W�F�N�g��T��
			if (obj->FindTag(L"Player"))
			{
				// Player��Position���擾
				auto playerPos = obj->GetComponent<Transform>()->GetPosition();
				return playerPos;
			}
		}
		return Vec3(0.0f, 0.0f, 0.0f);
	}

	shared_ptr<ObjState<Player>> Enemy::GetPlayerCurrentStateMachine()
	{
		shared_ptr<ObjState<Player>> currentState = NULL;
		// stage�̃I�u�W�F�N�g���擾
		auto stageObj = GetStage()->GetGameObjectVec();
		for (auto& obj : stageObj)
		{
			auto player = dynamic_pointer_cast<Player>(obj);
			if (player)
			{
				// Player�̌��݂̃X�e�[�g�}�V�[�����擾
				currentState = player->GetStateMachine()->GetCurrentState();
			}
		}
		return currentState;
	}

	Player::CopyType Enemy::GetPlayerCopyType() const
	{
		// stage�̃I�u�W�F�N�g���擾
		auto stageObj = GetStage()->GetGameObjectVec();
		for (auto& obj : stageObj)
		{
			auto player = dynamic_pointer_cast<Player>(obj);
			if (player)
			{
				auto playerCopyType = player->GetPlayerCopytype();
				return playerCopyType;
			}
		}

		return Player::CopyType::Normal;
	}

	shared_ptr<MeshResource> Enemy::GetMeshResourceForMinimap()
	{
		if (m_meshResForMinimap)
			return m_meshResForMinimap;

		Col4 color = Col4(0.8f, 0.1f, 0.1f, 1);

		vector<VertexPositionColor> vertices{
			{ Vec3(-0.75f,0.0f,-1.0f),color },
			{ Vec3(0.0f,0.0f,1.0f),color },
			{ Vec3(0.0f,0.0f,-0.5f),color },
			{ Vec3(0.75f,0.0f,-1.0f),color }
		};

		vector<uint16_t> indices{
			0,1,2,
			2,1,3
		};

		m_meshResForMinimap = MeshResource::CreateMeshResource(vertices, indices, true);
		return m_meshResForMinimap;
	}

}
// end basecross