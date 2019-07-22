/*!
@file Sprite.h
@brief スプライト
@author Kyohei Okawa
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	struct TexturePackInfo;

/** @addtogroup FairyLead
*   @brief FairyLead独自のプログラム
*/
/*@{*/

	/*!
	 *@struct SpriteAnimInfo
	 *@brief スプライトの連番アニメーション情報
	 *@author Kyohei Okawa
	*/
	struct SpriteAnimInfo
	{
		//!１枚の絵を切り替える時間(秒)
		float timeInterval;
		//!切り替える時間をカウントする用
		float timeCount;
		//!使用しているテクスチャのキー
		wstring texKey;
		//!テクスチャアトラス情報のキー
		wstring xmlKey;
		//!現在何番目の画像か
		int nowIndex;

		SpriteAnimInfo() :
			nowIndex(0),
			timeCount(0)
		{

		}
	};

	/*!
	 *@class Sprite
	 *@brief スプライト基底クラス
	 *@author Kyohei Okawa
	*/
	class Sprite : public GameObject
	{
	public:
		/*!
		 *@enum BlendState
		 *@brief ブレンドステート
		 *@author Kyohei Okawa
		*/
		enum class BlendState
		{
			//!不透明
			Opaque,
			//!透明
			Trace,
			//!加算
			Additive
		};
	protected:
		//!スプライトのメッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		//!頂点情報のバックアップ
		vector<VertexPositionColorTexture> m_BackupVertices;
		//!使用するテクスチャのキー
		wstring m_TextureResName;
		//!大きさ
		Vec2 m_Scale;
		//!ラジアン回転(z軸)
		float m_Rotation;
		//!位置
		Vec2 m_Position;
		//!X方向に画像を何回タイリングするか
		UINT m_XWrap;
		//!Y方向に画像を何回タイリングするか
		UINT m_YWrap;
		//!スプライトに加算する色
		Col4 m_Emissive;
		//!スプライトに乗算する色
		Col4 m_Diffuse;
		//!ブレンドステート
		BlendState m_BlendState;

		//!連番アニメーション情報
		SpriteAnimInfo m_AnimInfo;
		//!アニメーションするか
		bool m_IsAnimated;
		//!フェードアウト中であるか
		bool m_IsFadeOut;
		//!フェードアウトに掛ける時間
		float m_FadeOutInterval;
		//!フェードアウトの時間をカウントするための物
		float m_FadeOutCount;
	public:
		/*!
		 *@brief コンストラクタ
		 *@param[in] stagePtr 追加するステージのポインタ
		 *@param[in] textureResName 使用するテクスチャのキー
		 *@param[in] startScale 最初の大きさ
		 *@param[in] startRot 最初の回転角（ラジアン）
		 *@param[in] startPos 最初の位置(スクリーンの座標)
		 *@param[in] xWrap X方向に画像を何回タイリングするか
		 *@param[in] yWrap Y方向に画像を何回タイリングするか
		*/
		Sprite(const shared_ptr<Stage>& stagePtr,
			const wstring& textureResName,
			const Vec2& startScale,
			float startRot,
			const Vec2& startPos,
			UINT xWrap, UINT yWrap);

		//!デストラクタ
		virtual ~Sprite() {}
		/*!
		@brief バックアップ頂点の修正(仮想関数)。派生クラスは独自の頂点初期修正を実装
		@return	なし
		*/
		virtual void AdjustVertex() {}
		/*!
		@brief 頂点の変更(仮想関数)。派生クラスは独自の頂点変更を実装
		@param[in]	ElapsedTime	ターン時間
		@param[out]	vertices	マップされた頂点データ
		@return	なし
		*/
		virtual void UpdateVertex(float elapsedTime, VertexPositionColorTexture* vertices);

		/*!
		 *@brief 使用しているテクスチャのキーを取得する
		 *@return 使用しているテクスチャのキー
		*/
		const wstring& GetTextureResName() const
		{
			return m_TextureResName;
		}

		/*!
		 *@brief 使用するテクスチャのキーを変更する
		 *@param[in] textureResName テクスチャのキー
		 *@return なし
		*/
		void SetTextureResName(const wstring& textureResName)
		{
			m_TextureResName = textureResName;
		}

		/*!
		 *@brief 現在のブレンドステートを取得する
		 *@return 現在のブレンドステート
		*/
		const BlendState& GetBlendState() const
		{
			return m_BlendState;
		}

		/*!
		 *@brief ブレンドステートを変更する
		 *@param[in] blendState ブレンドステート
		 *@return なし
		*/
		void SetBlendState(BlendState blendState)
		{
			m_BlendState = blendState;
		}

		/*!
		 *@brief スプライトに加算する色を取得する
		 *@return スプライトに加算する色
		*/
		const Col4& GetEmissive() const
		{
			return m_Emissive;
		}

		/*!
		 *@brief スプライトに加算する色を変更する
		 *@param[in] col スプライトに加算する色
		 *@return なし
		*/
		void SetEmissive(const Col4& col)
		{
			m_Emissive = col;
		}

		/*!
		*@brief スプライトに乗算する色を取得する
		*@return スプライトに乗算する色
		*/
		const Col4& GetDiffuse() const
		{
			return m_Diffuse;
		}

		/*!
		*@brief スプライトに乗算する色を変更する
		*@param[in] col スプライトに乗算する色
		*@return なし
		*/
		void SetDiffuse(const Col4& col)
		{
			m_Diffuse = col;
		}

		/*!
		 *@brief 現在の位置を取得する
		 *@return 現在の位置
		*/
		const Vec2& GetPosition() const
		{
			return m_Position;
		}

		/*!
		 *@brief 現在の位置を変更する
		 *@param[in] pos 設定する位置
		 *@return なし
		*/
		void SetPosition(const Vec2 pos)
		{
			m_Position = pos;
		}

		/*!
		 *@brief 現在の大きさを取得する
		 *@return 現在の大きさ
		*/
		const Vec2& GetScale() const
		{
			return m_Scale;
		}

		/*!
		 *@brief 大きさを変更する
		 *@param[in] scale 設定する大きさ
		 *@return なし
		*/
		void SetScale(const Vec2 scale)
		{
			m_Scale = scale;
		}

		/*!
		@brief アトラス情報からUV座標を計算しUVを変更する
		@param[in] xmlKey 登録したアトラス情報(XML)のキー
		@param[in] texname テクスチャアトラス内の画像の名前
		@return なし
		*/
		void SelectTextureUVFromPackInfo(wstring xmlKey,wstring texname);

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;

		/*!
		 *@brief 連番アニメーションを再生する
		 *@param[in] texKey 使用するテクスチャのキー
		 *@param[in] xmlKey アトラス情報のxmlキー
		 *@param[in] animIntervalSec 一枚の画像を切り替えるのにかける時間
		 *@return なし
		 *@details png画像だけ対応で、アトラス情報にはアニメーションの順番0~N.pngのようにして登録する</br>
		           0~Nのループ再生にだけ対応
		*/
		void StartAnimation(wstring texKey, wstring xmlKey, float animIntervalSec);
		/*!
		 *@brief 連番アニメーションを停止する
		 *@return なし
		*/
		void StopAnimation()
		{
			m_IsAnimated = false;
		}

		/*!
		 *@brief スプライトをフェードアウトさせる
		 *@param[in] outTime フェードアウトにかける時間
		 *@return なし
		 *@details フェードアウト後にこのオブジェクトを削除しない
		*/
		void FadeOut(float outTime);
	};
/*@}*/
}