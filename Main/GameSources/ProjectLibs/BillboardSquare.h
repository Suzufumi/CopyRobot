#pragma once
#include "stdafx.h"

namespace basecross
{
	class BillboardSquare : public GameObject
	{
	public:
		struct InitParam
		{
			Vec3 m_Position;
			Vec3 m_Scale;
			wstring m_TexKey;
			float m_AspectWdivH;

			InitParam() :
				m_AspectWdivH(1.0f)
			{

			}
		};

	private:
		BillboardSquare::InitParam m_InitParam;
		vector<VertexPositionColorTexture> m_BackupVertices;
		shared_ptr<MeshResource> m_SquareMesh;
		float m_Value;

	public:
		BillboardSquare(const shared_ptr<Stage>& stagePtr, BillboardSquare::InitParam initParam);
		virtual ~BillboardSquare() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void SetMeshVertexPositionUvX(float value);
		float GetValue() { return m_Value; }
	};

	class CopySuccessGauge : public GameObject
	{
	public:
		struct InitParam
		{
			weak_ptr<Transform> m_Target;
			Vec3 m_PositionDiff;
		};
	protected:
		CopySuccessGauge::InitParam m_InitParam;
		shared_ptr<BillboardSquare> m_BackSquare;
		shared_ptr<BillboardSquare> m_FrontSquare;
		shared_ptr<BillboardSquare> m_FrameSquare;
		shared_ptr<BillboardSquare> m_StateSquare;
		vector<shared_ptr<BillboardSquare>> m_Squares;
		BillboardSquare::InitParam m_SquareInitParam;
		bool m_IsVisible;
		bool m_IsRunCloseAction;
		bool m_IsRunOpenAction;
		bool m_IsOpened;
		bool m_IsPlaySuccessVoice;

	public:
		CopySuccessGauge(const shared_ptr<Stage>& stagePtr, CopySuccessGauge::InitParam initParam);
		virtual ~CopySuccessGauge() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnUpdate2()override;
		virtual void OnDraw()override;
		virtual void OnEvent(const shared_ptr<Event>& event)override;
		void SetGaugeValue(float value);

		virtual void RunOpenAction();
		virtual void RunCloseAction();

		bool GetIsVisible() { return m_IsVisible; }
		bool GetIsRunCloseAction() { return m_IsRunCloseAction; }
	};

	class UsageTimeGauge : public CopySuccessGauge
	{
	public:
		UsageTimeGauge(const shared_ptr<Stage>& stagePtr, CopySuccessGauge::InitParam initParam);
		virtual ~UsageTimeGauge() {}

		virtual void OnCreate() override;

		virtual void RunOpenAction()override;
		virtual void SetGaugeValue(float value);
	};
}