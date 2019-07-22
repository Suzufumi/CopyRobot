/*!
@file Sprite.cpp
@brief スプライト
@author Kyohei Okawa
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Sprite::Sprite(
		const shared_ptr<Stage>& stagePtr,
		const wstring& textureResName,
		const Vec2& startScale,
		float startRot,
		const Vec2& startPos,
		UINT xWrap, UINT yWrap
	) :
		GameObject(stagePtr),
		m_TextureResName(textureResName),
		m_Scale(startScale),
		m_Rotation(startRot),
		m_Position(startPos),
		m_XWrap(xWrap),
		m_YWrap(yWrap),
		m_Emissive(0,0,0,0),
		m_Diffuse(1,1,1,1),
		m_BlendState(BlendState::Trace),
		m_IsAnimated(false),
		m_IsFadeOut(false),
		m_FadeOutCount(0)
	{

	}

	void Sprite::OnCreate()
	{
		GameObject::OnCreate();

		float halfSize = 0.5f;

		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-halfSize, halfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(halfSize, halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-halfSize, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(halfSize, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};

		AdjustVertex();

		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
	}

	void Sprite::UpdateVertex(float elapsedTime, VertexPositionColorTexture* vertices)
	{
		for (size_t i = 0; i < m_BackupVertices.size(); i++)
		{
			vertices[i] = m_BackupVertices[i];
		}
	}

	void Sprite::OnUpdate()
	{
		float elapsedTime = App::GetApp()->GetElapsedTime();
		auto dev = App::GetApp()->GetDeviceResources();
		auto deviceContext = dev->GetD3DDeviceContext();

		//アニメーション処理
		if (m_IsAnimated)
		{
			m_AnimInfo.timeCount += elapsedTime;

			if (m_AnimInfo.timeCount > m_AnimInfo.timeInterval)
			{
				m_AnimInfo.timeCount = 0;
				m_AnimInfo.nowIndex++;

				unordered_map<wstring, TexturePackInfo> texMap = App::GetApp()->GetScene<Scene>()->GetTexturePackdInfo(m_AnimInfo.xmlKey);
				if (m_AnimInfo.nowIndex >= (int)texMap.size())
					m_AnimInfo.nowIndex = 0;

				SelectTextureUVFromPackInfo(m_AnimInfo.xmlKey, Util::IntToWStr(m_AnimInfo.nowIndex) + L".png");
			}
		}

		//フェードアウト処理
		if (m_IsFadeOut)
		{
			m_Diffuse.w -= elapsedTime / m_FadeOutInterval;
			m_FadeOutCount += elapsedTime;
			if (m_FadeOutCount > m_FadeOutInterval)
			{
				m_FadeOutCount = 0;
				m_IsFadeOut = false;
				m_Diffuse.w = 0.0f;
			}
		}

		//頂点情報変更処理
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		if (FAILED(deviceContext->Map(m_SquareMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer)))
		{
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"WrappedSprite::UpdateVertex()"
			);
		}

		VertexPositionColorTexture* vertices
			= (VertexPositionColorTexture*)mappedBuffer.pData;
		UpdateVertex(elapsedTime, vertices);

		deviceContext->Unmap(m_SquareMesh->GetVertexBuffer().Get(), 0);
	}

	void Sprite::OnDraw()
	{
		auto texPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto dev = App::GetApp()->GetDeviceResources();
		auto deviceContext = dev->GetD3DDeviceContext();
		auto renderState = dev->GetRenderState();

		Mat4x4 world;
		world.affineTransformation2D(
			m_Scale,
			Vec2(0, 0),
			m_Rotation,
			m_Position
		);

		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Mat4x4 proj(XMMatrixOrthographicLH(w, h, -1.0f, 1.0f));
		world *= proj;

		SpriteConstants sb;
		sb.Emissive = m_Emissive;
		sb.Diffuse = m_Diffuse;
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
		deviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(),nullptr,0);

		deviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());

		switch (m_BlendState) 
		{
		case BlendState::Opaque:
			deviceContext->OMSetBlendState(renderState->GetOpaque(), nullptr, 0xffffffff);
			break;
		case BlendState::Trace:
			deviceContext->OMSetBlendState(renderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			break;
		case BlendState::Additive:
			deviceContext->OMSetBlendState(renderState->GetAdditive(), nullptr, 0xffffffff);
			break;
		}

		deviceContext->OMSetDepthStencilState(renderState->GetDepthNone(), 0);
		deviceContext->RSSetState(renderState->GetCullBack());

		ID3D11ShaderResourceView* pNull[1] = { 0 };
		deviceContext->PSSetShaderResources(0, 1, texPtr->GetShaderResourceView().GetAddressOf());

		ID3D11SamplerState* pSampler = renderState->GetLinearWrap();
		deviceContext->PSSetSamplers(0, 1, &pSampler);

		deviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);

		dev->InitializeStates();
	}

	void Sprite::SelectTextureUVFromPackInfo(wstring xmlKey, wstring texname)
	{
		auto info = App::GetApp()->GetScene<Scene>()->GetTexturePackdInfo(xmlKey)[texname];

		float texWidth = (float)info.packedTextureInfo.width;
		float texHeight = (float)info.packedTextureInfo.height;

		float left = info.x / texWidth;
		float top = info.y / texHeight;
		float right = (info.x + info.width) / texWidth;
		float bottom = (info.y + info.height) / texHeight;

		float halfSize = 0.5f;

		m_BackupVertices.clear();
		m_BackupVertices = 
		{
			{ VertexPositionColorTexture(Vec3(-halfSize, halfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(left, top)) },
			{ VertexPositionColorTexture(Vec3(halfSize, halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(right, top)) },
			{ VertexPositionColorTexture(Vec3(-halfSize, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(left, bottom)) },
			{ VertexPositionColorTexture(Vec3(halfSize, -halfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(right, bottom)) },
		};
	}

	void Sprite::StartAnimation(wstring texKey, wstring xmlKey, float animIntervalSec)
	{
		m_IsAnimated = true;

		m_AnimInfo.nowIndex = 0;
		m_AnimInfo.texKey = texKey;
		m_AnimInfo.xmlKey = xmlKey;
		m_AnimInfo.timeInterval = animIntervalSec;

		m_TextureResName = texKey;

		SelectTextureUVFromPackInfo(xmlKey, L"0.png");
	}

	void Sprite::FadeOut(float outTime)
	{
		if (m_IsFadeOut)
			return;

		m_FadeOutInterval = outTime;
		m_IsFadeOut = true;
	}
}