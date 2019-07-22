#pragma once
#include "stdafx.h"

namespace basecross
{
	class TpsCamera : public Camera
	{
		friend class CameraDefaultState;
		friend class CameraCloseState;
		friend class CameraReturnState;
		friend class CameraFixState;

	public:
		enum AvoidCollisionType
		{
			Obb
		};

	private:
		struct AlphaStruct
		{
			bool m_IsAvoid;
			weak_ptr<SmBaseDraw> m_DrawComp;
			float m_NowAlpha;

			AlphaStruct():
			m_IsAvoid(true),
			m_NowAlpha(1.0f)
			{

			}
		};

		unique_ptr<InputHandler> m_Input;
		weak_ptr<Transform> m_TargetTransform;

		///�^�[�Q�b�g�ƃJ�����̋���
		float m_Distance;
		///���ʍ��WZ����](���W�A��)
		float m_RotZ;
		///���ʍ��WX����](���W�A��)
		float m_RotX;
		///Z������]������X�s�[�h(�f�B�O���[�p�x���b)
		float m_RotZSpeed;
		///X������]������X�s�[�h(�f�B�O���[�p�x���b)
		float m_RotXSpeed;
		///�J�������ő�ŏ�ɍs���p�x(�f�B�O���[)
		float m_UpAngle;
		///�J�������ő�ŉ��ɍs���p�x(�f�B�O���[)
		float m_DownAngle;
		///�J�����̍ŒZ�̃^�[�Q�b�g�̒���(�f�t�H���g)
		float m_DefaultMinArms;
		///�J�����̍ŒZ�̃^�[�Q�b�g�̒���
		float m_MinArms;
		///�J�����̒����_�̍�
		Vec3 m_LookAtDiff;
		///�����x�̍ŏ��l(�f�t�H���g)
		float m_DefaultMinAlpha;
		///�����x�̍ŏ��l
		float m_MinAlpha;
		///�����⊮�Ɏg���l(�f�t�H���g)
		float m_DefaultAlphaLerp;
		///�����⊮�Ɏg���l
		float m_AlphaLerp;

		///CameraCloseState�łǂ��ɋ߂Â���
		Vec3 m_ClosetPos;

		float m_BefRotZ;

		Vec3 m_CloseStartPos;
		float m_CloseTimeCount = 0.0f;
		float m_CloseTime;

		std::map<wstring, AvoidCollisionType> m_AvoidObjectTags;
		vector<AlphaStruct> m_AlphaStructs;

		unique_ptr<StateMachine<TpsCamera>> m_StateMachine;

		Vec3 CalculateEyeLocalPosition(float rotX,float rotZ);

		template<typename T>
		vector<shared_ptr<T>> GetAvoidCollisiontVec(const wstring tag)
		{
			vector<shared_ptr<T>> ret;

			auto it = m_AvoidObjectTags.find(tag);
			if (it != m_AvoidObjectTags.end())
			{
				vector<shared_ptr<GameObject>> objects;
				App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetUsedTagObjectVec(tag,objects);
				for (auto obj : objects)
				{
					shared_ptr<T> collision = obj->GetComponent<T>();
					ret.push_back(collision);
				}

				return ret;
			}

			return ret;
		}

		void ChangeObjectAlpha(const shared_ptr<GameObject>& gameObject, const float alpha);

		void AvoidObject(Vec3& eye);

		void AddAlphaStruct(const shared_ptr<GameObject>& gameObject);
		void OffAvoidAlpha(const shared_ptr<GameObject>& gameObject);
		void UpdateAlpha();
		Vec3 GetResetEyePosition();

		///�ȉ��r�w�C�r�A
		void DefaultUpdate();
		void CloseUpdate();
		/*
		 *return �f�t�H���g�̈ʒu�ɖ߂������ǂ���
		*/
		bool ReturnUpdate();

		void FixUpdate();
	public:
		TpsCamera();
		virtual ~TpsCamera() {}

		void OnCreate() override;
		void OnUpdate() override;

		void SetTarget(const shared_ptr<Transform>& target);
		void SetAvoidTag(wstring tag, AvoidCollisionType collisionType);
		bool CloseUpTarget(Vec3 closePosition,float closeTime = 1.0f);
		void EndClose();
		void Fix(Vec3 eyePos, Vec3 atPos);
	};

	class CameraFixState : public ObjState<TpsCamera>
	{
		CameraFixState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(CameraFixState)

		virtual void Enter(const shared_ptr<TpsCamera>& camera)override;
		virtual void Execute(const shared_ptr<TpsCamera>& camera)override;
		virtual void Exit(const shared_ptr<TpsCamera>& camera)override;
	};

	class CameraDefaultState : public ObjState<TpsCamera>
	{
		CameraDefaultState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(CameraDefaultState)

		virtual void Enter(const shared_ptr<TpsCamera>& camera)override {};
		virtual void Execute(const shared_ptr<TpsCamera>& camera)override;
		virtual void Exit(const shared_ptr<TpsCamera>& camera)override {};
	};

	class CameraCloseState : public ObjState<TpsCamera>
	{
		CameraCloseState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(CameraCloseState)

		virtual void Enter(const shared_ptr<TpsCamera>& camera)override;
		virtual void Execute(const shared_ptr<TpsCamera>& camera)override;
		virtual void Exit(const shared_ptr<TpsCamera>& camera)override;
	};

	class CameraReturnState : public ObjState<TpsCamera>
	{
		CameraReturnState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(CameraReturnState)

		virtual void Enter(const shared_ptr<TpsCamera>& camera)override {};
		virtual void Execute(const shared_ptr<TpsCamera>& camera)override;
		virtual void Exit(const shared_ptr<TpsCamera>& camera)override {};
	};
}