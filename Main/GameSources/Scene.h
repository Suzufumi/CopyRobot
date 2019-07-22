/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

namespace basecross
{
	class Transition;
	class StageSelectStage;

	/*!
	*@struct PackedTextureInfo
	*@brief TexturePackerで出力した画像の情報
	*@author Kyohei Okawa
	*/
	struct PackedTextureInfo
	{
		//!登録したテクスチャのキー
		wstring textureKey;
		//!テクスチャの幅と高さ(ピクセル)
		int width, height;
	};

	/*!
	*@struct TexturePackInfo
	*@brief TexturePackerで出力したアトラステクスチャの中の一つ一つの画像の情報
	*@author Kyohei Okawa
	*/
	struct TexturePackInfo
	{
		//!画像の名前
		wstring name;
		//!テクスチャの中の座標(ピクセル) 左上の位置
		int x, y;
		//!画像の幅と高さ(ピクセル)
		int width, height;

		//!元の一枚のテクスチャの情報
		PackedTextureInfo packedTextureInfo;
	};

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
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
		@brief リソース取得
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void CreateResources();

		//!TexturePackerで出力されたxmlの情報を登録する
		unordered_map<wstring, unordered_map<wstring, TexturePackInfo>> m_PackdTextureInfo;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;

		virtual void OnUpdate() override;

		virtual void OnDraw() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;

		shared_ptr<Transition> GetTransition() { return m_Transition; }

		//現在セレクトされているステージナンバー
		int m_SrageNum;
		//ステージナンバーのアクセサ
		int GetStageNum() const 
		{
			return m_SrageNum;
		}
		void SetStageNum(int i)
		{
			m_SrageNum = i;
		}
		//GameStageからクリアするまでのTimeを測る。
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
		@brief アトラス化されたテクスチャの一つ一つの情報を登録する
		@param[in] key 登録した一塊の情報を取得するときに使用するキー
		@param[in] filePath 情報が書かれているxmlファイルパス
		@param[in] texKey この情報と結びつけるアトラス化された一枚のテクスチャーのキー
		@author Kyohei Okawa
		*/
		void RegisterTexturePackInfo(wstring key, wstring filePath, wstring texKey);

		/*!
		@brief 指定されたアトラス化情報を取得する
		@param[in] key 登録時に設定したキー
		@author Kyohei Okawa
		*/
		unordered_map<wstring, TexturePackInfo> GetTexturePackdInfo(wstring key)
		{
			return m_PackdTextureInfo.at(key);
		}

		/*!
		@brief すべてのアトラス化情報を取得する
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
