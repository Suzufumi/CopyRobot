/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class PlayerEgg : public GameObject
	{
	public:
		PlayerEgg(const shared_ptr<Stage>& stagePtr):
			GameObject(stagePtr){}
		virtual ~PlayerEgg() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};

	class Player : public AliveGameObject
	{
		friend class PlayerCopyingState;
		friend class PlayerNormalState;
		friend class PlayerCamereonDefaultState;
		friend class PlayerDeadState;
		friend class PlayerGoalState;
		friend class PlayerStartState;
		friend class PlayerTakeInState;

	public:
		enum CopyType 
		{
			None,//コピー中
			Normal,//何のコピー能力も持ってない
			NormalEnemy,//ノーマルエネミーになれる
			CamereonEnemy, // カメレオンエネミーになれる
			Pillar,
			CrystalTable
		};

		struct InitializeParam
		{
			Vec3 m_startPos;
			Quat m_startQuat;
			Vec3 m_startScale;
		};

	private:
		InitializeParam m_initParam;
		//エフェクト
		shared_ptr<EfkEffect> m_DownEffect;
		//エフェクト実行オブジェクト
		shared_ptr<EfkPlay> m_EfkPlay;

		unique_ptr<InputHandler> m_InputHandler;
		//ステートマシーン
		unique_ptr<StateMachine<Player>> m_StateMachine;

		unique_ptr<StateMachine<Player>> m_AnimationStateMachine;

		shared_ptr<PlayerAssimililationObject> m_AssimililationObject;

		weak_ptr<CopySuccessGauge> m_CopySuccessGauge;

		weak_ptr<UsageTimeGauge> m_UsageTimeGauge;

		float m_BehindDistance = 5;

		shared_ptr<MeshResource> m_MeshResForMinimap;

		CopyType m_NowCopyType = CopyType::None;

		weak_ptr<TargetCursor> m_TargetCursor;

		weak_ptr<Square> m_Shadow;

		Vec3 m_GoalPos;

		weak_ptr<GameObject> m_Egg;

		float m_CopySuccessGaugeValue;
		float m_CopyAbilityGaugeValue;
		float m_CopyValueDecreaseSpeedStaticObj;
		float m_CopyValueDecreaseSpeedEnemy;

	public:
		Player(const shared_ptr<Stage>& stagePtr)
			: AliveGameObject(stagePtr),
			m_CopySuccessGaugeValue(0.0f),
			m_CopyValueDecreaseSpeedStaticObj(0.05f),
			m_CopyAbilityGaugeValue(0.0f)
		{}
		Player(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);

		virtual ~Player()
		{}
		// 構築
		virtual void OnCreate() override;
		// 更新
		virtual void OnUpdate() override;

		virtual void OnUpdate2() override;

		virtual void OnDraw() override;
		// 移動
		virtual void Move(Vec3 direction);
		//あたり判定
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;

		vector<shared_ptr<GameObject>> GetNearObjectVec(wstring searchTag);

		bool CheckIsBehindEnemy(const shared_ptr<Transform>& enemyTrans);

		InputHandler* GetInputHandler() { return m_InputHandler.get(); }

		unique_ptr< StateMachine<Player> >& GetStateMachine(){return m_StateMachine;}

		unique_ptr<StateMachine<Player>>& GetAnimationStateMachine() { return m_AnimationStateMachine; }

		shared_ptr<PlayerAssimililationObject>& GetAssimililationObject() { return m_AssimililationObject; }

		shared_ptr<CopySuccessGauge> GetCopySuccessGauge(){return m_CopySuccessGauge.lock();}

		shared_ptr<UsageTimeGauge> GetUsageTimeGauge() { return m_UsageTimeGauge.lock(); }

		CopyType GetPlayerCopytype() { return m_NowCopyType; }

		float GetCopySuccessValue() { return m_CopySuccessGaugeValue; }
		void SetCopySuccessValue(float value) { m_CopySuccessGaugeValue = value; }

		float GetCopyAbilityValue() { return m_CopyAbilityGaugeValue; }
		void SetCopyAbilityValue(float value) { m_CopyAbilityGaugeValue = value; }

		shared_ptr<GameObject> GetTargetCursor() { return m_TargetCursor.lock(); }

		void ChangeAlpha(float alpha);

		float DecreaseCopyValue();

		shared_ptr<MeshResource> GetMeshResourceForMinimap();

		bool SetCursorToCopyableObject();

		void Goal(Vec3 goalPos);

		Vec3 GetStartPosition() { return m_initParam.m_startPos; }
		Quat GetStartQuat() { return m_initParam.m_startQuat; }
	};
}
//end basecross