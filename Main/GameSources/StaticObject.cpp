#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	///	壁模様のスプライト
	//--------------------------------------------------------------------------------------
	WallSprite::WallSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos)
	{}

	WallSprite::~WallSprite() {}
	void WallSprite::OnCreate()
	{
		float HelfSize = 0.5f;
		//頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices =
		{
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	//--------------------------------------------------------------------------------------
	///	スクロールスプライト
	//--------------------------------------------------------------------------------------
	ScrollSprite::ScrollSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}

	ScrollSprite::~ScrollSprite() {}
	void ScrollSprite::OnCreate()
	{
		float HelfSize = 0.5f;
		//頂点配列
		m_BackupVertices =
		{
			{ VertexPositionTexture(Vec3(-HelfSize, HelfSize, 0), Vec2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Vec3(HelfSize, HelfSize, 0), Vec2(1.0f, 0.0f)) },
			{ VertexPositionTexture(Vec3(-HelfSize, -HelfSize, 0), Vec2(0.0f, 1.0f)) },
			{ VertexPositionTexture(Vec3(HelfSize, -HelfSize, 0), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PTSpriteDraw>(m_BackupVertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	void ScrollSprite::OnUpdate()
	{
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime*0.1f;
		if (m_TotalTime > 1.0f)
		{
			m_TotalTime = 0;
		}
		vector<VertexPositionTexture> NewVertices;
		for (size_t i = 0; i < m_BackupVertices.size(); i++)
		{
			Vec2 UV = m_BackupVertices[i].textureCoordinate;
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 1.0f) {
				UV.x += m_TotalTime;
			}
			auto v = VertexPositionTexture(
				m_BackupVertices[i].position,
				UV
			);
			NewVertices.push_back(v);
		}
		auto PtrDraw = GetComponent<PTSpriteDraw>();
		PtrDraw->UpdateVertices(NewVertices);
	}

	//--------------------------------------------------------------------------------------
	///	アニメスプライト
	//--------------------------------------------------------------------------------------
	AnimeSprite::AnimeSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec2& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0.0f),
		m_Selected(false)
	{}

	AnimeSprite::~AnimeSprite() {}

	void AnimeSprite::OnCreate()
	{
		float HelfSize = 0.5f;
		//頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertex = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertex, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	void AnimeSprite::OnUpdate()
	{
		if (m_Selected)
		{
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += ElapsedTime * 5.0f;
			if (m_TotalTime >= XM_2PI)
			{
				m_TotalTime = 0;
			}
			auto PtrDraw = GetComponent<PCTSpriteDraw>();
			Col4 col(1.0, 1.0, 1.0, 1.0);
			col.w = sin(m_TotalTime) * 0.5f + 0.5f;
			PtrDraw->SetDiffuse(col);
		}
		else
		{
			auto PtrDraw = GetComponent<PCTSpriteDraw>();
			Col4 col(1.0, 1.0, 1.0, 1.0);
			PtrDraw->SetDiffuse(col);
		}
	}

	//--------------------------------------------------------------------------------------
	///	スコア表示のスプライト
	//--------------------------------------------------------------------------------------
	ScoreSprite::ScoreSprite(const shared_ptr<Stage>& StagePtr, UINT NumberOfDigits,
		const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_NumberOfDigits(NumberOfDigits),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_Score(0.0f)
	{}

	void ScoreSprite::OnCreate() {
		float XPiecesize = 1.0f / (float)m_NumberOfDigits;
		float HelfSize = 0.5f;

		//インデックス配列
		vector<uint16_t> indices;
		for (UINT i = 0; i < m_NumberOfDigits; i++) {
			float Vertex0 = -HelfSize + XPiecesize * (float)i;
			float Vertex1 = Vertex0 + XPiecesize;
			//0
			m_BackupVertices.push_back(
				VertexPositionTexture(Vec3(Vertex0, HelfSize, 0), Vec2(0.0f, 0.0f))
			);
			//1
			m_BackupVertices.push_back(
				VertexPositionTexture(Vec3(Vertex1, HelfSize, 0), Vec2(0.1f, 0.0f))
			);
			//2
			m_BackupVertices.push_back(
				VertexPositionTexture(Vec3(Vertex0, -HelfSize, 0), Vec2(0.0f, 1.0f))
			);
			//3
			m_BackupVertices.push_back(
				VertexPositionTexture(Vec3(Vertex1, -HelfSize, 0), Vec2(0.1f, 1.0f))
			);
			indices.push_back(i * 4 + 0);
			indices.push_back(i * 4 + 1);
			indices.push_back(i * 4 + 2);
			indices.push_back(i * 4 + 1);
			indices.push_back(i * 4 + 3);
			indices.push_back(i * 4 + 2);
		}

		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PTSpriteDraw>(m_BackupVertices, indices);
		PtrDraw->SetTextureResource(m_TextureKey);

		GetStage()->SetSharedGameObject(L"ScoreSprite", GetThis<ScoreSprite>());
	}

	void ScoreSprite::OnUpdate() {
		vector<VertexPositionTexture> NewVertices;
		UINT Num;
		int VerNum = 0;
		for (UINT i = m_NumberOfDigits; i > 0; i--) {
			UINT Base = (UINT)pow(10, i);
			Num = ((UINT)m_Score) % Base;
			Num = Num / (Base / 10);
			Vec2 UV0 = m_BackupVertices[VerNum].textureCoordinate;
			UV0.x = (float)Num / 10.0f;
			auto v = VertexPositionTexture(
				m_BackupVertices[VerNum].position,
				UV0
			);
			NewVertices.push_back(v);

			Vec2 UV1 = m_BackupVertices[VerNum + 1].textureCoordinate;
			UV1.x = UV0.x + 0.1f;
			v = VertexPositionTexture(
				m_BackupVertices[VerNum + 1].position,
				UV1
			);
			NewVertices.push_back(v);

			Vec2 UV2 = m_BackupVertices[VerNum + 2].textureCoordinate;
			UV2.x = UV0.x;

			v = VertexPositionTexture(
				m_BackupVertices[VerNum + 2].position,
				UV2
			);
			NewVertices.push_back(v);

			Vec2 UV3 = m_BackupVertices[VerNum + 3].textureCoordinate;
			UV3.x = UV0.x + 0.1f;

			v = VertexPositionTexture(
				m_BackupVertices[VerNum + 3].position,
				UV3
			);
			NewVertices.push_back(v);

			VerNum += 4;
		}
		auto PtrDraw = GetComponent<PTSpriteDraw>();
		PtrDraw->UpdateVertices(NewVertices);
	}

	//--------------------------------------------------------------------------------------
	//Aroowスプライト
	//--------------------------------------------------------------------------------------

	ArrowSprite::ArrowSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec2& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0.0f)
	{}

	ArrowSprite::~ArrowSprite() {}

	void ArrowSprite::OnCreate()
	{
		float HelfSize = 0.5f;
		//頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertex = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertex, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
		auto PtrAction = AddComponent<Action>();

	}

	void ArrowSprite::OnUpdate()
	{
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime * 5.0f;
		if (m_TotalTime >= XM_2PI)
		{
			m_TotalTime = 0;
		}
		auto PtrDraw = GetComponent<PCTSpriteDraw>();
	}


	//--------------------------------------------------------------------------------------
	//タイトルのアニメーション
	//--------------------------------------------------------------------------------------
	//構築と消滅
	SpriteStudioAnim::SpriteStudioAnim(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir, const wstring& ssae, const wstring& animName) :
		SS5ssae(StagePtr, BaseDir, ssae, animName, true)
	{}
	SpriteStudioAnim::~SpriteStudioAnim() {}
	//初期化
	void SpriteStudioAnim::OnCreate()
	{
		//元となるオブジェクトからアニメーションオブジェクトへの行列の設定
		//作成されたグラフィックとの調整に使用
		Mat4x4 mat;
		mat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0, 0, 0),
			Vec3(0, 0, 0),
			Vec3(0, 0, 0)
		);
		SetToAnimeMatrix(mat);

		//位置の初期化
		auto PtrT = AddComponent<Transform>();
		//スプライト版のSpriteStdioは、サイズ32.0f基準のスプライトを作成するので
		//それに合わせてスケーリングする
		PtrT->SetScale(32.0f, 32.0f, 1.0f);
		//ポジションはピクセル単位センター合わせ（固定）
		PtrT->SetPosition(Vec3(0, 0, 0));
		//親クラスのOnCreateを必ず呼ぶ
		SS5ssae::OnCreate();
		//値は秒あたりのフレーム数
		SetFps(30.0f);
	}
	void SpriteStudioAnim::OnUpdate()
	{
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		auto PtrT = GetComponent<Transform>();
		auto Pos = PtrT->GetPosition();
		PtrT->SetPosition(Pos);

		//アニメーションを更新する
		UpdateAnimeTime(ElapsedTime);

	}

	void SpriteStudioAnim::AnimeChangeMotion(const wstring & Key, bool looped)
	{
		ChangeAnimation(Key);
		SetLooped(looped);

	}


	//--------------------------------------------------------------------------------------
	///	SelectMojiModel
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SelectMojiModel::SelectMojiModel(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	SelectMojiModel::~SelectMojiModel() {}
	//初期化
	void SelectMojiModel::OnCreate()
	{
		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(1.5f, 1.5f, 1.5f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Mat4x4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);


		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticModelDraw>();
		PtrDraw->SetSpecular(Col4(1.0f, 1.0f, 1.0f, 0.7f));

		PtrDraw->SetMeshResource(L"STAGESELECT_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);

	}


	//--------------------------------------------------------------------------------------
	///	SelectRingModel
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SelectRingModel::SelectRingModel(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	SelectRingModel::~SelectRingModel() {}
	//初期化
	void SelectRingModel::OnCreate()
	{
		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(1.5f, 1.5f, 1.5f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Mat4x4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//アクションの登録
		auto PtrAction = AddComponent<Action>();

		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticModelDraw>();
		PtrDraw->SetSpecular(Col4(1.0f, 1.0f, 1.0f, 5.0f));

		PtrDraw->SetMeshResource(L"SELECT_RING_MESH");
		PtrDraw->SetLineTextureResource(L"SELECT_RING_LINE_TEX");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);

	}


	//--------------------------------------------------------------------------------------
	///	StageSelectSquare
	//--------------------------------------------------------------------------------------
	Square::Square(const shared_ptr<Stage>& stagePtr, Square::InitParam initParam) :
		GameObject(stagePtr),
		m_InitParam(initParam)
	{

	}

	void Square::OnCreate()
	{
		float halfSize = 0.5f;
		float xScale = m_InitParam.m_AspectWdivH * 0.5f;

		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-halfSize * xScale, halfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(halfSize * xScale, halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(1, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-halfSize * xScale, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 1)) },
			{ VertexPositionColorTexture(Vec3(halfSize * xScale, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(1, 1)) },
		};

		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);

		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_InitParam.m_Position);
		trans->SetScale(m_InitParam.m_Scale);
		trans->SetRotation(m_InitParam.m_Rotate);
		auto drawComp = AddComponent<PCTStaticDraw>();
		drawComp->SetTextureResource(m_InitParam.m_TexKey);
		drawComp->SetMeshResource(m_SquareMesh);
		drawComp->SetBlendState(BlendState::AlphaBlend);
		drawComp->SetDepthStencilState(DepthStencilState::Default);
		drawComp->SetRasterizerState(RasterizerState::DoubleDraw);
	}

	void Square::OnUpdate()
	{
		//auto ptrCamera = GetStage()->GetView()->GetTargetCamera();
		//Quat qt;
		////向きをビルボードにする
		//qt = MyUtil::createBillboard(ptrCamera->GetAt() - ptrCamera->GetEye());
		//GetComponent<Transform>()->SetQuaternion(qt);
	}

	void Square::SetMeshVertexPositionUvX(float value)
	{
		m_Value = value;

		float halfSize = 0.5f;
		float xScale = m_InitParam.m_AspectWdivH * 0.5f;

		float width = halfSize * xScale * 2.0f;
		float widthHalf = width / 2.0f;

		float setValue = width * value - widthHalf;

		for (size_t i = 0; i < m_BackupVertices.size(); i++)
		{
			if (i == 1 || i == 3)
			{
				m_BackupVertices[i].position.x = setValue;
				m_BackupVertices[i].textureCoordinate.x = value;
			}
		}

		m_SquareMesh->UpdateVirtexBuffer(m_BackupVertices);
	}


	//--------------------------------------------------------------------------------------
	///	PostSelectSquare
	//--------------------------------------------------------------------------------------
	PostSquare::PostSquare(const shared_ptr<Stage>& stagePtr, PostSquare::InitParam initParam) :
		GameObject(stagePtr),
		m_InitParam(initParam)
	{

	}

	void PostSquare::OnCreate()
	{
		float halfSize = 0.5f;
		float xScale = m_InitParam.m_AspectWdivH * 0.5f;

		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-halfSize * xScale, halfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(halfSize * xScale, halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(1, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-halfSize * xScale, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 1)) },
			{ VertexPositionColorTexture(Vec3(halfSize * xScale, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(1, 1)) },
		};

		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);

		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_InitParam.m_Position);
		trans->SetScale(m_InitParam.m_Scale);
		trans->SetRotation(m_InitParam.m_Rotate);

		Square::InitParam frameInit;
		frameInit.m_AspectWdivH = m_InitParam.m_AspectWdivH;
		frameInit.m_Position = m_InitParam.m_Position;
		frameInit.m_Rotate = m_InitParam.m_Rotate;
		frameInit.m_Scale = m_InitParam.m_Scale*1.25f;
		frameInit.m_TexKey = L"Stage_" + Util::IntToWStr(m_InitParam.m_Num);

		m_FrameSquare = ObjectFactory::Create<Square>(GetStage(),frameInit);

		SetDrawLayer(5);

		SetAlphaActive(true);
	}

	void PostSquare::OnUpdate()
	{

	}

	void PostSquare::OnUpdate2()
	{
		auto trans = GetComponent<Transform>();

		auto frameTrans = m_FrameSquare->GetComponent<Transform>();
		frameTrans->SetPosition(trans->GetPosition() + trans->Forward()*-0.001f);
		frameTrans->SetRotation(trans->GetRotation());
	}

	void PostSquare::OnDraw()
	{
		auto dev = App::GetApp()->GetDeviceResources();
		auto deviceContext = dev->GetD3DDeviceContext();
		auto renderState = dev->GetRenderState();

		auto trans = GetComponent<Transform>();
		Mat4x4 world;
		world = trans->GetWorldMatrix();

		auto camera = App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetView()->GetTargetCamera();

		world *= camera->GetViewMatrix();
		world *= camera->GetProjMatrix();

		SpriteConstants sb;
		sb.Emissive = Col4(0, 0, 0, 0);
		sb.Diffuse = Col4(1,1,1,1);
		sb.World = world;

		deviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, m_SquareMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(m_SquareMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);


		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;

		deviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		deviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		deviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);

		deviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);

		deviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());

		deviceContext->OMSetBlendState(renderState->GetOpaque(), nullptr, 0xffffffff);

		deviceContext->OMSetDepthStencilState(renderState->GetDepthDefault(), 0);

		auto rtLock = m_InitParam.m_RenderTarget.lock();
		if (!rtLock)
			return;

		auto sr = rtLock->GetShaderResourceView();
		deviceContext->PSSetShaderResources(0, 1, &sr);

		ID3D11SamplerState* pSampler = renderState->GetLinearWrap();
		deviceContext->PSSetSamplers(0, 1, &pSampler);

		deviceContext->RSSetState(renderState->GetCullFront());
		deviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);

		deviceContext->RSSetState(renderState->GetCullBack());
		deviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);

		dev->InitializeStates();

		m_FrameSquare->OnDraw();
	}

	void PostSquare::SetMeshVertexPositionUvX(float value)
	{
		float halfSize = 0.5f;
		float xScale = m_InitParam.m_AspectWdivH * 0.5f;

		float width = halfSize * xScale * 2.0f;
		float widthHalf = width / 2.0f;

		float setValue = width * value - widthHalf;

		for (size_t i = 0; i < m_BackupVertices.size(); i++)
		{
			if (i == 1 || i == 3)
			{
				m_BackupVertices[i].position.x = setValue;
				m_BackupVertices[i].textureCoordinate.x = value;
			}
		}

		m_SquareMesh->UpdateVirtexBuffer(m_BackupVertices);
	}
}