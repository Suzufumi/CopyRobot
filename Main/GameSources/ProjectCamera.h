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

		///ターゲットとカメラの距離
		float m_Distance;
		///球面座標Z軸回転(ラジアン)
		float m_RotZ;
		///球面座標X軸回転(ラジアン)
		float m_RotX;
		///Z軸を回転させるスピード(ディグリー角度毎秒)
		float m_RotZSpeed;
		///X軸を回転させるスピード(ディグリー角度毎秒)
		float m_RotXSpeed;
		///カメラが最大で上に行く角度(ディグリー)
		float m_UpAngle;
		///カメラが最大で下に行く角度(ディグリー)
		float m_DownAngle;
		///カメラの最短のターゲットの長さ(デフォルト)
		float m_DefaultMinArms;
		///カメラの最短のターゲットの長さ
		float m_MinArms;
		///カメラの注視点の差
		Vec3 m_LookAtDiff;
		///透明度の最小値(デフォルト)
		float m_DefaultMinAlpha;
		///透明度の最小値
		float m_MinAlpha;
		///透明補完に使う値(デフォルト)
		float m_DefaultAlphaLerp;
		///透明補完に使う値
		float m_AlphaLerp;

		///CameraCloseStateでどこに近づくか
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

		///以下ビヘイビア
		void DefaultUpdate();
		void CloseUpdate();
		/*
		 *return デフォルトの位置に戻ったかどうか
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