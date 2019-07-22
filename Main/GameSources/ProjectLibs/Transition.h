/*!
@file Transition.h
@brief シーン遷移のフェードにかかわるもの
@author Kyohei Okawa
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
/** @addtogroup FairyLead
*  @brief FairyLead独自のプログラム
*/
/*@{*/

	/*!
	 *@enum TransitionState
	 *@brief 遷移の種類
	 *@author Kyohei Okawa
	*/
	enum TransitionState
	{
		//!フェードイン
		FadeIn,
		//!フェードアウト
		FadeOut
	};

	/*!
	 *@class TransitionObj
	 *@brief　どのように遷移させるかを定義する基底クラス
	 *@author Kyohei Okawa
	*/
	class TransitionObj : public ObjectInterface, public ShapeInterface
	{
	protected:
		//!遷移にかける時間
		float m_TransitionSec;
		//!遷移の状態　フェードインかアウトか
		TransitionState m_TransitionState;

		//!遷移にかかった時間をカウントする
		float m_CountSec;
		//!遷移が終了したか
		bool m_IsFinished;

		//!頂点情報
		vector<VertexPositionColorTexture> m_Vertices;
		//!メッシュリソース
		shared_ptr<MeshResource> m_MeshResource;
	public:
		/*!
		 *@brief コンストラクタ
		 *@param[in] transSec 遷移にかける時間
		 *@param[in] transitionState フェードインかアウトか
		*/
		TransitionObj(float transSec,TransitionState transitionState):
			m_TransitionSec(transSec),
			m_TransitionState(transitionState),
			m_CountSec(0),
			m_IsFinished(false)
		{}
		//!デストラクタ
		virtual ~TransitionObj() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override {}

		/*!
		 *@brief 遷移が終了したかどうかを取得する
		 *@return true 遷移が終了した
		 *@return false 遷移が終了していない
		*/
		bool GetIsFinishedTransition()
		{
			return m_IsFinished;
		}

		/*!
		 *@brief あと何秒で遷移が終了するかを取得する
		 *@return あと何秒で遷移が終了するか
		*/
		float GetLeftFadeTime()
		{
			return m_TransitionSec - m_CountSec;
		}
	};

	/*!
	 *@class NormalFadeTransition
	 *@brief　単色フェード
	 *@author Kyohei Okawa
	*/
	class NormalFadeTransition : public TransitionObj
	{
		//!フェードの色
		Col4 m_FadeColor;
	public:
		/*!
		 *@brief コンストラクタ
		 *@param[in] transSec 遷移にかける時間
		 *@param[in] transitionState フェードインかアウトか
		 *@param[in] fadeColor フェードの色(デフォルトで白)
		*/
		NormalFadeTransition(float transSec,TransitionState transitionState,Col4 fadeColor = Col4(1.0f)):
			TransitionObj(transSec,transitionState),
			m_FadeColor(fadeColor)
		{}

		virtual void OnDraw() override;
	};

	/*!
	 *@class FadeMaskTransition
	 *@brief マスク付きフェード
	 *@author Kyohei Okawa
	*/
	class FadeMaskTransition : public TransitionObj
	{
		//!マスクテクスチャのキー
		wstring m_FadeTex;
		//!テクスチャリソース
		shared_ptr<TextureResource> m_TextureResource;
		//!フェードの色
		Col4 m_FadeColor;
	public:
		/*!
		 *@brief コンストラクタ
		 *@brief コンストラクタ
		 *@param[in] transSec 遷移にかける時間
		 *@param[in] fadeTex マスクテクスチャのキー
		 *@param[in] transitionState フェードインかアウトか
		 *@param[in] fadeColor フェードの色(デフォルトで黒)
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
	 *@brief 実際にシーン遷移を行うクラス
	 *@author Kyohei Okawa
	*/
	class Transition : public ObjectInterface, public ShapeInterface
	{
		friend TransitionNoneState;
		friend OnlyFadeOutState;
		friend FadeInState;
		friend FadeOutState;

		//!フェードインのオブジェクト
		shared_ptr<TransitionObj> m_TransitionObjIn;
		//!フェードアウトのオブジェクト
		shared_ptr<TransitionObj> m_TransitionObjOut;
		//!フェードアウト終了時に送るメッセージ
		wstring m_Msg;
		//!フェードアウト後に行う処理
		std::function<void()> m_AfterOutFunc;
		//!フェードアウト終了後、フェードインせずにで止めるか
		bool m_IsStopFadeOut;

		//!イベントを送らないフェードアウトをしている間はtrue
		//!終わった場合はfalse
		bool m_IsPlayedOnlyFadeOut;

		//!フェードで使用するステートマシーン
		shared_ptr<StateMachine<Transition>> m_StateMachine;
	public:
		//!コンストラクタ
		Transition();
		//!デストラクタ
		virtual ~Transition() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;

		//!ステートマシーンを取得する
		shared_ptr<StateMachine<Transition>> GetStateMachine()
		{
			return m_StateMachine;
		}

		/*!
		 *@brief OnlyFadeOutStateに関わる状態を返す
		 *@return true OnlyFadeOutStateに入った状態で、フェードアウトが終了していない
		 *@return false OnlyFadeOutStateに入っていない又はOnlyFadeOutStateに入っている状態でフェードアウトが終了している
		*/
		bool GetIsPlayedOnlyFadeOut()
		{
			return m_IsPlayedOnlyFadeOut;
		}

		//!あと何秒でフェードインが終了するかを得る
		float GetFadeInLeftTime()
		{
			if (m_TransitionObjIn)
			{
				return m_TransitionObjIn->GetLeftFadeTime();
			}

			return 0;
		}

		//!あと何秒でフェードアウトが終了するかを得る
		float GetFadeOutLeftTime()
		{
			if (m_TransitionObjOut)
			{
				return m_TransitionObjOut->GetLeftFadeTime();
			}

			return 0;
		}

		/*!
		 *@brief 現在フェードアウト中かどうか取得する
		 *@return true　フェードアウト中である
		 *@return false フェードアウト中ではない
		*/
		bool GetNowFadeOut();

		/*!
		 *@brief 現在フェード中かどうかを得る
		 *@return true フェード中
		 *@return false フェード中ではない
		 */
		bool GetIsFadeNow();
		/*!
		 *@brief フェードイン、アウト用のオブジェクトを破棄する
		 *@return なし
		*/
		void ClearTransitionObj();

		/*!
		 *@brief フェードイン、アウトの遷移を実行する(フェードアウト後にSceneにメッセージを送る)
		 *@param[in] inTransition フェードイン用のTransitionObj
		 *@param[in] outTransition フェードアウト用のTransitionObj
		 *@param[in] wstrMsg フェードアウト後にSceneに送るメッセージ
		 *@return なし
		 *@details <b>使用例</b>\n
		 *			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn);\n
		 *			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut);\n
		 *			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, L"ToMainStage");\n
		*/
		void PlayInOutTransitionWithMsg(shared_ptr<TransitionObj> inTransition, shared_ptr<TransitionObj> outTransition, wstring wstrMsg);

		/*!
		*@brief フェードイン、アウトの遷移を実行する
		*@param[in] inTransition フェードイン用のTransitionObj
		*@param[in] outTransition フェードアウト用のTransitionObj
		*@return なし
		*@details <b>使用例</b>\n
		*			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn);\n
		*			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut);\n
		*			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransition(transIn, transOut);\n
		*/
		void PlayInOutTransition(shared_ptr<TransitionObj> inTransition, shared_ptr<TransitionObj> outTransition);

		/*!
		*@brief フェードイン、アウトの遷移を実行する(フェードアウト後に渡された処理を行う)
		*@param[in] inTransition フェードイン用のTransitionObj
		*@param[in] outTransition フェードアウト用のTransitionObj
		*@param[in] action フェードアウト後に実行する処理
		*@return なし
		*@details <b>使用例1　ラムダ式を使う場合</b>\n
		*			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn);\n
		*			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut);\n
		*			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, [](){cout << "Hello World"});\n
		*			-------------------------------------------------------------------------------------------------------------------------------------\n
		*		  <b>使用例２　関数コールバックする場合</b>\n
		*			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn);\n
		*			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut);\n
		*			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, BC_CALLBACK_0(ClassName::FunctionName));\n
		*/
		void PlayInOutTransitionWithAction(shared_ptr<TransitionObj> inTransition, shared_ptr<TransitionObj> outTransition,std::function<void()> action);

		/*!
		 *@brief フェードアウトのみを実行する
		 *@param[in] outTransition フェードアウト用のTransitionObj
		 *@param[in] isStop フェードアウト終了時に画面が見えていない状態で止めるか
		 *@return なし
		 *@details <b>使用例</b>\n
		 *			auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeOut);\n
		 *			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayFadeOut(transOut);\n
		*/
		void PlayFadeOut(shared_ptr<TransitionObj> outTransition,bool isStop = false);
		/*!
		 *@brief フェードインのみを実行する
		 *@param[in] inTransition フェードイン用のTransitionObj
		 *@return なし
		 *@details <b>使用例</b>\n
		 *			auto transIn = ObjectFactory::Create<NormalFadeTransition>(0.5f, TransitionState::FadeIn);\n
		 *			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayFadeIn(transOut);\n
		*/
		void PlayFadeIn(shared_ptr<TransitionObj> inTransition);
	};

	/*!
	 *@class TransitionNoneState
	 *@brief 遷移していない状態
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
	*@brief フェードイン中の状態
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
	*@brief フェードアウト中の状態
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
	*@brief フェードアウト中の状態で終了したときにPostEventしないステート
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