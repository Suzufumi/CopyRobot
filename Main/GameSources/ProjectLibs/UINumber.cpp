/*!
@file UINumber.cpp
@brief 数字スプライト
@author Kyohei Okawa
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	UINumber::UINumber(const shared_ptr<Stage>& stagePtr,
		const wstring& textureResName,
		const Vec2& startScale,
		float startRot,
		const Vec2& startPos,
		int number) :
		Sprite(stagePtr, textureResName, startScale, startRot, startPos, 1, 1),
		m_Number(number),
		m_IsVisible(true)
	{

	}

	void UINumber::AdjustVertex()
	{
		float width = 1.0f / 10.0f;
		float xOffset = m_Number / 10.0f;

		m_BackupVertices.clear();

		float halfSize = 0.5f;
		m_BackupVertices =
		{
			{ VertexPositionColorTexture(Vec3(-halfSize, halfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(xOffset, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(halfSize, halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)xOffset + width, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-halfSize, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(xOffset, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(halfSize, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)xOffset + width, (float)m_YWrap)) },
		};
	}

	void UINumber::OnCreate()
	{
		Sprite::OnCreate();

		SetBlendState(BlendState::Trace);
	}

	void UINumber::OnDraw()
	{
		if (!m_IsVisible)
			return;

		Sprite::OnDraw();
	}

	void UINumber::ChangeNumber(int number)
	{
		if (number < 0)
			number = 0;
		else if (number > 9)
			number = 9;

		m_Number = number;

		AdjustVertex();
	}

	void UINumber::ChangeColor(Col4 color)
	{
		SetDiffuse(color);
	}


	UINumbers::UINumbers(const shared_ptr<Stage>& stagePtr,
		const wstring& textureResName,
		const Vec2& startNumScale,
		const Vec2& startPos,
		int numOfDigits,
		int number,
		bool shouldShowZero) :
		GameObject(stagePtr),
		m_TextureResName(textureResName),
		m_StartNumScale(startNumScale),
		m_StartPos(startPos),
		m_NumOfDigits(numOfDigits),
		m_Number(number),
		m_ShouldShowZero(shouldShowZero)
	{

	}

	void UINumbers::OnCreate()
	{
		int digit = MyUtil::getDigit(m_Number);
		auto digits = MyUtil::extractTotalDigitsSpotCount(m_Number,m_NumOfDigits);
		Vec2 offset = Vec2(0,0);

		for (int i = 0; i < m_NumOfDigits; i++)
		{
			offset.x = m_StartNumScale.x * i;
			
			auto obj = GetStage()->AddGameObject<UINumber>(
				m_TextureResName,
				m_StartNumScale,
				0.0f,
				m_StartPos - offset,
				digits.at(i)
				);
			auto num = obj->GetThis<UINumber>();

			if (i >= digit && !m_ShouldShowZero)
				num->SetVisible(false);

			m_NumSprites.push_back(num);
		}
	}

	void UINumbers::ChangeNumber(int number)
	{
		m_Number = number;
		auto digit = MyUtil::getDigit(m_Number);
		auto digits = MyUtil::extractTotalDigitsSpotCount(m_Number, m_NumOfDigits);

		for (int i = 0; i < (int)digits.size(); i++)
		{
			m_NumSprites.at(i)->ChangeNumber(digits.at(i));

			if (!m_ShouldShowZero)
			{
				if (i >= digit)
					m_NumSprites.at(i)->SetVisible(false);
				else
					m_NumSprites.at(i)->SetVisible(true);
			}
		}
	}

	void UINumbers::ChangeColor(Col4 color)
	{
		for (auto& num : m_NumSprites)
			num->ChangeColor(color);
	}

	Col4 UINumbers::GetColor()
	{
		return m_NumSprites[0]->GetDiffuse();
	}

	void UINumbers::SetShouldShowZero(bool b)
	{
		m_ShouldShowZero = b;

		ChangeNumber(m_Number);
	}

	void UINumbers::ChangePosition(Vec2 pos)
	{
		m_StartPos = pos;

		Vec2 offset(0, 0);
		for (int i = 0; i < m_NumOfDigits; i++)
		{
			offset.x = m_StartNumScale.x * i;

			m_NumSprites.at(i)->SetPosition(m_StartPos - offset);
		}
	}

	void UINumbers::ChangeScale(Vec2 scale)
	{
		m_StartNumScale = scale;

		Vec2 offset(0, 0);
		for (int i = 0; i < m_NumOfDigits; i++)
		{
			offset.x = m_StartNumScale.x * i;

			m_NumSprites.at(i)->SetPosition(m_StartPos - offset);
			m_NumSprites.at(i)->SetScale(m_StartNumScale);
		}
	}

	void UINumbers::Destroy()
	{
		auto stage = GetStage();
		for (auto obj : m_NumSprites)
			stage->RemoveGameObject<GameObject>(obj->GetThis<GameObject>());

		stage->RemoveGameObject<GameObject>(GetThis<GameObject>());
	}
}