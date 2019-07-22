/*!
@file Sprite.h
@brief �X�v���C�g
@author Kyohei Okawa
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	struct TexturePackInfo;

/** @addtogroup FairyLead
*   @brief FairyLead�Ǝ��̃v���O����
*/
/*@{*/

	/*!
	 *@struct SpriteAnimInfo
	 *@brief �X�v���C�g�̘A�ԃA�j���[�V�������
	 *@author Kyohei Okawa
	*/
	struct SpriteAnimInfo
	{
		//!�P���̊G��؂�ւ��鎞��(�b)
		float timeInterval;
		//!�؂�ւ��鎞�Ԃ��J�E���g����p
		float timeCount;
		//!�g�p���Ă���e�N�X�`���̃L�[
		wstring texKey;
		//!�e�N�X�`���A�g���X���̃L�[
		wstring xmlKey;
		//!���݉��Ԗڂ̉摜��
		int nowIndex;

		SpriteAnimInfo() :
			nowIndex(0),
			timeCount(0)
		{

		}
	};

	/*!
	 *@class Sprite
	 *@brief �X�v���C�g���N���X
	 *@author Kyohei Okawa
	*/
	class Sprite : public GameObject
	{
	public:
		/*!
		 *@enum BlendState
		 *@brief �u�����h�X�e�[�g
		 *@author Kyohei Okawa
		*/
		enum class BlendState
		{
			//!�s����
			Opaque,
			//!����
			Trace,
			//!���Z
			Additive
		};
	protected:
		//!�X�v���C�g�̃��b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		//!���_���̃o�b�N�A�b�v
		vector<VertexPositionColorTexture> m_BackupVertices;
		//!�g�p����e�N�X�`���̃L�[
		wstring m_TextureResName;
		//!�傫��
		Vec2 m_Scale;
		//!���W�A����](z��)
		float m_Rotation;
		//!�ʒu
		Vec2 m_Position;
		//!X�����ɉ摜������^�C�����O���邩
		UINT m_XWrap;
		//!Y�����ɉ摜������^�C�����O���邩
		UINT m_YWrap;
		//!�X�v���C�g�ɉ��Z����F
		Col4 m_Emissive;
		//!�X�v���C�g�ɏ�Z����F
		Col4 m_Diffuse;
		//!�u�����h�X�e�[�g
		BlendState m_BlendState;

		//!�A�ԃA�j���[�V�������
		SpriteAnimInfo m_AnimInfo;
		//!�A�j���[�V�������邩
		bool m_IsAnimated;
		//!�t�F�[�h�A�E�g���ł��邩
		bool m_IsFadeOut;
		//!�t�F�[�h�A�E�g�Ɋ|���鎞��
		float m_FadeOutInterval;
		//!�t�F�[�h�A�E�g�̎��Ԃ��J�E���g���邽�߂̕�
		float m_FadeOutCount;
	public:
		/*!
		 *@brief �R���X�g���N�^
		 *@param[in] stagePtr �ǉ�����X�e�[�W�̃|�C���^
		 *@param[in] textureResName �g�p����e�N�X�`���̃L�[
		 *@param[in] startScale �ŏ��̑傫��
		 *@param[in] startRot �ŏ��̉�]�p�i���W�A���j
		 *@param[in] startPos �ŏ��̈ʒu(�X�N���[���̍��W)
		 *@param[in] xWrap X�����ɉ摜������^�C�����O���邩
		 *@param[in] yWrap Y�����ɉ摜������^�C�����O���邩
		*/
		Sprite(const shared_ptr<Stage>& stagePtr,
			const wstring& textureResName,
			const Vec2& startScale,
			float startRot,
			const Vec2& startPos,
			UINT xWrap, UINT yWrap);

		//!�f�X�g���N�^
		virtual ~Sprite() {}
		/*!
		@brief �o�b�N�A�b�v���_�̏C��(���z�֐�)�B�h���N���X�͓Ǝ��̒��_�����C��������
		@return	�Ȃ�
		*/
		virtual void AdjustVertex() {}
		/*!
		@brief ���_�̕ύX(���z�֐�)�B�h���N���X�͓Ǝ��̒��_�ύX������
		@param[in]	ElapsedTime	�^�[������
		@param[out]	vertices	�}�b�v���ꂽ���_�f�[�^
		@return	�Ȃ�
		*/
		virtual void UpdateVertex(float elapsedTime, VertexPositionColorTexture* vertices);

		/*!
		 *@brief �g�p���Ă���e�N�X�`���̃L�[���擾����
		 *@return �g�p���Ă���e�N�X�`���̃L�[
		*/
		const wstring& GetTextureResName() const
		{
			return m_TextureResName;
		}

		/*!
		 *@brief �g�p����e�N�X�`���̃L�[��ύX����
		 *@param[in] textureResName �e�N�X�`���̃L�[
		 *@return �Ȃ�
		*/
		void SetTextureResName(const wstring& textureResName)
		{
			m_TextureResName = textureResName;
		}

		/*!
		 *@brief ���݂̃u�����h�X�e�[�g���擾����
		 *@return ���݂̃u�����h�X�e�[�g
		*/
		const BlendState& GetBlendState() const
		{
			return m_BlendState;
		}

		/*!
		 *@brief �u�����h�X�e�[�g��ύX����
		 *@param[in] blendState �u�����h�X�e�[�g
		 *@return �Ȃ�
		*/
		void SetBlendState(BlendState blendState)
		{
			m_BlendState = blendState;
		}

		/*!
		 *@brief �X�v���C�g�ɉ��Z����F���擾����
		 *@return �X�v���C�g�ɉ��Z����F
		*/
		const Col4& GetEmissive() const
		{
			return m_Emissive;
		}

		/*!
		 *@brief �X�v���C�g�ɉ��Z����F��ύX����
		 *@param[in] col �X�v���C�g�ɉ��Z����F
		 *@return �Ȃ�
		*/
		void SetEmissive(const Col4& col)
		{
			m_Emissive = col;
		}

		/*!
		*@brief �X�v���C�g�ɏ�Z����F���擾����
		*@return �X�v���C�g�ɏ�Z����F
		*/
		const Col4& GetDiffuse() const
		{
			return m_Diffuse;
		}

		/*!
		*@brief �X�v���C�g�ɏ�Z����F��ύX����
		*@param[in] col �X�v���C�g�ɏ�Z����F
		*@return �Ȃ�
		*/
		void SetDiffuse(const Col4& col)
		{
			m_Diffuse = col;
		}

		/*!
		 *@brief ���݂̈ʒu���擾����
		 *@return ���݂̈ʒu
		*/
		const Vec2& GetPosition() const
		{
			return m_Position;
		}

		/*!
		 *@brief ���݂̈ʒu��ύX����
		 *@param[in] pos �ݒ肷��ʒu
		 *@return �Ȃ�
		*/
		void SetPosition(const Vec2 pos)
		{
			m_Position = pos;
		}

		/*!
		 *@brief ���݂̑傫�����擾����
		 *@return ���݂̑傫��
		*/
		const Vec2& GetScale() const
		{
			return m_Scale;
		}

		/*!
		 *@brief �傫����ύX����
		 *@param[in] scale �ݒ肷��傫��
		 *@return �Ȃ�
		*/
		void SetScale(const Vec2 scale)
		{
			m_Scale = scale;
		}

		/*!
		@brief �A�g���X��񂩂�UV���W���v�Z��UV��ύX����
		@param[in] xmlKey �o�^�����A�g���X���(XML)�̃L�[
		@param[in] texname �e�N�X�`���A�g���X���̉摜�̖��O
		@return �Ȃ�
		*/
		void SelectTextureUVFromPackInfo(wstring xmlKey,wstring texname);

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;

		/*!
		 *@brief �A�ԃA�j���[�V�������Đ�����
		 *@param[in] texKey �g�p����e�N�X�`���̃L�[
		 *@param[in] xmlKey �A�g���X����xml�L�[
		 *@param[in] animIntervalSec �ꖇ�̉摜��؂�ւ���̂ɂ����鎞��
		 *@return �Ȃ�
		 *@details png�摜�����Ή��ŁA�A�g���X���ɂ̓A�j���[�V�����̏���0~N.png�̂悤�ɂ��ēo�^����</br>
		           0~N�̃��[�v�Đ��ɂ����Ή�
		*/
		void StartAnimation(wstring texKey, wstring xmlKey, float animIntervalSec);
		/*!
		 *@brief �A�ԃA�j���[�V�������~����
		 *@return �Ȃ�
		*/
		void StopAnimation()
		{
			m_IsAnimated = false;
		}

		/*!
		 *@brief �X�v���C�g���t�F�[�h�A�E�g������
		 *@param[in] outTime �t�F�[�h�A�E�g�ɂ����鎞��
		 *@return �Ȃ�
		 *@details �t�F�[�h�A�E�g��ɂ��̃I�u�W�F�N�g���폜���Ȃ�
		*/
		void FadeOut(float outTime);
	};
/*@}*/
}