#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TutorialBox::TutorialBox(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode) :
		GameObject(stagePtr)
	{
		auto scaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");
		auto rotStr = XmlDocReader::GetAttribute(pNode, L"Quat");
		auto posStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto texStr = XmlDocReader::GetAttribute(pNode, L"TextureKey");

		Vec3 scale = MyUtil::unityVec3StrToBCVec3(scaleStr);
		Quat quat = MyUtil::unityQuatStrToBCQuat(rotStr);
		Vec3 pos = MyUtil::unityVec3StrToBCVec3(posStr);

		InitParam param;
		param.m_Position = pos;
		param.m_Quat = quat;
		param.m_Scale = scale;
		param.m_TexKey = texStr;

		m_InitParam = param;
	}

	void TutorialBox::OnCreate()
	{
		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_InitParam.m_Position);
		trans->SetScale(m_InitParam.m_Scale);
		trans->SetQuaternion(m_InitParam.m_Quat);
	}

	void TutorialBox::OnUpdate()
	{
		auto trans = GetComponent<Transform>();

		vector<shared_ptr<GameObject>> player;
		GetStage()->GetUsedTagObjectVec(L"Player", player);
		OBB playerObb = player[0]->GetComponent<CollisionObb>()->GetObb();
		OBB ownObb(trans->GetScale(), trans->GetWorldMatrix());
		if (HitTest::OBB_OBB(playerObb, ownObb))
		{
			TutorialMessage::InitParam initParam;
			initParam.m_StartPos = Vec3(0, 0, 0);
			initParam.m_TexKey = m_InitParam.m_TexKey;
			GetStage()->AddGameObject<TutorialMessage>(initParam);

			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
		}
	}

	void TutorialBox::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{

	}
}