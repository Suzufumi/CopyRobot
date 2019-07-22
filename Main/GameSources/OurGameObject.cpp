#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void OurGameObject::TakenIn(Vec3 takenPos)
	{
		RemoveComponent<Action>();

		auto ptrAction = AddComponent<Action>();
		ptrAction->AddMoveTo(0.8f, takenPos, Lerp::Easein);
		ptrAction->AddScaleTo(0.8f, Vec3(0.01f, 0.01f, 0.01f), Lerp::Easein);
		ptrAction->Run();
	}

	// Xml����Pos,Quat,Scale�̏����擾����
	void OurGameObject::LoadXmlParam(IXMLDOMNodePtr pNode, Vec3& pos, Quat& quat, Vec3& scale)
	{
		// Xml����Position�AQuat,Scale���擾����
		auto posStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto rotStr = XmlDocReader::GetAttribute(pNode, L"Quat");
		auto scaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");

		// wstring����Vec3,Quat�ɕϊ�����
		pos = MyUtil::unityVec3StrToBCVec3(posStr);
		quat = MyUtil::unityQuatStrToBCQuat(rotStr);
		scale = MyUtil::unityVec3StrToBCVec3(scaleStr);
	}

	// ���f���̍s��ݒ�
	Mat4x4 OurGameObject::SetModelMatrix(const Vec3& scale, const Vec3& rotOrigin, const Vec3& rot, const Vec3& pos)
	{
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(scale),	// �傫��
			Vec3(rotOrigin),	// ��]�̌��_
			Vec3(rot),	// ��]
			Vec3(pos)	// �ʒu
		);

		return spanMat;
	}


}