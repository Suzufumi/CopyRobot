#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void PlayerBehaviour::Move()
	{
		auto player = dynamic_pointer_cast<Player>(GetGameObject());

		auto cam = GetStage()->GetView()->GetTargetCamera();
		Vec2 Dir = player->GetInputHandler()->GetLStickDir();
		Vec3 Forward = cam->GetAt() - cam->GetEye();
		Forward.y = 0;
		Forward.normalize();
		Vec3 Right = MyUtil::rotateXZVec3(Forward, 90);
		Forward *= Dir.y;
		Right *= Dir.x;
		Vec3 NewVel = Forward + Right;
		player->Move(NewVel * m_Speed);
	}

	Player::CopyType PlayerBehaviour::Copy()
	{
		auto player = dynamic_pointer_cast<Player>(GetGameObject());
		auto targetLock = m_CopyTarget.lock();
		if (!targetLock)
			MessageBox(NULL, L"", L"プレイヤーコピー対象オブジェクトが設定されていません", MB_OK);
		auto target = dynamic_pointer_cast<OurGameObject>(targetLock);
		if (!target)
			MessageBox(NULL, L"", L"プレイヤーコピー対象オブジェクトにOurGameObjectクラス以外のクラスが設定されています", MB_OK);

		auto drawComp = player->GetComponent<PNTBoneModelDraw>();

		auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		if (pad.bConnected)
		{
			//Aボタン
			if (pad.wButtons & XINPUT_GAMEPAD_A)
			{
				m_PressTime += App::GetApp()->GetElapsedTime();

				auto copySuccGauge = player->GetCopySuccessGauge();
				if (!copySuccGauge->GetIsVisible())
					copySuccGauge->RunOpenAction();

				player->SetCopySuccessValue(m_PressTime / m_CopySuccessTime);

				if (m_PressTime >= m_CopySuccessTime)
				{
					target->SetUpdateActive(true);
					player->SetCopyAbilityValue(1.0f);
					
					if (target->FindTag(L"NormalEnemy"))
						m_CopyType = Player::CopyType::NormalEnemy;
					else if (target->FindTag(L"CamereonEnemy"))
						m_CopyType = Player::CopyType::CamereonEnemy;
					else if (target->FindTag(L"Pillar"))
						m_CopyType = Player::CopyType::Pillar;
					else if (target->FindTag(L"CrystalTable"))
						m_CopyType = Player::CopyType::CrystalTable;

					return m_CopyType;
				}

			}
			if (pad.wReleasedButtons & XINPUT_GAMEPAD_A)
			{
				if (m_PressTime <= m_CopySuccessTime)
				{
					target->SetUpdateActive(true);
					m_CopyType = Player::CopyType::Normal;
					return Player::CopyType::Normal;
				}
			}
		}

		m_CopyType = Player::CopyType::None;
		return Player::CopyType::None;
	}

	void PlayerBehaviour::TakeInEnter()
	{
		auto player = dynamic_pointer_cast<Player>(GetGameObject());
		auto targetLock = m_CopyTarget.lock();
		auto target = dynamic_pointer_cast<OurGameObject>(targetLock);
		if (!target)
			MessageBox(NULL, L"", L"プレイヤーコピー対象オブジェクトにOurGameObjectクラス以外のクラスが設定されています", MB_OK);

		target->Stop();
		target->TakenIn(player->GetComponent<Transform>()->GetPosition() + Vec3(0, 0.4f, 0.0f));

		if (target->FindTag(L"CopyObj"))
			target->GetComponent<Collision>()->SetUpdateActive(false);

		App::GetApp()->GetXAudio2Manager()->Start(L"INHALE_SE", 0);
	}

	Player::CopyType PlayerBehaviour::TakeIn()
	{
		//取り込みが終わったらreturn m_CopyType;
		auto targetLock = m_CopyTarget.lock();
		auto target = dynamic_pointer_cast<OurGameObject>(targetLock);

		if (!target)
			MessageBox(NULL, L"", L"プレイヤーコピー対象オブジェクトにOurGameObjectクラス以外のクラスが設定されています", MB_OK);
		auto ptrAction = target->AddComponent<Action>();

		if (ptrAction->IsArrived())
		{
			target->EndTakeIn();
			return m_CopyType;
		}

		return Player::CopyType::None;
	}

	bool PlayerCamereonBehaviour::Transform()
	{
		auto player = dynamic_pointer_cast<Player>(GetGameObject());

		float delta = App::GetApp()->GetElapsedTime();
		m_TransformCount += delta;

		float alpha = m_TransformCount / m_TransformTime;
		player->GetAssimililationObject()->ChangeAlpha(alpha);
		player->ChangeAlpha(1.0f - alpha);

		if (m_TransformCount > m_TransformTime)
		{
			m_TransformCount = m_TransformTime;
			return true;
		}

		return false;
	}

	bool PlayerCamereonBehaviour::CancelTransform()
	{
		auto player = dynamic_pointer_cast<Player>(GetGameObject());

		float delta = App::GetApp()->GetElapsedTime();
		m_TransformCount -= delta;

		float alpha = m_TransformCount / m_TransformTime;
		player->GetAssimililationObject()->ChangeAlpha(alpha);
		player->ChangeAlpha(1.0f - alpha);

		if (m_TransformCount < 0)
		{
			m_TransformCount = 0;
			return true;
		}

		return false;
	}
}
// end basecross