#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Gauge::Gauge(const shared_ptr<Stage>& stagePtr, Vec2 position) :
		Sprite(stagePtr,L"GAUGE_ABILITY_BAR",Vec2(315*0.7f, 147*0.7f),0.0f,position,1,1),
		m_Value(1.0f),
		m_StartPos(position)
	{

	}

	void Gauge::OnCreate()
	{
		Sprite::OnCreate();

		m_GaugeBase = ObjectFactory::Create<Sprite>(
			GetStage(),
			L"GAUGE_ABILITY_BASE",
			Vec2(430 * 0.7f, 147 * 0.7f),
			0.0f,
			m_StartPos + Vec2(78, 0),
			1, 1
			);
		
		SetDrawLayer(200);

		AddTag(L"CopyGauge");
	}

	void Gauge::OnUpdate()
	{
		Sprite::OnUpdate();

		vector<shared_ptr<GameObject>> m_Players;
		GetStage()->GetUsedTagObjectVec(L"Player",m_Players);

		auto player = dynamic_pointer_cast<Player>(m_Players[0]);
		m_Value = player->GetCopyAbilityValue();
	}

	void Gauge::OnDraw()
	{
		Sprite::OnDraw();
		m_GaugeBase->OnDraw();
	}

	void Gauge::AdjustVertex()
	{
		m_BackupVertices.clear();

		float halfSize = 0.5f;

		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(0, halfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(1, halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(0, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(1, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
	}

	void Gauge::UpdateVertex(float elapsedTime, VertexPositionColorTexture* vertices)
	{
		for (size_t i = 0; i < m_BackupVertices.size(); i++)
		{
			vertices[i] = m_BackupVertices[i];

			if (i == 1 || i == 3)
			{
				VertexPositionColorTexture newVertex = m_BackupVertices[i];
				newVertex.position.x = m_Value;
				newVertex.textureCoordinate.x = m_Value;
				vertices[i] = newVertex;
			}
		}
	}
}