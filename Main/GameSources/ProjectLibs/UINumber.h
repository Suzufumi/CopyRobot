/*!
@file UINumber.h
@brief �����X�v���C�g
@author Kyohei Okawa
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
/** @addtogroup FairyLead
*   @brief FairyLead�Ǝ��̃v���O����
*/
/*@{*/

	/*!
	 *@class UINumber
	 *@brief �ꂯ�������̐�����\������X�v���C�g
	 *@author Kyohei Okawa
	*/
	class UINumber : public Sprite
	{
		//!�\�����鐔��(0�`9)
		int m_Number;
		//!�\�����邩���Ȃ���
		bool m_IsVisible;
	public:
		/*!
		 *@brief �R���X�g���N�^
		 *@param[in] stagePtr ���̃I�u�W�F�N�g��ǉ�����X�e�[�W
		 *@param[in] textureResName 0�`9�̐��������񂾃e�N�X�`���̃L�[
		 *@param[in] startScale �����̑傫��
		 *@param[in] startRot �����̉�]
		 *@param[in] startPos �����ʒu
		 *@param[in] number �\�����鐔��(0�`9�����ꂩ�̐���)
		*/
		UINumber(const shared_ptr<Stage>& stagePtr,
			const wstring& textureResName,
			const Vec2& startScale,
			float startRot,
			const Vec2& startPos,
			int number);
		//!�f�X�g���N�^
		~UINumber() {}

		virtual void AdjustVertex() override;

		virtual void OnCreate() override;
		virtual void OnDraw() override;

		//!�\�����Ă��鐔�����擾����
		int GetNumber()
		{
			return m_Number;
		}
		//!�\��(true),��\��(false)��ݒ肷��
		void SetVisible(bool visible)
		{
			m_IsVisible = visible;
		}
		//!�\�����鐔����ύX����(0�`9)
		void ChangeNumber(int number);
		//!�F��ύX����
		void ChangeColor(Col4 color);
	};

	/*!
	 *@class UINumbers
	 *@brief �������̐�����\������X�v���C�g
	 *@author Kyohei Okawa
	*/
	class UINumbers : public GameObject
	{
		//!0�`9�̐��������񂾃e�N�X�`���̃L�[
		wstring m_TextureResName;
		//!�ꌅ���̂̐����X�v���C�g�̏����̑傫��
		Vec2 m_StartNumScale;
		//!�����ʒu1�̈ʂ��n�_�ɂȂ�
		Vec2 m_StartPos;
		//!�����\�����邩
		int m_NumOfDigits;
		//!�\�����鐮��
		int m_Number;
		//!0032�̂悤�ɂȂ����Ƃ���0�̕�����\��(true)�A��\��(false)�ɂ��邩
		bool m_ShouldShowZero;

		//!�ꌅ���̃X�v���C�g�̃x�N�^�[
		vector<shared_ptr<UINumber>> m_NumSprites;
	public:
		/*!
		 *@brief �R���X�g���N�^
		 *@param[in] stagePtr ���̃I�u�W�F�N�g��ǉ�����X�e�[�W
		 *@param[in] textureResName 0�`9�̐��������񂾃e�N�X�`���̃L�[
		 *@param[in] startNumScale �ꌅ���̂̐����X�v���C�g�̏����̑傫��
		 *@param[in] startPos �����ʒu(�P�̈ʂ��n�_)
		 *@param[in] numOfDigits �����܂ŕ\�����邩
		 *@param[in] number �\�����鐮��
		 *@param[in] shouldShowZero 0032�̂悤�ɂȂ����Ƃ���0�̕�����\��(true)�A��\��(false)�ɂ��邩
		*/
		UINumbers(const shared_ptr<Stage>& stagePtr,
			const wstring& textureResName,
			const Vec2& startNumScale,
			const Vec2& startPos,
			int numOfDigits,
			int number,
			bool shouldShowZero
		);
		//!�f�X�g���N�^
		~UINumbers() {}

		virtual void OnCreate()override;

		//!�\�����Ă��鐔�����擾����
		int GetNumber()
		{
			return m_Number;
		}
		//!�ꌅ�̐����X�v���C�g�̑傫�����擾����
		Vec2 GetScale()
		{
			return m_StartNumScale;
		}
		//!�P�̈ʂ̐����X�v���C�g�̈ʒu���擾����
		Vec2 GetPosition()
		{
			return m_StartPos;
		}

		//!�\�����鐮����ύX����
		void ChangeNumber(int number);
		//!���ׂĂ̌��̐����X�v���C�g�̐F��ύX����
		void ChangeColor(Col4 color);
		//!�P�̈ʂ̃X�v���C�g�̈ʒu���n�_�Ƃ��Ĉʒu��ύX����
		void ChangePosition(Vec2 pos);
		//!���ׂĂ̐����̑傫����ύX���A�ʒu�����킹��
		void ChangeScale(Vec2 scale);
		//!�\���F���擾����
		Col4 GetColor();
		//!0032�̂悤�ɂȂ����Ƃ���0�̕�����\��(true)�A��\��(false)�ɂ��邩��ݒ肷��
		void SetShouldShowZero(bool b);

		//!���̃N���X��j������ꍇ�͂��̊֐����Ă�
		void Destroy();
	};

/*@}*/
}