/*!
@file PlayerBehaviour.h
@brief プレイヤーのビヘイビア
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class PlayerBehaviour : public Behavior
	{
		float m_Speed = 5;
		float m_PressTime = 0;
		float m_CopySuccessTime;
		weak_ptr<GameObject> m_CopyTarget;
		Player::CopyType m_CopyType;
	public:
		PlayerBehaviour(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr),
			m_CopySuccessTime(2.0f)
		{}

		virtual ~PlayerBehaviour()
		{}

		void Move();

	    Player::CopyType Copy();
		Player::CopyType TakeIn();
		void TakeInEnter();

		void SetSpeed(float speed) { m_Speed = speed; }
		void SetPressTime(float time) { m_PressTime = time; }
		float GetSpeed() { return m_Speed; }
		void SetCopyTarget(shared_ptr<GameObject>& targetEnemy) { m_CopyTarget = targetEnemy; }
		shared_ptr<GameObject> GetCopyTarget() { return m_CopyTarget.lock(); }
	};

	class PlayerCamereonBehaviour : public Behavior
	{
		float m_TransformTime = 1.0f;
		float m_TransformCount = 0.0f;

	public:
		PlayerCamereonBehaviour(const shared_ptr<GameObject>& gameObjectPtr):
			Behavior(gameObjectPtr)
		{}
		virtual ~PlayerCamereonBehaviour() {}

		bool Transform();

		bool CancelTransform();
	};

}
//end basecross
