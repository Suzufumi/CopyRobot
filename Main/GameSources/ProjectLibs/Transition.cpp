#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void TransitionObj::OnCreate()
	{
		switch (m_TransitionState)
		{
		case TransitionState::FadeIn:
			m_CountSec = 0;
			break;
		case TransitionState::FadeOut:
			m_CountSec = m_TransitionSec;
			break;
		}

		VertexPositionColorTexture vertex;

		//頂点を決定
		{
			//左上
			vertex.position = Vec3(-1, 1, 0);
			vertex.color = Col4(1, 1, 1, 1);
			vertex.textureCoordinate = Vec2(0, 0);
			m_Vertices.push_back(vertex);

			//右上
			vertex.position = Vec3(1, 1, 0);
			vertex.textureCoordinate = Vec2(1, 0);
			m_Vertices.push_back(vertex);

			//左下
			vertex.position = Vec3(-1, -1, 0);
			vertex.textureCoordinate = Vec2(0, 1);
			m_Vertices.push_back(vertex);

			//左下
			vertex.position = Vec3(-1, -1, 0);
			vertex.textureCoordinate = Vec2(0, 1);
			m_Vertices.push_back(vertex);

			//右上
			vertex.position = Vec3(1, 1, 0);
			vertex.textureCoordinate = Vec2(1, 0);
			m_Vertices.push_back(vertex);

			//右下
			vertex.position = Vec3(1, -1, 0);
			vertex.textureCoordinate = Vec2(1, 1);
			m_Vertices.push_back(vertex);
		}

		m_MeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(m_Vertices,false);
	}

	void TransitionObj::OnUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();

		switch (m_TransitionState)
		{
		case TransitionState::FadeIn:
		{
			m_CountSec += delta;
			if (m_CountSec >= m_TransitionSec)
			{
				m_IsFinished = true;
				m_CountSec = m_TransitionSec;
			}
			break;
		}
		case TransitionState::FadeOut:
		{
			m_CountSec -= delta;
			if (m_CountSec <= 0)
			{
				m_IsFinished = true;
				m_CountSec = 0;
			}
			break;
		}
		}
	}

	void NormalFadeTransition::OnDraw()
	{
		auto dev = App::GetApp()->GetDeviceResources();
		auto deviceContext = dev->GetD3DDeviceContext();
		auto renderState = dev->GetRenderState();

		TransitionConstants cb;
		cb.Second = Vec4(m_TransitionSec, m_CountSec, 0, 0);
		cb.Color = m_FadeColor;

		deviceContext->UpdateSubresource(CBTransition::GetPtr()->GetBuffer(), 0, nullptr, &cb, 0, 0);

		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11Buffer* pConstantBuffer = CBTransition::GetPtr()->GetBuffer();

		deviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		deviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		deviceContext->VSSetShader(VSTransition::GetPtr()->GetShader(), nullptr, 0);
		deviceContext->PSSetShader(PSTransition::GetPtr()->GetShader(), nullptr, 0);

		deviceContext->IASetInputLayout(VSTransition::GetPtr()->GetInputLayout());

		deviceContext->OMSetBlendState(renderState->GetAlphaBlendEx(), nullptr, 0xffffffff);

		deviceContext->OMSetDepthStencilState(renderState->GetDepthNone(), 0);
		deviceContext->RSSetState(renderState->GetCullNone());

		deviceContext->IASetVertexBuffers(0, 1, m_MeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);

		deviceContext->Draw(m_Vertices.size(), 0);

		dev->InitializeStates();
	}

	void FadeMaskTransition::OnCreate()
	{
		TransitionObj::OnCreate();

		m_TextureResource = App::GetApp()->GetResource<TextureResource>(m_FadeTex);
	}

	void FadeMaskTransition::OnDraw()
	{
		auto dev = App::GetApp()->GetDeviceResources();
		auto deviceContext = dev->GetD3DDeviceContext();
		auto renderState = dev->GetRenderState();

		TransitionConstants cb;
		cb.Second = Vec4(m_TransitionSec, m_CountSec, 0, 0);
		cb.Color = m_FadeColor;

		deviceContext->UpdateSubresource(CBTransition::GetPtr()->GetBuffer(), 0, nullptr, &cb, 0, 0);

		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11Buffer* pConstantBuffer = CBTransition::GetPtr()->GetBuffer();

		deviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		deviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		deviceContext->VSSetShader(VSTransition::GetPtr()->GetShader(), nullptr, 0);
		deviceContext->PSSetShader(PSFadeTexTransition::GetPtr()->GetShader(), nullptr, 0);

		//フェードテクスチャ
		deviceContext->PSSetShaderResources(0, 1, m_TextureResource->GetShaderResourceView().GetAddressOf());

		ID3D11SamplerState* pSampler = renderState->GetLinearWrap();
		deviceContext->PSSetSamplers(0, 1, &pSampler);

		deviceContext->IASetInputLayout(VSTransition::GetPtr()->GetInputLayout());

		deviceContext->OMSetBlendState(renderState->GetAlphaBlendEx(), nullptr, 0xffffffff);

		deviceContext->OMSetDepthStencilState(renderState->GetDepthNone(), 0);
		deviceContext->RSSetState(renderState->GetCullNone());

		deviceContext->IASetVertexBuffers(0, 1, m_MeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);

		deviceContext->Draw(m_Vertices.size(), 0);

		dev->InitializeStates();
	}

	Transition::Transition():
		m_IsStopFadeOut(false),
		m_IsPlayedOnlyFadeOut(false)
	{

	}

	void Transition::OnCreate()
	{
		m_StateMachine = make_shared <StateMachine<Transition>>(GetThis<Transition>());
		m_StateMachine->ChangeState(TransitionNoneState::Instance());
	}

	void Transition::OnUpdate()
	{
		m_StateMachine->Update();
	}

	void Transition::OnDraw()
	{
		auto state = m_StateMachine->GetCurrentState();

		if (state == FadeInState::Instance())
		{
			if(m_TransitionObjIn)
				m_TransitionObjIn->OnDraw();
		}
		else if (state == FadeOutState::Instance() || state == OnlyFadeOutState::Instance())
		{
			if(m_TransitionObjOut)
				m_TransitionObjOut->OnDraw();
		}
	}

	bool Transition::GetNowFadeOut()
	{
		auto state = m_StateMachine->GetCurrentState();

		if (state == FadeOutState::Instance() &&
			state == OnlyFadeOutState::Instance())
		{
			return true;
		}

		return false;
	}

	bool Transition::GetIsFadeNow()
	{
		auto state = m_StateMachine->GetCurrentState();

		if (state != TransitionNoneState::Instance())
			return true;

		return false;
	}

	void Transition::ClearTransitionObj()
	{
		m_TransitionObjIn = nullptr;
		m_TransitionObjOut = nullptr;
	}

	void Transition::PlayInOutTransitionWithMsg(shared_ptr<TransitionObj> inTransition, shared_ptr<TransitionObj> outTransition, wstring wstrMsg)
	{
		m_TransitionObjIn = inTransition;
		m_TransitionObjOut = outTransition;
		m_Msg = wstrMsg;

		m_StateMachine->ChangeState(FadeOutState::Instance());
	}

	void Transition::PlayInOutTransition(shared_ptr<TransitionObj> inTransition, shared_ptr<TransitionObj> outTransition)
	{
		m_TransitionObjIn = inTransition;
		m_TransitionObjOut = outTransition;

		m_StateMachine->ChangeState(OnlyFadeOutState::Instance());
	}

	void Transition::PlayInOutTransitionWithAction(shared_ptr<TransitionObj> inTransition, shared_ptr<TransitionObj> outTransition, std::function<void()> action)
	{
		m_TransitionObjIn = inTransition;
		m_TransitionObjOut = outTransition;
		m_AfterOutFunc = action;

		m_StateMachine->ChangeState(OnlyFadeOutState::Instance());
	}

	void Transition::PlayFadeIn(shared_ptr<TransitionObj> inTransition)
	{
		m_TransitionObjIn = inTransition;
		m_StateMachine->ChangeState(FadeInState::Instance());
	}

	void Transition::PlayFadeOut(shared_ptr<TransitionObj> outTransition,bool isStop)
	{
		m_IsStopFadeOut = isStop;
		m_TransitionObjOut = outTransition;
		m_StateMachine->ChangeState(OnlyFadeOutState::Instance());
	}

	IMPLEMENT_SINGLETON_INSTANCE(TransitionNoneState)

	IMPLEMENT_SINGLETON_INSTANCE(FadeInState)

	void FadeInState::Enter(const shared_ptr<Transition>& transObj)
	{

		auto action = transObj->m_AfterOutFunc;
		if (action)
		{
			action();
			transObj->m_AfterOutFunc = nullptr;
		}
	}

	void FadeInState::Execute(const shared_ptr<Transition>& transObj)
	{
		auto objIn = transObj->m_TransitionObjIn;
		if(objIn)
			objIn->OnUpdate();
		else
		{
			transObj->m_StateMachine->ChangeState(TransitionNoneState::Instance());
			return;
		}

		if (objIn->GetIsFinishedTransition())
			transObj->m_StateMachine->ChangeState(TransitionNoneState::Instance());
	}

	void FadeInState::Exit(const shared_ptr<Transition>& transObj)
	{
		transObj->ClearTransitionObj();
	}

	IMPLEMENT_SINGLETON_INSTANCE(FadeOutState)

	void FadeOutState::Enter(const shared_ptr<Transition>& transObj)
	{

	}
	
	void FadeOutState::Execute(const shared_ptr<Transition>& transObj)
	{
		auto objOut = transObj->m_TransitionObjOut;
		if(objOut)
			objOut->OnUpdate();
		else
		{
			transObj->m_StateMachine->ChangeState(FadeInState::Instance());
			return;
		}

		if (objOut->GetIsFinishedTransition())
			transObj->m_StateMachine->ChangeState(FadeInState::Instance());
	}

	void FadeOutState::Exit(const shared_ptr<Transition>& transObj)
	{
		transObj->PostEvent(0,
			App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetThis<ObjectInterface>(),
			App::GetApp()->GetScene<Scene>(),
			transObj->m_Msg);
	}

	IMPLEMENT_SINGLETON_INSTANCE(OnlyFadeOutState)

	void OnlyFadeOutState::Enter(const shared_ptr<Transition>& transObj)
	{
		transObj->m_IsPlayedOnlyFadeOut = true;
	}

	void OnlyFadeOutState::Execute(const shared_ptr<Transition>& transObj)
	{
		auto objIn = transObj->m_TransitionObjIn;
		auto objOut = transObj->m_TransitionObjOut;
		if (objOut)
			objOut->OnUpdate();
		else
		{
			if (objIn == nullptr)
			{
				transObj->ClearTransitionObj();
				transObj->m_StateMachine->ChangeState(TransitionNoneState::Instance());
			}
			else
			{
				transObj->m_StateMachine->ChangeState(FadeInState::Instance());
			}

			return;
		}
		
		if (objOut->GetIsFinishedTransition())
		{
			if (objIn == nullptr)
			{
				transObj->m_IsPlayedOnlyFadeOut = false;
				if (!transObj->m_IsStopFadeOut)
				{
					transObj->ClearTransitionObj();
					transObj->m_StateMachine->ChangeState(TransitionNoneState::Instance());
				}
			}
			else
			{
				transObj->m_StateMachine->ChangeState(FadeInState::Instance());
			}
		}
	}

	void OnlyFadeOutState::Exit(const shared_ptr<Transition>& transObj)
	{

	}
}