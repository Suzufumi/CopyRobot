#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TpsCamera::TpsCamera() :
		Camera(),
		m_Distance(6.0f),
		m_RotZ(XM_PIDIV2),
		m_RotX(0.0f),
		m_RotZSpeed(150.0f),
		m_RotXSpeed(120.0f),
		m_UpAngle(145.0f),
		m_DownAngle(90.0f),
		m_DefaultMinArms(6.0f),
		m_MinArms(m_DefaultMinArms),
		m_LookAtDiff(Vec3(0,1.0f,0)),
		m_DefaultMinAlpha(0.0775f),
		m_MinAlpha(m_DefaultMinAlpha),
		m_DefaultAlphaLerp(0.05f),
		m_AlphaLerp(m_DefaultAlphaLerp)
	{

	}

	void TpsCamera::OnCreate()
	{
		m_Input.reset(new InputHandler());

		m_StateMachine.reset(new StateMachine<TpsCamera>(GetThis<TpsCamera>()));
		m_StateMachine->ChangeState(CameraDefaultState::Instance());

		SetAvoidTag(L"Wall", AvoidCollisionType::Obb);
		SetAvoidTag(L"Pillar", AvoidCollisionType::Obb);
	}

	void TpsCamera::OnUpdate()
	{
		m_Input->StickHandle();
		m_StateMachine->Update();
	}

	void TpsCamera::DefaultUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();
		auto target = m_TargetTransform.lock();
		if (!target)
			return;

		Vec2 stickDir = m_Input->GetRStickDir();
		Vec2 cameraMoveDir = stickDir;
		float befLen = bsm::length(target->GetBeforePosition() - target->GetPosition());
		if (stickDir.length() < 0.1f && befLen > 0.05f)
		{
			Vec3 back(0.0f, 0.0f, -1.0f);
			Vec3 targetBack = -target->Forward();
			float angle = targetBack.angle(back);
			if (back.cross(targetBack).y < 0)
				angle *= -1.0f;

			Vec3 cameraForward = GetAt() - GetEye();
			cameraForward.y = 0.0f;
			cameraForward.normalize();
			float cameraTargetAngle = cameraForward.angle(target->Forward());

			Vec3 right(1, 0, 0);
			if ((cameraTargetAngle < Deg2Rad(120.0f)))
			{
				float setAngle = XM_PIDIV2 + angle;
				if (setAngle < 0)
				{
					setAngle = XM_2PI - (fabsf(angle) - XM_PIDIV2);
					if (m_RotZ < XM_PIDIV2)
						m_RotZ += XM_2PI;
				}
				
				if (m_BefRotZ > XM_PIDIV2*3.0f && setAngle < XM_PIDIV2)
					setAngle += XM_2PI;
				
				m_RotZ = Lerp::CalculateLerp(m_RotZ, setAngle, 0.0f, 1.0f, 0.04f, Lerp::Linear);
			}
		}


		m_RotZ += Deg2Rad(cameraMoveDir.x * m_RotZSpeed) * delta;
		if (fabsf(m_RotZ) > XM_2PI)
		{
			if (m_RotZ > 0)
				m_RotZ -= XM_2PI;
			else
				m_RotZ += XM_2PI;
		}
		if (m_RotZ < 0)
			m_RotZ = XM_2PI + m_RotZ;

		m_BefRotZ = m_RotZ;

		m_RotX -= Deg2Rad(cameraMoveDir.y * m_RotXSpeed) * delta;
		m_RotX = CLAMP(m_RotX, Deg2Rad(m_DownAngle), Deg2Rad(m_UpAngle));

		const Vec3 targetPos = target->GetPosition();

		Vec3 eye = CalculateEyeLocalPosition(m_RotX,m_RotZ) + targetPos;
		Vec3 befEye = GetEye();

		AvoidObject(eye);

		eye = Lerp::CalculateLerp(befEye, eye, 0, 1, 0.2f, Lerp::Linear);

		SetEye(eye);
		SetAt(targetPos + m_LookAtDiff);

		UpdateAlpha();

		auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		if (pad.bConnected)
		{
			if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ||
				pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				Fix(GetEye(), GetAt());
				EndClose();
			}
		}

		Camera::OnUpdate();
	}

	void TpsCamera::CloseUpdate()
	{
		auto target = m_TargetTransform.lock();
		if (!target)
			return;
		const Vec3 targetPos = target->GetPosition();

		m_CloseTimeCount += App::GetApp()->GetElapsedTime();
		Vec3 eye = Lerp::CalculateLerp<Vec3>(m_CloseStartPos, m_ClosetPos, 0.0f, m_CloseTime, m_CloseTimeCount,Lerp::Linear);

		AvoidObject(eye);

		SetEye(eye);

		UpdateAlpha();
		Camera::OnUpdate();
	}

	bool TpsCamera::ReturnUpdate()
	{
		auto target = m_TargetTransform.lock();
		if (!target)
			return false;

		Vec3 back(0.0f, 0.0f, -1.0f);
		Vec3 targetBack = -target->Forward();
		float angle = targetBack.angle(back);
		if (back.cross(targetBack).y < 0)
			angle *= -1.0f;
		
		m_RotX = XM_PIDIV2 + XM_PIDIV4 * 0.5f;
		m_RotZ = XM_PIDIV2 + angle;

		const Vec3 returnPos = CalculateEyeLocalPosition(m_RotX, m_RotZ) + target->GetPosition();

		Vec3 eye = Lerp::CalculateLerp<Vec3>(GetEye(), returnPos, 0.0f, 1.0f, 0.1f, Lerp::Linear);

		AvoidObject(eye);

		SetAt(target->GetPosition() + m_LookAtDiff);
		SetEye(eye);
		
		UpdateAlpha();
		Camera::OnUpdate();

		if (m_Input->GetLStickDir().length() > 0.2f)
			return true;

		if (bsm::length(returnPos - GetEye()) < 0.1f)
			return true;

		return false;
	}

	void TpsCamera::FixUpdate()
	{
		Vec3 eye = GetEye();
		AvoidObject(eye);
		UpdateAlpha();
		Camera::OnUpdate();
	}

	void TpsCamera::AddAlphaStruct(const shared_ptr<GameObject>& gameObject)
	{
		auto drawComp = gameObject->GetDynamicComponent<SmBaseDraw>(false);
		if (!drawComp)
			return;

		auto it = std::find_if(m_AlphaStructs.begin(), m_AlphaStructs.end(),
			[drawComp](AlphaStruct& a)
		{
			return a.m_DrawComp.lock() == drawComp;
		});

		if (it != m_AlphaStructs.end())
			return;

		AlphaStruct alphaStruct;
		alphaStruct.m_DrawComp = drawComp;

		m_AlphaStructs.push_back(alphaStruct);
	}

	void TpsCamera::OffAvoidAlpha(const shared_ptr<GameObject>& gameObject)
	{
		auto drawComp = gameObject->GetDynamicComponent<SmBaseDraw>(false);
		if (!drawComp)
			return;

		auto it = std::find_if(m_AlphaStructs.begin(), m_AlphaStructs.end(),
			[drawComp](AlphaStruct& a)
		{
			return a.m_DrawComp.lock() == drawComp;
		});

		if (it == m_AlphaStructs.end())
			return;

		m_AlphaStructs[std::distance(m_AlphaStructs.begin(), it)].m_IsAvoid = false;
	}

	void TpsCamera::UpdateAlpha()
	{
		vector<shared_ptr<SmBaseDraw>> removeStruct;

		for (auto it = m_AlphaStructs.begin(); it != m_AlphaStructs.end(); it++)
		{
			auto lkDraw = it->m_DrawComp.lock();
			if (!lkDraw)
				continue;
			if (it->m_IsAvoid)
			{
				it->m_NowAlpha = Lerp::CalculateLerp(it->m_NowAlpha, m_MinAlpha, 0, 1, m_AlphaLerp, Lerp::Linear);
			}
			else
			{
				it->m_NowAlpha = Lerp::CalculateLerp(it->m_NowAlpha, 1.0f, 0, 1, m_AlphaLerp, Lerp::Linear);
				if (1.0f - it->m_NowAlpha < 0.1f)
					removeStruct.push_back(lkDraw);
			}

			ChangeObjectAlpha(lkDraw->GetGameObject(),it->m_NowAlpha);
		}

		for (auto drawComp : removeStruct)
		{
			auto it = std::find_if(m_AlphaStructs.begin(), m_AlphaStructs.end(),
				[drawComp](AlphaStruct& a)
			{
				return a.m_DrawComp.lock() == drawComp;
			});

			ChangeObjectAlpha(drawComp->GetGameObject(), 1.0f);
			m_AlphaStructs.erase(it);
		}
	}

	void TpsCamera::AvoidObject(Vec3& eye)
	{
		auto target = m_TargetTransform.lock();
		if (!target)
			return;
		const Vec3 targetPos = target->GetPosition();

		for (auto& avoid : m_AvoidObjectTags)
		{
			switch (avoid.second)
			{
			case AvoidCollisionType::Obb:
			{
				auto obbComponents = GetAvoidCollisiontVec<CollisionObb>(avoid.first);
				if (obbComponents.size() <= 0)
					break;

				for (auto& obbComponent : obbComponents)
				{
					OBB obb = obbComponent->GetObb();
					SPHERE sphere = SPHERE(targetPos, 0.1f);
					Vec3 dir = eye - targetPos;

					float hitTime;
					if (HitTest::CollisionTestSphereObb(sphere, dir, obb, 0.0f, 1.0f, hitTime))
					{
						if (bsm::length(dir * hitTime) > m_MinArms)
							eye = targetPos + dir * hitTime;
						else
						{
							AddAlphaStruct(obbComponent->GetGameObject());

							eye = targetPos + bsm::normalize(dir) * m_MinArms;
						}
					}
					else
					{
						OffAvoidAlpha(obbComponent->GetGameObject());
					}
				}
			}
			break;
			}
		}
	}

	Vec3 TpsCamera::CalculateEyeLocalPosition(float rotX,float rotZ)
	{
		Vec3 eye;
		eye.x = m_Distance * sinf(rotX) * cosf(rotZ);
		eye.y = -m_Distance * cosf(rotX);
		eye.z = -m_Distance * sinf(rotX) * sinf(rotZ);

		return eye;
	}

	void TpsCamera::ChangeObjectAlpha(const shared_ptr<GameObject>& gameObject, const float alpha)
	{
		auto drawComp = gameObject->GetDynamicComponent<SmBaseDraw>(false);
		if (!drawComp)
			return;

		Col4 color = drawComp->GetDiffuse();
		color.w = alpha;
		drawComp->SetDiffuse(color);

		if (alpha < 1.0f)
		{
			gameObject->SetDrawLayer(5);
			drawComp->SetDepthStencilState(DepthStencilState::Read);
		}
		else
		{
			gameObject->SetDrawLayer(0);
			drawComp->SetDepthStencilState(DepthStencilState::Default);
		}
	}

	void TpsCamera::SetTarget(const shared_ptr<Transform>& target)
	{
		m_TargetTransform = target; 

		m_RotX = Deg2Rad((m_UpAngle + m_DownAngle) / 2.0f);
		SetAt(target->GetPosition());
	}

	Vec3 TpsCamera::GetResetEyePosition()
	{
		float rotZ = XM_PIDIV2;
		float rotX = Deg2Rad((m_UpAngle + m_DownAngle) / 2.0f);

		return CalculateEyeLocalPosition(m_RotX, m_RotZ);
	}

	void TpsCamera::SetAvoidTag(wstring tag, AvoidCollisionType collisionType)
	{
		m_AvoidObjectTags[tag] = collisionType;
	}

	bool TpsCamera::CloseUpTarget(Vec3 closePosition,float closeTime)
	{
		if (m_StateMachine->GetCurrentState() == CameraCloseState::Instance())
			return false;

		m_CloseTimeCount = 0.0f;
		m_CloseTime = closeTime;
		m_CloseStartPos = GetEye();

		m_ClosetPos = closePosition;
		m_StateMachine->ChangeState(CameraCloseState::Instance());

		SetAt(m_TargetTransform.lock()->GetPosition() + m_LookAtDiff);

		return true;
	}

	void TpsCamera::EndClose()
	{
		if (m_StateMachine->GetCurrentState() == CameraReturnState::Instance())
			return;

		m_StateMachine->ChangeState(CameraReturnState::Instance());
	}

	void TpsCamera::Fix(Vec3 eyePos, Vec3 atPos)
	{
		m_StateMachine->ChangeState(CameraFixState::Instance());

		SetEye(eyePos);
		SetAt(atPos);
	}

	IMPLEMENT_SINGLETON_INSTANCE(CameraFixState)

	void CameraFixState::Enter(const shared_ptr<TpsCamera>& camera)
	{
		camera->m_AlphaLerp = 1.0f;
		camera->m_MinAlpha = 0.0f;
	}

	void CameraFixState::Execute(const shared_ptr<TpsCamera>& camera)
	{
		camera->FixUpdate();
	}

	void CameraFixState::Exit(const shared_ptr<TpsCamera>& camera)
	{
		camera->m_AlphaLerp = camera->m_DefaultAlphaLerp;
		camera->m_MinAlpha = camera->m_DefaultMinAlpha;
	}

	IMPLEMENT_SINGLETON_INSTANCE(CameraDefaultState)

	void CameraDefaultState::Execute(const shared_ptr<TpsCamera>& camera)
	{
		camera->DefaultUpdate();
	}

	IMPLEMENT_SINGLETON_INSTANCE(CameraCloseState)

	void CameraCloseState::Enter(const shared_ptr<TpsCamera>& camera)
	{
		camera->m_MinAlpha = 0.0f;
		camera->m_AlphaLerp = 0.1f;
	}

	void CameraCloseState::Execute(const shared_ptr<TpsCamera>& camera)
	{
		camera->CloseUpdate();
	}

	void CameraCloseState::Exit(const shared_ptr<TpsCamera>& camera)
	{
		camera->m_MinAlpha = camera->m_DefaultMinAlpha;
		camera->m_AlphaLerp = camera->m_DefaultAlphaLerp;
	}

	IMPLEMENT_SINGLETON_INSTANCE(CameraReturnState)
	
	void CameraReturnState::Execute(const shared_ptr<TpsCamera>& camera)
	{
		if (camera->ReturnUpdate())
			camera->m_StateMachine->ChangeState(CameraDefaultState::Instance());
	}
}