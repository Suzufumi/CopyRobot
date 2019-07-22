/*!
* @file PostEffectRenderTarget.cpp
* @brief 描画後にかけるエフェクト
* @author Kyohei Okawa
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	struct PostEffectRenderTarget::Impl
	{
		ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
		ComPtr<ID3D11Texture2D> m_Texture2D;
		ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		Impl() {}
		~Impl() {}
	};

	PostEffectRenderTarget::PostEffectRenderTarget() :
		pImpl(new Impl())
	{
		try
		{
			//デバイスとコンテキストインターフェイスの取得
			auto dev = App::GetApp()->GetDeviceResources();
			auto pD3D11Device = dev->GetD3DDevice();
			auto pSwapChain = dev->GetSwapChain();
			auto pD3D11DeviceContext = dev->GetD3DDeviceContext();

			D3D11_TEXTURE2D_DESC texDesc;
			ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
			texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			texDesc.Width = App::GetApp()->GetGameWidth();
			texDesc.Height = App::GetApp()->GetGameHeight();
			texDesc.CPUAccessFlags = 0;
			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;

			ThrowIfFailed(
				pD3D11Device->CreateTexture2D(&texDesc, nullptr, &pImpl->m_Texture2D),
				L"テクスチャの作成に失敗しました",
				L"PostEffectRenderTarget",
				L""
			);

			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			memset(&rtvDesc, 0, sizeof(rtvDesc));
			rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

			ThrowIfFailed(
				pD3D11Device->CreateRenderTargetView(pImpl->m_Texture2D.Get(), &rtvDesc, &pImpl->m_RenderTargetView),
				L"ポストエフェクト用レンダーターゲットの作成に失敗しました",
				L"",
				L""
			);

			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			ThrowIfFailed(
				pD3D11Device->CreateShaderResourceView(pImpl->m_Texture2D.Get(), &shaderResourceViewDesc, &pImpl->m_ShaderResourceView),
				L"シェーダーリソースビューの作成に失敗しました。",
				L"PostEffectRenderTarget",
				L""
			);

			D3D11_VIEWPORT viewPort;
			ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
			viewPort.Height = (FLOAT)App::GetApp()->GetGameHeight();
			viewPort.Width = (FLOAT)App::GetApp()->GetGameWidth();
			viewPort.MinDepth = 0.f;
			viewPort.MaxDepth = 1.f;
			viewPort.TopLeftX = 0;
			viewPort.TopLeftY = 0;
			SetViewport(viewPort);
		}
		catch (...)
		{
			throw;
		}
	}

	PostEffectRenderTarget::~PostEffectRenderTarget() {}

	ID3D11ShaderResourceView* PostEffectRenderTarget::GetShaderResourceView() const { return pImpl->m_ShaderResourceView.Get(); }

	void PostEffectRenderTarget::ClearViews(const bsm::Col4& col)
	{
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		float Color[4] = { col.x, col.y, col.z, col.w };
		D3D11_VIEWPORT ViewPort;
		//ビューポートのセットアップ
		ZeroMemory(&ViewPort, sizeof(ViewPort));
		ViewPort.Width = (float)App::GetApp()->GetGameWidth();
		ViewPort.Height = (float)App::GetApp()->GetGameHeight();
		ViewPort.MinDepth = 0.0f;
		ViewPort.MaxDepth = 1.0f;
		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		pD3D11DeviceContext->RSSetViewports(1, &ViewPort);
		
		//通常の深度バッファとステンシルバッファのクリア
		pD3D11DeviceContext->ClearRenderTargetView(pImpl->m_RenderTargetView.Get(), Color);
	}

	void PostEffectRenderTarget::StartRenderTarget()
	{
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		ID3D11RenderTargetView* pV = pImpl->m_RenderTargetView.Get();
		ID3D11DepthStencilView* dsView = App::GetApp()->GetDeviceResources()->GetDefaultRenderTarget()->GetDepthStencilView();
		pD3D11DeviceContext->OMSetRenderTargets(1, &pV, dsView);

		auto viewPort = GetViewport();
		pD3D11DeviceContext->RSSetViewports(1, &viewPort);

		D3D11_RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (LONG)viewPort.Width;
		rect.bottom = (LONG)viewPort.Height;
		pD3D11DeviceContext->RSSetScissorRects(1, &rect);

		//シェーダーリソースビューのクリア
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//シェーダーは指定しない
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//ブレンドは指定しない
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void PostEffectRenderTarget::EndRenderTarget()
	{
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//シェーダーリソースビューのクリア
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//シェーダーは指定しない
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//ブレンドは指定しない
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	PostEffectSprite::PostEffectSprite(const shared_ptr<Stage>& stage, const shared_ptr<PostEffectRenderTarget> renderTarget) :
		GameObject(stage),
		m_RenderTarget(renderTarget),
		m_PostEffectColor(PostEffectColor::Color),
		m_Diffuse(Col4(1, 1, 1, 1)),
		m_IsVisible(true)
	{

	}

	void PostEffectSprite::OnCreate()
	{
		GameObject::OnCreate();

		float halfSize = 0.5f;
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-halfSize, halfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(halfSize, halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)1, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-halfSize, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)1)) },
			{ VertexPositionColorTexture(Vec3(halfSize, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)1, (float)1)) },
		};

		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, false);
	}

	void PostEffectSprite::OnDraw()
	{
		if (!m_IsVisible)
			return;

		auto dev = App::GetApp()->GetDeviceResources();
		auto deviceContext = dev->GetD3DDeviceContext();
		auto renderState = dev->GetRenderState();

		auto trans = GetComponent<Transform>();
		Mat4x4 world;
		world.affineTransformation2D(
			Vec2(trans->GetScale().x,trans->GetScale().y),
			Vec2(0, 0),
			trans->GetRotation().z,
			Vec2(trans->GetPosition().x,trans->GetPosition().y)
		);

		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Mat4x4 proj(XMMatrixOrthographicLH(w, h, -1.0f, 1.0f));
		world *= proj;

		SpriteConstants sb;
		sb.Emissive = Col4(0,0,0,0);
		sb.Diffuse = m_Diffuse;
		sb.World = world;

		deviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, m_MeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(m_MeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);


		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;

		deviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		deviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		deviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		switch (m_PostEffectColor)
		{
		case PostEffectColor::Color:
			deviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
			break;
		case PostEffectColor::GrayScale:
			deviceContext->PSSetShader(PSPCTGrayScaleSprite::GetPtr()->GetShader(), nullptr, 0);
			break;
		case PostEffectColor::Sepia:
			deviceContext->PSSetShader(PSPCTSepiaSprite::GetPtr()->GetShader(), nullptr, 0);
			break;
		}

		deviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());

		deviceContext->OMSetBlendState(renderState->GetOpaque(), nullptr, 0xffffffff);

		deviceContext->OMSetDepthStencilState(renderState->GetDepthNone(), 0);
		deviceContext->RSSetState(renderState->GetCullBack());

		auto rtLock = m_RenderTarget.lock();
		if (!rtLock)
			return;

		auto sr = rtLock->GetShaderResourceView();
		deviceContext->PSSetShaderResources(0, 1, &sr);

		ID3D11SamplerState* pSampler = renderState->GetLinearWrap();
		deviceContext->PSSetSamplers(0, 1, &pSampler);

		deviceContext->DrawIndexed(m_MeshResource->GetNumIndicis(), 0, 0);

		dev->InitializeStates();
	}
}