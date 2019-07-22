/*!
@file Transition.h
@brief �V�[���J�ڂ̃t�F�[�h�ɂ���������
@author Kyohei Okawa
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
	 *@enum TransitionState
	 *@brief �J�ڂ̎��
	 *@author Kyohei Okawa
	*/
	enum TransitionState
	{
		//!�t�F�[�h�C��
		FadeIn,
		//!�t�F�[�h�A�E�g
		FadeOut
	};

	/*!
	 *@class TransitionObj
	 *@brief�@�ǂ̂悤�ɑJ�ڂ����邩���`������N���X
	 *@author Kyohei Okawa
	*/
	class TransitionObj : public ObjectInterface, public ShapeInterface
	{
	protected:
		//!�J�ڂɂ����鎞��
		float m_TransitionSec;
		//!�J�ڂ̏�ԁ@�t�F�[�h�C�����A�E�g��
		TransitionState m_TransitionState;

		//!�J�ڂɂ����������Ԃ��J�E���g����
		float m_CountSec;
		//!�J�ڂ��I��������
		bool m_IsFinished;

		//!���_���
		vector<VertexPositionColorTexture> m_Vertices;
		//!���b�V�����\�[�X
		shared_ptr<MeshResource> m_MeshResource;
	public:
		/*!
		 *@brief �R���X�g���N�^
		 *@param[in] transSec �J�ڂɂ����鎞��
		 *@param[in] transitionState �t�F�[�h�C�����A�E�g��
		*/
		TransitionObj(float transSec,TransitionState transitionState):
			m_TransitionSec(transSec),
			m_TransitionState(transitionState),
			m_CountSec(0),
			m_IsFinished(false)
		{}
		//!�f�X�g���N�^
		virtual ~TransitionObj() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override {}

		/*!
		 *@brief �J�ڂ��I���������ǂ������擾����
		 *@return true �J�ڂ��I������
		 *@return false �J�ڂ��I�����Ă��Ȃ�
		*/
		bool GetIsFinishedTransition()
		{
			return m_IsFinished;
		}

		/*!
		 *@brief ���Ɖ��b�őJ�ڂ��I�����邩���擾����
		 *@return ���Ɖ��b�őJ�ڂ��I�����邩
		*/
		float GetLeftFadeTime()
		{
			return m_TransitionSec - m_CountSec;
		}
	};

	/*!
	 *@class NormalFadeTransition
	 *@brief�@�P�F�t�F�[�h
	 *@author Kyohei Okawa
	*/
	class NormalFadeTransition : public TransitionObj
	{
		//!�t�F�[�h�̐F
		Col4 m_FadeColor;
	public:
		/*!
		 *@brief �R���X�g���N�^
		 *@param[in] transSec �J�ڂɂ����鎞��
		 *@param[in] transitionState �t�F�[�h�C�����A�E�g��
		 *@param[in] fadeColor �t�F�[�h�̐F(�f�t�H���g�Ŕ�)
		*/
		NormalFadeTransition(float transSec,TransitionState transitionState,Col4 fadeColor = Col4(1.0f)):
			TransitionObj(transSec,transitionState),
			m_FadeColor(fadeColor)
		{}

		virtual void OnDraw() override;
	};

	/*!
	 *@class FadeMaskTransition
	 *@brief �}�X�N�t���t�F�[�h
	 *@author Kyohei Okawa
	*/
	class FadeMaskTransition : public TransitionObj
	{
		//!�}�X�N�e�N�X�`���̃L�[
		wstring m_FadeTex;
		//!�e�N�X�`�����\�[�X
		shared_ptr<TextureResource> m_TextureResource;
		//!�t�F�[�h�̐F
		Col4 m_FadeColor;
	public:
		/*!
		 *@brief �R���X�g���N�^
		 *@brief �R���X�g���N�^
		 *@param[in] transSec �J�ڂɂ����鎞��
		 *@param[in] fadeTex �}�X�N�e�N�X�`���̃L�[
		 *@param[in] transitionState �t�F�[�h�C�����A�E�g��
		 *@param[in] fadeColor �t�F�[�h�̐F(�f�t�H���g�ō�)
		*/
		FadeMaskTransition(float transSec, wstring fadeTex, TransitionState transitionState,Col4 fadeColor = Col4(0,0,0,1)) :
			TransitionObj(transSec, transitionState),
			m_FadeTex(fadeTex),
			m_FadeColor(fadeColor)
		{}

		virtual void OnCreate() override;
		virtual void OnDraw() override;
	};

	class TransitionNoneState;
	class OnlyFadeOutState;
	class FadeInState;
	class FadeOutState;

	/*!
	 *@class Transition
	 *@brief ���ۂɃV�[���J�ڂ��s���N���X
	 *@author Kyohei Okawa
	*/
	class Transition : public ObjectInterface, public ShapeInterface
	{
		friend TransitionNoneState;
		friend OnlyFadeOutState;
		friend FadeInState;
		friend FadeOutState;

		//!�t�F�[�h�C���̃I�u�W�F�N�g
		shared_ptr<TransitionObj> m_TransitionObjIn;
		//!�t�F�[�h�A�E�g�̃I�u�W�F�N�g
		shared_ptr<TransitionObj> m_TransitionObjOut;
		//!�t�F�[�h�A�E�g�I�����ɑ��郁�b�Z�[�W
		wstring m_Msg;
		//!�t�F�[�h�A�E�g��ɍs������
		std::function<void()> m_AfterOutFunc;
		//!�t�F�[�h�A�E�g�I����A�t�F�[�h�C�������ɂŎ~�߂邩
		bool m_IsStopFadeOut;

		//!�C�x���g�𑗂�Ȃ��t�F�[�h�A�E�g�����Ă���Ԃ�true
		//!�I������ꍇ��false
		bool m_IsPlayedOnlyFadeOut;

		//!�t�F�[�h�Ŏg�p����X�e�[�g�}�V�[��
		shared_ptr<StateMachine<Transition>> m_StateMachine;
	public:
		//!�R���X�g���N�^
		Transition();
		//!�f�X�g���N�^
		virtual ~Transition() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;

		//!�X�e�[�g�}�V�[�����擾����
		shared_ptr<StateMachine<Transition>> GetStateMachine()
		{
			return m_StateMachine;
		}

		/*!
		 *@brief OnlyFadeOutState�Ɋւ���Ԃ�Ԃ�
		 *@return true OnlyFadeOutState�ɓ�������ԂŁA�t�F�[�h�A�E�g���I�����Ă��Ȃ�
		 *@return false OnlyFadeOutState�ɓ����Ă��Ȃ�����OnlyFadeOutState�ɓ����Ă����ԂŃt�F�[�h�A�E�g���I�����Ă���
		*/
		bool GetIsPlayedOnlyFadeOut()
		{
			return m_IsPlayedOnlyFadeOut;
		}

		//!���Ɖ��b�Ńt�F�[�h�C�����I�����邩�𓾂�
		float GetFadeInLeftTime()
		{
			if (m_TransitionObjIn)
			{
				return m_TransitionObjIn->GetLeftFadeTime();
			}

			return 0;
		}

		//!���Ɖ��b�Ńt�F�[�h�A�E�g���I�����邩�𓾂�
		float GetFadeOutLeftTime()
		{
			if (m_TransitionObjOut)
			{
				return m_TransitionObjOut->GetLeftFadeTime();
			}

			return 0;
		}

		/*!
		 *@brief ���݃t�F�[�h�A�E�g�����ǂ����擾����
		 *@return true�@�t�F�[�h�A�E�g���ł���
		 *@return false �t�F�[�h�A�E�g���ł͂Ȃ�
		*/
		bool GetNowFadeOut();

		/*!
		 *@brief ���݃t�F�[�h�����ǂ����𓾂�
		 *@return true �t�F�[�h��
		 *@return false �t�F�[�h���ł͂Ȃ�
		 */
		bool GetIsFadeNow();
		/*!
		 *@brief �t�F�[�h�C���A�A�E�g�p�̃I�u�W�F�N�g��j������
		 *@return �Ȃ�
		*/
		void ClearTransitionObj();

		/*!
		 *@brief �t�F�[�h�C���A�A�E�g�̑J�ڂ����s����(�t�F�[�h�A�E�g���Scene�Ƀ��b�Z�[�W�𑗂�)
		 *@param[in] inTransition �t�F�[�h�C���p��TransitionObj
		 *@param[in] outTransition �t�F�[�h�A�E�g�p��TransitionObj
		 *@param[in] wstrMsg �t�F�[�h�A�E�g���Scene�ɑ��郁�b�Z�[�W
		 *@return �Ȃ�
		 *@details <b>�g�p��</b>\n
		 *			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn);\n
		 *			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut);\n
		 *			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, L"ToMainStage");\n
		*/
		void PlayInOutTransitionWithMsg(shared_ptr<TransitionObj> inTransition, shared_ptr<TransitionObj> outTransition, wstring wstrMsg);

		/*!
		*@brief �t�F�[�h�C���A�A�E�g�̑J�ڂ����s����
		*@param[in] inTransition �t�F�[�h�C���p��TransitionObj
		*@param[in] outTransition �t�F�[�h�A�E�g�p��TransitionObj
		*@return �Ȃ�
		*@details <b>�g�p��</b>\n
		*			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn);\n
		*			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut);\n
		*			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransition(transIn, transOut);\n
		*/
		void PlayInOutTransition(shared_ptr<TransitionObj> inTransition, shared_ptr<TransitionObj> outTransition);

		/*!
		*@brief �t�F�[�h�C���A�A�E�g�̑J�ڂ����s����(�t�F�[�h�A�E�g��ɓn���ꂽ�������s��)
		*@param[in] inTransition �t�F�[�h�C���p��TransitionObj
		*@param[in] outTransition �t�F�[�h�A�E�g�p��TransitionObj
		*@param[in] action �t�F�[�h�A�E�g��Ɏ��s���鏈��
		*@return �Ȃ�
		*@details <b>�g�p��1�@�����_�����g���ꍇ</b>\n
		*			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn);\n
		*			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut);\n
		*			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, [](){cout << "Hello World"});\n
		*			-------------------------------------------------------------------------------------------------------------------------------------\n
		*		  <b>�g�p��Q�@�֐��R�[���o�b�N����ꍇ</b>\n
		*			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn);\n
		*			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut);\n
		*			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, BC_CALLBACK_0(ClassName::FunctionName));\n
		*/
		void PlayInOutTransitionWithAction(shared_ptr<TransitionObj> inTransition, shared_ptr<TransitionObj> outTransition,std::function<void()> action);

		/*!
		 *@brief �t�F�[�h�A�E�g�݂̂����s����
		 *@param[in] outTransition �t�F�[�h�A�E�g�p��TransitionObj
		 *@param[in] isStop �t�F�[�h�A�E�g�I�����ɉ�ʂ������Ă��Ȃ���ԂŎ~�߂邩
		 *@return �Ȃ�
		 *@details <b>�g�p��</b>\n
		 *			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut);\n
		 *			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayFadeOut(transOut);\n
		*/
		void PlayFadeOut(shared_ptr<TransitionObj> outTransition,bool isStop = false);
		/*!
		 *@brief �t�F�[�h�C���݂̂����s����
		 *@param[in] inTransition �t�F�[�h�C���p��TransitionObj
		 *@return �Ȃ�
		 *@details <b>�g�p��</b>\n
		 *			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn);\n
		 *			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayFadeIn(transOut);\n
		*/
		void PlayFadeIn(shared_ptr<TransitionObj> inTransition);
	};

	/*!
	 *@class TransitionNoneState
	 *@brief �J�ڂ��Ă��Ȃ����
	 *@author Kyohei Okawa
	*/
	class TransitionNoneState : public ObjState<Transition>
	{
		TransitionNoneState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(TransitionNoneState)


		virtual void Enter(const shared_ptr<Transition>& transObj) override {};
		virtual void Execute(const shared_ptr<Transition>& transObj) override {};
		virtual void Exit(const shared_ptr<Transition>& transObj) override {};
	};

	/*!
	*@class FadeInState
	*@brief �t�F�[�h�C�����̏��
	*@author Kyohei Okawa
	*/
	class FadeInState : public ObjState<Transition>
	{
		FadeInState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(FadeInState)

		virtual void Enter(const shared_ptr<Transition>& transObj) override;
		virtual void Execute(const shared_ptr<Transition>& transObj) override;
		virtual void Exit(const shared_ptr<Transition>& transObj) override;
	};

	/*!
	*@class FadeOutState
	*@brief �t�F�[�h�A�E�g���̏��
	*@author Kyohei Okawa
	*/
	class FadeOutState : public ObjState<Transition>
	{
		FadeOutState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(FadeOutState)

		virtual void Enter(const shared_ptr<Transition>& transObj) override;
		virtual void Execute(const shared_ptr<Transition>& transObj) override;
		virtual void Exit(const shared_ptr<Transition>& transObj) override;
	};

	/*!
	*@class OnlyFadeOutState
	*@brief �t�F�[�h�A�E�g���̏�ԂŏI�������Ƃ���PostEvent���Ȃ��X�e�[�g
	*@author Kyohei Okawa
	*/
	class OnlyFadeOutState : public ObjState<Transition>
	{
		OnlyFadeOutState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(OnlyFadeOutState)

		virtual void Enter(const shared_ptr<Transition>& transObj) override;
		virtual void Execute(const shared_ptr<Transition>& transObj) override;
		virtual void Exit(const shared_ptr<Transition>& transObj) override;
	};
/*@}*/
}