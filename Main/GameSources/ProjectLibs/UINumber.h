/*!
@file UINumber.h
@brief 数字スプライト
@author Kyohei Okawa
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
/** @addtogroup FairyLead
*   @brief FairyLead独自のプログラム
*/
/*@{*/

	/*!
	 *@class UINumber
	 *@brief 一けただけの整数を表示するスプライト
	 *@author Kyohei Okawa
	*/
	class UINumber : public Sprite
	{
		//!表示する数字(0～9)
		int m_Number;
		//!表示するかしないか
		bool m_IsVisible;
	public:
		/*!
		 *@brief コンストラクタ
		 *@param[in] stagePtr このオブジェクトを追加するステージ
		 *@param[in] textureResName 0～9の数字が並んだテクスチャのキー
		 *@param[in] startScale 初期の大きさ
		 *@param[in] startRot 初期の回転
		 *@param[in] startPos 初期位置
		 *@param[in] number 表示する数字(0～9いずれかの整数)
		*/
		UINumber(const shared_ptr<Stage>& stagePtr,
			const wstring& textureResName,
			const Vec2& startScale,
			float startRot,
			const Vec2& startPos,
			int number);
		//!デストラクタ
		~UINumber() {}

		virtual void AdjustVertex() override;

		virtual void OnCreate() override;
		virtual void OnDraw() override;

		//!表示している数字を取得する
		int GetNumber()
		{
			return m_Number;
		}
		//!表示(true),非表示(false)を設定する
		void SetVisible(bool visible)
		{
			m_IsVisible = visible;
		}
		//!表示する数字を変更する(0～9)
		void ChangeNumber(int number);
		//!色を変更する
		void ChangeColor(Col4 color);
	};

	/*!
	 *@class UINumbers
	 *@brief 複数桁の整数を表示するスプライト
	 *@author Kyohei Okawa
	*/
	class UINumbers : public GameObject
	{
		//!0～9の数字が並んだテクスチャのキー
		wstring m_TextureResName;
		//!一桁毎のの数字スプライトの初期の大きさ
		Vec2 m_StartNumScale;
		//!初期位置1の位が始点になる
		Vec2 m_StartPos;
		//!何桁表示するか
		int m_NumOfDigits;
		//!表示する整数
		int m_Number;
		//!0032のようになったときに0の部分を表示(true)、非表示(false)にするか
		bool m_ShouldShowZero;

		//!一桁ずつのスプライトのベクター
		vector<shared_ptr<UINumber>> m_NumSprites;
	public:
		/*!
		 *@brief コンストラクタ
		 *@param[in] stagePtr このオブジェクトを追加するステージ
		 *@param[in] textureResName 0～9の数字が並んだテクスチャのキー
		 *@param[in] startNumScale 一桁毎のの数字スプライトの初期の大きさ
		 *@param[in] startPos 初期位置(１の位が始点)
		 *@param[in] numOfDigits 何桁まで表示するか
		 *@param[in] number 表示する整数
		 *@param[in] shouldShowZero 0032のようになったときに0の部分を表示(true)、非表示(false)にするか
		*/
		UINumbers(const shared_ptr<Stage>& stagePtr,
			const wstring& textureResName,
			const Vec2& startNumScale,
			const Vec2& startPos,
			int numOfDigits,
			int number,
			bool shouldShowZero
		);
		//!デストラクタ
		~UINumbers() {}

		virtual void OnCreate()override;

		//!表示している数字を取得する
		int GetNumber()
		{
			return m_Number;
		}
		//!一桁の数字スプライトの大きさを取得する
		Vec2 GetScale()
		{
			return m_StartNumScale;
		}
		//!１の位の数字スプライトの位置を取得する
		Vec2 GetPosition()
		{
			return m_StartPos;
		}

		//!表示する整数を変更する
		void ChangeNumber(int number);
		//!すべての桁の数字スプライトの色を変更する
		void ChangeColor(Col4 color);
		//!１の位のスプライトの位置を始点として位置を変更する
		void ChangePosition(Vec2 pos);
		//!すべての数字の大きさを変更し、位置も合わせる
		void ChangeScale(Vec2 scale);
		//!表示色を取得する
		Col4 GetColor();
		//!0032のようになったときに0の部分を表示(true)、非表示(false)にするかを設定する
		void SetShouldShowZero(bool b);

		//!このクラスを破棄する場合はこの関数を呼ぶ
		void Destroy();
	};

/*@}*/
}