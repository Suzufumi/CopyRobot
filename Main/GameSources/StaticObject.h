#pragma once
#include "stdafx.h"

namespace basecross
{
	class StaticObject : public OurGameObject
	{
	public:
		// Transformの初期化構造体
		struct InitializeParam
		{
			Vec3 startPos;		// Position
			Quat startQuat;		// Quaternion
			Vec3 startScale;		// Scale
		};
	protected:
		shared_ptr<Rigidbody> m_rigid;
		InitializeParam m_initParam;
	public:
		StaticObject(const shared_ptr<Stage>& stagePtr, InitializeParam& initParam)
			: OurGameObject(stagePtr), m_initParam(initParam)
		{}
		virtual ~StaticObject()		
		{}
		// 構築
		virtual void OnCreate() override
		{}
		// 更新
		virtual void OnUpdate() override
		{}
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override
		{}

	};

	//--------------------------------------------------------------------------------------
	///	壁スプライト
	//--------------------------------------------------------------------------------------
	class WallSprite : public GameObject
	{
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
	public:

		WallSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		virtual ~WallSprite();
		// 構築
		virtual void OnCreate() override;
		// 更新
		virtual void OnUpdate()override {}
	};

	//--------------------------------------------------------------------------------------
	///	スクロールスプライト
	//--------------------------------------------------------------------------------------
	class ScrollSprite : public GameObject 
	{
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
		float m_TotalTime;
		vector<VertexPositionTexture> m_BackupVertices;
	public:

		ScrollSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		virtual ~ScrollSprite();
		virtual void OnCreate() override;
		virtual void OnUpdate()override;
	};

	//--------------------------------------------------------------------------------------
	///	アニメスプライト
	//--------------------------------------------------------------------------------------
	class AnimeSprite : public GameObject 
	{
		bool m_Trace;
		Vec2 m_StartScale;
		Vec2 m_StartPos;
		wstring m_TextureKey;
		float m_TotalTime;
		bool m_Selected;
	public:

		AnimeSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec2& StartPos);
		virtual ~AnimeSprite();
		// 構築
		virtual void OnCreate() override;
		// 更新
		virtual void OnUpdate()override;
		bool IsSelect() const 
		{
			return m_Selected;
		}
		void SetSelect(bool b)
		{
			m_Selected = b;
			m_TotalTime = 0.0f;
		}
	};



	//--------------------------------------------------------------------------------------
	//Aroowスプライト
	//--------------------------------------------------------------------------------------

	class ArrowSprite : public GameObject
	{
		bool m_Trace;
		Vec2 m_StartScale;
		Vec2 m_StartPos;
		wstring m_TextureKey;
		float m_TotalTime;
	public:

		ArrowSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec2& StartPos);
		virtual ~ArrowSprite();
		// 構築
		virtual void OnCreate() override;
		// 更新
		virtual void OnUpdate()override;
	};

	//--------------------------------------------------------------------------------------
	///	スコア表示のスプライト
	//--------------------------------------------------------------------------------------
	class ScoreSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
		float m_Score;
		//桁数
		UINT m_NumberOfDigits;
		//バックアップ頂点データ
		vector<VertexPositionTexture> m_BackupVertices;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	NumberOfDigits	桁数
		@param[in]	TextureKey	テクスチャキー
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		ScoreSprite(const shared_ptr<Stage>& StagePtr, UINT NumberOfDigits,
			const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);

		virtual ~ScoreSprite() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief スコアのセット
		@param[in]	f	値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetScore(float f) {
			m_Score = f;
		}

		virtual void OnCreate() override;

		virtual void OnUpdate()override;
	};

	//--------------------------------------------------------------------------------------
	//タイトルのアニメーション
	//--------------------------------------------------------------------------------------
	class SpriteStudioAnim : public SS5ssae 
	{
		float m_Span;
	public:
		//構築と消滅
		SpriteStudioAnim(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir,const wstring& ssae = L"TitleA.ssae",const wstring& animName = L"TitleAnime");
		virtual ~SpriteStudioAnim();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate()override;
		//アクセサ
		void AnimeChangeMotion(const wstring& Key, bool looped);

	};


	//--------------------------------------------------------------------------------------
	///	SelectMojiModel
	//--------------------------------------------------------------------------------------
	class SelectMojiModel : public GameObject
	{
		Vec3 m_StartPos;
	public:
		//構築と破棄
		SelectMojiModel(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SelectMojiModel();
		//初期化
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	SelectRingModel
	//--------------------------------------------------------------------------------------
	class SelectRingModel : public GameObject
	{
		Vec3 m_StartPos;
	public:
		//構築と破棄
		SelectRingModel(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SelectRingModel();
		//初期化
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	StageSelectSquare
	//--------------------------------------------------------------------------------------
	class Square : public GameObject
	{
	public:
		struct InitParam
		{
			Vec3 m_Position;
			Vec3 m_Scale;
			Vec3 m_Rotate;
			wstring m_TexKey;
			float m_AspectWdivH;

			InitParam() :
				m_AspectWdivH(1.0f)
			{

			}
		};

	private:
		Square::InitParam m_InitParam;
		vector<VertexPositionColorTexture> m_BackupVertices;
		shared_ptr<MeshResource> m_SquareMesh;
		float m_Value;

	public:
		Square(const shared_ptr<Stage>& stagePtr, Square::InitParam initParam);
		virtual ~Square() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void SetMeshVertexPositionUvX(float value);
		float GetValue() { return m_Value; }
		Vec3 GetStartPosition() { return m_InitParam.m_Position; }
	};

	class PostSquare : public GameObject
	{
	public:
		struct InitParam
		{
			Vec3 m_Position;
			Vec3 m_Scale;
			Vec3 m_Rotate;
			weak_ptr<PostEffectRenderTarget> m_RenderTarget;
			float m_AspectWdivH;
			int m_Num;

			InitParam() :
				m_AspectWdivH(1.0f),
				m_Num(1)
			{

			}
		};

	private:
		PostSquare::InitParam m_InitParam;
		vector<VertexPositionColorTexture> m_BackupVertices;
		shared_ptr<MeshResource> m_SquareMesh;
		shared_ptr<Square> m_FrameSquare;

	public:
		PostSquare(const shared_ptr<Stage>& stagePtr, PostSquare::InitParam initParam);
		virtual ~PostSquare() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnUpdate2()override;
		virtual void OnDraw() override;
		void SetMeshVertexPositionUvX(float value);
		Vec3 GetStartPosition() { return m_InitParam.m_Position; }
	};
}