/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

namespace basecross
{
	class Transition;
	class StageSelectStage;

	/*!
	*@struct PackedTextureInfo
	*@brief TexturePacker�ŏo�͂����摜�̏��
	*@author Kyohei Okawa
	*/
	struct PackedTextureInfo
	{
		//!�o�^�����e�N�X�`���̃L�[
		wstring textureKey;
		//!�e�N�X�`���̕��ƍ���(�s�N�Z��)
		int width, height;
	};

	/*!
	*@struct TexturePackInfo
	*@brief TexturePacker�ŏo�͂����A�g���X�e�N�X�`���̒��̈��̉摜�̏��
	*@author Kyohei Okawa
	*/
	struct TexturePackInfo
	{
		//!�摜�̖��O
		wstring name;
		//!�e�N�X�`���̒��̍��W(�s�N�Z��) ����̈ʒu
		int x, y;
		//!�摜�̕��ƍ���(�s�N�Z��)
		int width, height;

		//!���̈ꖇ�̃e�N�X�`���̏��
		PackedTextureInfo packedTextureInfo;
	};

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase
	{
		shared_ptr<SoundItem> m_Audio;
		shared_ptr<GameObject> m_LoadingAnim;

	private:
		shared_ptr<Transition> m_Transition;

		vector<shared_ptr<StageSelectStage>> m_StageSelectStages;

		//--------------------------------------------------------------------------------------
		/*!
		@brief ���\�[�X�擾
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void CreateResources();

		//!TexturePacker�ŏo�͂��ꂽxml�̏���o�^����
		unordered_map<wstring, unordered_map<wstring, TexturePackInfo>> m_PackdTextureInfo;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;

		virtual void OnUpdate() override;

		virtual void OnDraw() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g�擾
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;

		shared_ptr<Transition> GetTransition() { return m_Transition; }

		//���݃Z���N�g����Ă���X�e�[�W�i���o�[
		int m_SrageNum;
		//�X�e�[�W�i���o�[�̃A�N�Z�T
		int GetStageNum() const 
		{
			return m_SrageNum;
		}
		void SetStageNum(int i)
		{
			m_SrageNum = i;
		}
		//GameStage����N���A����܂ł�Time�𑪂�B
		int m_TotalTime;
		int GetTotalTime() const
		{
			return m_TotalTime;
		}
		void SetTotalTime(int Time)
		{
			m_TotalTime = Time;
		}

		/*!
		@brief �A�g���X�����ꂽ�e�N�X�`���̈��̏���o�^����
		@param[in] key �o�^�������̏����擾����Ƃ��Ɏg�p����L�[
		@param[in] filePath ��񂪏�����Ă���xml�t�@�C���p�X
		@param[in] texKey ���̏��ƌ��т���A�g���X�����ꂽ�ꖇ�̃e�N�X�`���[�̃L�[
		@author Kyohei Okawa
		*/
		void RegisterTexturePackInfo(wstring key, wstring filePath, wstring texKey);

		/*!
		@brief �w�肳�ꂽ�A�g���X�������擾����
		@param[in] key �o�^���ɐݒ肵���L�[
		@author Kyohei Okawa
		*/
		unordered_map<wstring, TexturePackInfo> GetTexturePackdInfo(wstring key)
		{
			return m_PackdTextureInfo.at(key);
		}

		/*!
		@brief ���ׂẴA�g���X�������擾����
		@author Kyohei Okawa
		*/
		unordered_map<wstring, unordered_map<wstring, TexturePackInfo>> GetPackdTextureInfo()
		{
			return m_PackdTextureInfo;
		}

		vector<shared_ptr<StageSelectStage>> GetStageSelectStages() {
			return m_StageSelectStages;}
	};

}

//end basecross
