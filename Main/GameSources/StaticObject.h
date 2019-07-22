#pragma once
#include "stdafx.h"

namespace basecross
{
	class StaticObject : public OurGameObject
	{
	public:
		// Transform�̏������\����
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
		// �\�z
		virtual void OnCreate() override
		{}
		// �X�V
		virtual void OnUpdate() override
		{}
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override
		{}

	};

	//--------------------------------------------------------------------------------------
	///	�ǃX�v���C�g
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
		// �\�z
		virtual void OnCreate() override;
		// �X�V
		virtual void OnUpdate()override {}
	};

	//--------------------------------------------------------------------------------------
	///	�X�N���[���X�v���C�g
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
	///	�A�j���X�v���C�g
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
		// �\�z
		virtual void OnCreate() override;
		// �X�V
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
	//Aroow�X�v���C�g
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
		// �\�z
		virtual void OnCreate() override;
		// �X�V
		virtual void OnUpdate()override;
	};

	//--------------------------------------------------------------------------------------
	///	�X�R�A�\���̃X�v���C�g
	//--------------------------------------------------------------------------------------
	class ScoreSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
		float m_Score;
		//����
		UINT m_NumberOfDigits;
		//�o�b�N�A�b�v���_�f�[�^
		vector<VertexPositionTexture> m_BackupVertices;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	NumberOfDigits	����
		@param[in]	TextureKey	�e�N�X�`���L�[
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartScale	�����X�P�[��
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		ScoreSprite(const shared_ptr<Stage>& StagePtr, UINT NumberOfDigits,
			const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);

		virtual ~ScoreSprite() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�R�A�̃Z�b�g
		@param[in]	f	�l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetScore(float f) {
			m_Score = f;
		}

		virtual void OnCreate() override;

		virtual void OnUpdate()override;
	};

	//--------------------------------------------------------------------------------------
	//�^�C�g���̃A�j���[�V����
	//--------------------------------------------------------------------------------------
	class SpriteStudioAnim : public SS5ssae 
	{
		float m_Span;
	public:
		//�\�z�Ə���
		SpriteStudioAnim(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir,const wstring& ssae = L"TitleA.ssae",const wstring& animName = L"TitleAnime");
		virtual ~SpriteStudioAnim();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate()override;
		//�A�N�Z�T
		void AnimeChangeMotion(const wstring& Key, bool looped);

	};


	//--------------------------------------------------------------------------------------
	///	SelectMojiModel
	//--------------------------------------------------------------------------------------
	class SelectMojiModel : public GameObject
	{
		Vec3 m_StartPos;
	public:
		//�\�z�Ɣj��
		SelectMojiModel(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SelectMojiModel();
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	SelectRingModel
	//--------------------------------------------------------------------------------------
	class SelectRingModel : public GameObject
	{
		Vec3 m_StartPos;
	public:
		//�\�z�Ɣj��
		SelectRingModel(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SelectRingModel();
		//������
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