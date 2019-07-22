#pragma once
#include "stdafx.h"

namespace basecross
{
	//-----------------------------------------------------------------------------
	/// �Z���������g�����s���N���X
	//-----------------------------------------------------------------------------
	template<typename T>
	class CellSearchBehavior : public ObjBehavior<T>
	{
	private:
		// �p�j�|�C���g�̋���
		const float WANDER_DISTANCE = 50.0f;
		// �p�j�X�s�[�h(����)
		const float SLOWING_SPEED = 0.83f;
		// �v���C���[��ǂ�������X�s�[�h(����)
		const float CHASE_SPEED = 0.93f;
		// �Z���}�b�v�̃|�C���^
		weak_ptr<StageCellMap> m_cellMap;
		// �o�H�̏����i�[
		vector<CellIndex> m_cellPath;
		//���݂̎����̃Z���C���f�b�N�X
		int m_cellIndex = -1;
		//�߂����i���́j�̃Z���C���f�b�N�X
		int m_nextCellIndex = -1;
		//�^�[�Q�b�g�̃Z���C���f�b�N�X
		int m_targetCellIndex = -1;
	public:
		CellSearchBehavior(const shared_ptr<T>& GameObjectPtr, const weak_ptr<StageCellMap>& CellMap)
			:ObjBehavior(GameObjectPtr), m_cellMap(CellMap)
		{}
		virtual ~CellSearchBehavior()
		{}

		// �ړI�̏ꏊ���T�[�`����
		bool SearchNextPoint(const Vec3& TargetPos, shared_ptr<StageCellMap>& mapPtr)
		{
			// �Z���}�b�v�̃|�C���^���擾�ł��Ă�����
			if (mapPtr)
			{
				// �o�H��������
				SearchCellPath(TargetPos);
				return true;
			}
			else
			{
				// ���s����
				m_cellIndex = -1;
				m_nextCellIndex = -1;
				m_targetCellIndex = -1;
			}
			return false;
		}
		// �o�H��������
		void SearchCellPath(const Vec3& TargetPos)
		{
			auto pathPtr = GetGameObject()->GetComponent<PathSearch>();
			// �ړI�|�C���g�̎擾
			Vec3 targetPos = TargetPos;
			// �o�H�̏����N���A
			m_cellPath.clear();
			// �p�X�{����������
			if (pathPtr->SearchCell(targetPos, m_cellPath))
			{
				// ��������������
				m_cellIndex = 0;
				m_targetCellIndex = m_cellPath.size() - 1;
				if (m_cellIndex == m_targetCellIndex)
				{
					// ���łɓ����Z���ɂ���
					m_nextCellIndex = m_cellIndex;
				}
				else
				{
					// ����Ă���
					m_nextCellIndex = m_cellIndex + 1;
				}
			}
		}
		// ���̖ړI�n�ֈړ�����
		void GoToNextWayPoint(const Vec3& TargetPos, shared_ptr<StageCellMap>& mapPtr)
		{
			// �����̃|�W�V�������擾
			auto myPos = GetGameObject()->GetComponent<Transform>()->GetPosition();
			if (mapPtr)
			{
				// �傫�߂ɐݒ肵�Ă����ċ}�ɃX�s�[�h�������Ȃ�Ȃ��悤�ɐݒ�
				if (length(myPos - TargetPos) <= WANDER_DISTANCE)
				{
					auto rigidComp = GetGameObject()->GetComponent<Rigidbody>();
					auto velocity = rigidComp->GetVelocity();
					// ��������
					velocity *= SLOWING_SPEED;
					rigidComp->SetVelocity(velocity);
				}
				auto ptrSeek = GetGameObject()->GetBehavior<SeekSteering>();
				AABB ret;
				mapPtr->FindAABB(m_cellPath[m_nextCellIndex], ret);
				auto pos = ret.GetCenter();
				pos.y = TargetPos.y;
				ptrSeek->Execute(pos);
			}

		}
		// �p�j�s��������
		void WanderingBehavior(const Vec3& TargetPos)
		{
			auto myPos = GetGameObject()->GetComponent<Transform>()->GetPosition();
			// �Z���}�b�v�f�[�^�̓ǂݍ���
			auto map = m_cellMap.lock();
			if (map)
			{
				// ���̜p�j�|�C���g��T��
				if (SearchNextPoint(TargetPos, map))
				{
					GoToNextWayPoint(TargetPos, map);
				}
				else
				{
					auto ptrSeek = GetGameObject()->GetBehavior<SeekSteering>();
					CellIndex targetCell;
					if (map->FindCell(TargetPos, targetCell))
					{
						AABB ret;
						map->FindAABB(targetCell, ret);
						auto pos = ret.GetCenter();
						pos.y = TargetPos.y;
						ptrSeek->Execute(pos);
					}
				}
			}
		}
		// �v���C���[��ǐՂ���
		void ChasePlayer(const Vec3& PlayerPos)
		{
			auto myPos = GetGameObject()->GetComponent<Transform>()->GetPosition();
			auto map = m_cellMap.lock();
			if (map)
			{
				if (SearchNextPoint(PlayerPos, map))
				{
					HeadToPlayer(PlayerPos, myPos, map);
				}
			}
		}

		// �v���C���[�Ɍ�����
		void HeadToPlayer(const Vec3& PlayerPos, const Vec3& EnemyPos, shared_ptr<StageCellMap>& mapPtr)
		{
			// �ǂ������鋗��
			const float CHASEDISTANCE = 15.0f;
			auto ptrSeek = GetGameObject()->GetBehavior<SeekSteering>();
			// �ړI�n�ɒ�������
			if (m_nextCellIndex == 0) {
				// ����
				auto ptrRigid = GetGameObject()->GetComponent<Rigidbody>();
				auto velocity = ptrRigid->GetVelocity();
				velocity *= CHASE_SPEED;
				ptrRigid->SetVelocity(velocity);
				ptrSeek->Execute(PlayerPos);
			}
			else {
				// Enemy����Player�̋������擾����
				auto distance = length(EnemyPos - PlayerPos);
				// ������15.0f�ȉ���������
				if (distance <= CHASEDISTANCE) {
					// ����
					auto ptrRigid = GetGameObject()->GetComponent<Rigidbody>();
					auto velocity = ptrRigid->GetVelocity();
					velocity *= CHASE_SPEED;
					ptrRigid->SetVelocity(velocity);
				}
				AABB ret;
				mapPtr->FindAABB(m_cellPath[m_nextCellIndex], ret);
				auto pos = ret.GetCenter();
				pos.y = PlayerPos.y;
				ptrSeek->Execute(Vec3(pos.x, 0.5f, pos.z));
			}
		}
	};
}