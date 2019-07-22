/*!
* @file PostEffectRenderTarget.h
* @brief 描画後にかけるエフェクト
* @author Kyohei Okawa
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
	 *@class PostEffectRenderTarget
	 *@brief ポストエフェクト用のレンダーターゲット
	 *@author Kyohei Okawa
	*/
	class PostEffectRenderTarget : public RenderTarget
	{
	public:
		PostEffectRenderTarget();
		virtual ~PostEffectRenderTarget();

		/*!
		 *@brief このレンダーターゲットに描画された情報をシェーダーリソースビューとして返す
		 *@return このレンダーターゲットに描画されたシェーダーリソースビュー
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
	 *@brief 描画後画面を何色にするか
	 *@author Kyohei Okawa
	*/
	enum PostEffectColor
	{
		//!描画された色をそのままにする
		Color,
		//!白黒にする
		GrayScale,
		//!セピア色にする
		Sepia
	};

	/*!
	 *@class PostEffectSprite
	 *@brief オフスクリーンレンダリングされたものを描画するためのスプライト
	 *@author Kyohei Okawa
	*/
	class PostEffectSprite : public GameObject
	{
		//!メッシュリソース
		shared_ptr<MeshResource> m_MeshResource;
		//!何色にして描画するか
		PostEffectColor m_PostEffectColor;

		//!表示するかしないか
		bool m_IsVisible;
		//!出力するピクセル色に掛ける色
		Col4 m_Diffuse;

		weak_ptr<PostEffectRenderTarget> m_RenderTarget;
	public:
		/*!
		 *@brief コンストラクタ
		 *@param[in] stage このオブジェクトを追加するステージのポインタ
		*/
		PostEffectSprite(const shared_ptr<Stage>& stage,const shared_ptr<PostEffectRenderTarget> renderTarget);
		//!デストラクタ
		~PostEffectSprite() {}

		virtual void OnCreate() override;
		virtual void OnDraw() override;

		/*!
		 *@brief 描画する際の色を変更する
		 *@param[in] postEffectColor 描画する際の色
		 *@return なし
		*/
		void ChangePostEffectColor(PostEffectColor postEffectColor)
		{
			m_PostEffectColor = postEffectColor;
		}

		/*!
		 *@brief 現在表示されている状態か取得する
		 *@return true 表示されている
		 *@return false　非表示になっている
		*/
		bool GetIsVisible()
		{
			return m_IsVisible;
		}

		/*!
		 *@brief 表示、非表示を指定する
		 *@param[in] visible trueの場合が表示
		 *@return なし
		*/
		void SetIsVisible(bool visible)
		{
			m_IsVisible = visible;
		}

		/*!
		 *@brief 出力するピクセル色に掛ける色を取得する
		 *@return 出力するピクセル色に掛ける色
		*/
		Col4 GetDiffuse()
		{
			return m_Diffuse;
		}

		/*!
		 *@brief 出力するピクセル色に掛ける色を指定する
		 *@param[in] diffuse 出力するピクセル色に掛ける色
		 *@return なし
		*/
		void SetDiffuse(Col4 diffuse)
		{
			m_Diffuse = diffuse;
		}
	};
/*@}*/
}