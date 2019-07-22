#pragma once
#include "stdafx.h"

namespace basecross
{
	class Enemy : public AliveGameObject
	{
	public:
		enum EnemyState
		{
			None,
			Normal,
			Chase,
			Lost,
			Copied
		};
		struct InitializeParam
		{
			Vec3 m_startPos;
			Quat m_startQuat;
			Vec3 m_startScale;
		};
	private:
		weak_ptr<Sprite> m_sprite;
		// �G�t�F�N�g
		shared_ptr<EfkEffect> m_downEffect;
		// �G�t�F�N�g�̎��s�I�u�W�F�N�g
		shared_ptr<EfkPlay> m_effectPlay;
		// ���̜p�j�|�C���g�̂Ƃ̋���
		const float NEXT_WAYPOINT = 1.0f;
		// �v���C���[�Ƃ̋���
		const float PLAYER_DISTANCE = 10.0f;
		// �v���C���[��velocit�̒���
		const float PLAYER_VELOCITY_LENGTH = 3.0f;
		// �G�̎��E
		const float ENEMY_EYESIGHT = 50.0f;
		// �v���C���[�������鎞��
		const unsigned int FIND_PLAYER_TIME = 60;
		// �ǂ������鎞��
		const unsigned int CHASE_TIME = 200;
		// �������Ă���p�j�s���ɖ߂鎞��
		const unsigned int  LOST_PLAYER_TIME = 60;
		// �G�̎��E�̌��E
		const float EYESIGHT_LIMIT = 80.0f;
		// ���X�|�[�����Ă���p�j�s���ɖ߂鎞��
		const unsigned int RETUNRN_WANDER_TIME = 720;

	protected:
		// �������ϐ�
		InitializeParam m_initParam;
		// ���S�t���O
		bool m_isAlived;
		// �Z���}�b�v��ێ�����
		weak_ptr<StageCellMap> m_cellMap;
		// ���̜p�j�|�C���g�̔ԍ���ێ�����
		int m_nextWayPointNum;
		// �p�j�|�C���g
		vector<Vec3> m_wayPoints;
		// �Z�������̍s���N���X
		unique_ptr<CellSearchBehavior<Enemy>> m_cellSearchBehavior;
		EnemyState m_enemyState = EnemyState::None;
		// �~�j�}�b�v�ɕ`�悷��Mesh��ێ�����
		shared_ptr<MeshResource> m_meshResForMinimap;
		// �s���̕b���Ǘ�
		unsigned int m_behaviorSecondCount;
		// �v���C���[�����߂Ă���b�����J�E���g
		unsigned int m_findTime;
	public:
		Enemy(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		Enemy(const shared_ptr<Stage>& stagePtr, const shared_ptr<StageCellMap>& cellMap, InitializeParam& initParam);

		virtual ~Enemy();
		// �\�z
		virtual void OnCreate()override;
		// �X�V
		virtual void OnUpdate()override;
		// ��X�V
		virtual void OnUpdate2()override;
		// �`��
		virtual void OnDraw()override;
		// �A�j���[�V�����̍X�V
		virtual void UpdateAnimation();

		// �������~�߂�
		virtual void Stop() override;
		// PostEvnet���擾����
		virtual void OnEvent(const shared_ptr<Event>& event) override;
		// PostEvent��ݒ肷��
		void CreatePostEvent(float delayTime, const wstring keyName);
		// �v���C���[��������
		bool CheckFindPlayer();
		// ���̜p�j�|�C���g�̔ԍ���ǉ�
		void NextWayPointNumAdd();
		// ���݂̜p�j�|�C���g�Ƃ̋���
		void CheckWayPointDistance();
		// �G�l�~�[�����X�|�[��������
		void RespawnEnemy();
		// �G���m���d�Ȃ�Ȃ��悤�ɂ���
		void SeparationEnemy();
		// �v���C���[�Ƃ̋������擾
		float GetPlayerDistance() const;
		// �G�l�~�[�̎��E���擾
		float GetEnemyEyeSight(Vec3 playerPos) const;
		// �v���C���[�̍��W���擾
		Vec3 GetPlayerPos() const;
		// �����̍��W���擾
		inline Vec3 GetMyPos() const
		{
			auto myPos = this->GetComponent<Transform>()->GetPosition();
			return myPos;
		}
		// �p�j�|�C���g�̍��W�̎擾
		inline Vec3 GetWayPoint() const { return m_wayPoints[m_nextWayPointNum]; }
		// ���g�������Ă��邩�ǂ������擾
		inline bool GetIsAlived() const { return m_isAlived; }
		inline void SetIsAlived(bool isAlived) { m_isAlived = isAlived; }
		// �v���C���[���ϐg�r�����ϐg�����ǂ������f����
		bool IsPlayerAssimililation();
		// �v���C���[�Ƃ̊Ԃɕǂ����邩�ǂ���
		bool IsWallBetweenPlayerAndEnemy(Vec3 playerPos, Vec3 enemyPos);
		// �X�e�[�g���擾
		EnemyState GetEnemyState() { return m_enemyState; }
		// �X�e�[�g��ݒ�
		inline void SetEnemyState(EnemyState state) { m_enemyState = state; }
		// �Z�������̍s���𓾂�
		inline unique_ptr<CellSearchBehavior<Enemy>>& GetCellSearchBehavior()
		{ return m_cellSearchBehavior; }
		// �X�v���C�g�̎擾
		inline weak_ptr<Sprite> GetRedLineSprite()
		{ return m_sprite; }
		// �v���C���[�̃X�e�[�g���擾
		Player::CopyType GetPlayerCopyType() const;
		
		// �v���C���[�̌��݂̃X�e�[�g�擾
		shared_ptr<ObjState<Player>> GetPlayerCurrentStateMachine();

		virtual shared_ptr<MeshResource> GetMeshResourceForMinimap();

		inline Vec3 GetStartPosition() { return m_initParam.m_startPos; }
		// �v���C���[�𔭌����鎞�Ԃ��擾
		inline unsigned int GetFindPlayerTime() const
		{ return FIND_PLAYER_TIME;}
		// �ǂ������鎞�Ԃ��擾
		inline unsigned int GetChaseTime() const
		{ return CHASE_TIME;}
		// �v���C���[���������Ă���p�j�ɖ߂鎞�Ԃ��擾
		inline unsigned int GetLostPlayerTime() const
		{ return LOST_PLAYER_TIME;}
		// �G�̎��E���E���擾
		inline float GetEyesightLimit() const
		{ return EYESIGHT_LIMIT;}
		// ���X�|�[����ɜp�j�ɖ߂鎞�Ԃ��擾
		inline unsigned int GetReturnWanderTime() const
		{ return RETUNRN_WANDER_TIME;}
		// ���݂̕b�����擾
		inline unsigned int GetBehaviorConut() const
		{ return m_behaviorSecondCount;}
		// �b���̐ݒ�
		inline void SetBehaviorCount(unsigned int countTime)
		{ m_behaviorSecondCount = countTime;}
		// ���݂̌��߂��b���̎擾
		inline unsigned int GetFindTime() const
		{ return m_findTime;}
		// ���߂��b���̐ݒ�
		inline void SetFindTime(unsigned int findTime)
		{ m_findTime = findTime;}
	};

}
// end basecross