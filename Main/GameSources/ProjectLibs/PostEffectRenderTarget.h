/*!
* @file PostEffectRenderTarget.h
* @brief �`���ɂ�����G�t�F�N�g
* @author Kyohei Okawa
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
/** @addtogroup FairyLead
*  @brief FairyLead�Ǝ��̃v���O����
*/
/*@{*/

	/*!
	 *@class PostEffectRenderTarget
	 *@brief �|�X�g�G�t�F�N�g�p�̃����_�[�^�[�Q�b�g
	 *@author Kyohei Okawa
	*/
	class PostEffectRenderTarget : public RenderTarget
	{
	public:
		PostEffectRenderTarget();
		virtual ~PostEffectRenderTarget();

		/*!
		 *@brief ���̃����_�[�^�[�Q�b�g�ɕ`�悳�ꂽ�����V�F�[�_�[���\�[�X�r���[�Ƃ��ĕԂ�
		 *@return ���̃����_�[�^�[�Q�b�g�ɕ`�悳�ꂽ�V�F�[�_�[���\�[�X�r���[
		*/
		ID3D11ShaderResourceView* GetShaderResourceView() const;

		virtual void ClearViews(const bsm::Col4& col = bsm::Col4(0, 0, 0, 1.0f)) override;

		virtual void StartRenderTarget() override;

		virtual void EndRenderTarget() override;

	private:
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	/*!
	 *@enum PostEffectColor
	 *@brief �`����ʂ����F�ɂ��邩
	 *@author Kyohei Okawa
	*/
	enum PostEffectColor
	{
		//!�`�悳�ꂽ�F�����̂܂܂ɂ���
		Color,
		//!�����ɂ���
		GrayScale,
		//!�Z�s�A�F�ɂ���
		Sepia
	};

	/*!
	 *@class PostEffectSprite
	 *@brief �I�t�X�N���[�������_�����O���ꂽ���̂�`�悷�邽�߂̃X�v���C�g
	 *@author Kyohei Okawa
	*/
	class PostEffectSprite : public GameObject
	{
		//!���b�V�����\�[�X
		shared_ptr<MeshResource> m_MeshResource;
		//!���F�ɂ��ĕ`�悷�邩
		PostEffectColor m_PostEffectColor;

		//!�\�����邩���Ȃ���
		bool m_IsVisible;
		//!�o�͂���s�N�Z���F�Ɋ|����F
		Col4 m_Diffuse;

		weak_ptr<PostEffectRenderTarget> m_RenderTarget;
	public:
		/*!
		 *@brief �R���X�g���N�^
		 *@param[in] stage ���̃I�u�W�F�N�g��ǉ�����X�e�[�W�̃|�C���^
		*/
		PostEffectSprite(const shared_ptr<Stage>& stage,const shared_ptr<PostEffectRenderTarget> renderTarget);
		//!�f�X�g���N�^
		~PostEffectSprite() {}

		virtual void OnCreate() override;
		virtual void OnDraw() override;

		/*!
		 *@brief �`�悷��ۂ̐F��ύX����
		 *@param[in] postEffectColor �`�悷��ۂ̐F
		 *@return �Ȃ�
		*/
		void ChangePostEffectColor(PostEffectColor postEffectColor)
		{
			m_PostEffectColor = postEffectColor;
		}

		/*!
		 *@brief ���ݕ\������Ă����Ԃ��擾����
		 *@return true �\������Ă���
		 *@return false�@��\���ɂȂ��Ă���
		*/
		bool GetIsVisible()
		{
			return m_IsVisible;
		}

		/*!
		 *@brief �\���A��\�����w�肷��
		 *@param[in] visible true�̏ꍇ���\��
		 *@return �Ȃ�
		*/
		void SetIsVisible(bool visible)
		{
			m_IsVisible = visible;
		}

		/*!
		 *@brief �o�͂���s�N�Z���F�Ɋ|����F���擾����
		 *@return �o�͂���s�N�Z���F�Ɋ|����F
		*/
		Col4 GetDiffuse()
		{
			return m_Diffuse;
		}

		/*!
		 *@brief �o�͂���s�N�Z���F�Ɋ|����F���w�肷��
		 *@param[in] diffuse �o�͂���s�N�Z���F�Ɋ|����F
		 *@return �Ȃ�
		*/
		void SetDiffuse(Col4 diffuse)
		{
			m_Diffuse = diffuse;
		}
	};
/*@}*/
}