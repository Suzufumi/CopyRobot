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
		};
	private:
		InitParam m_InitParam;

	public:
		BillboardSquare(const shared_ptr<Stage>& stagePtr,BillboardSquare::InitParam initParam);
		virtual ~BillboardSquare() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}