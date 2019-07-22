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

	// XmlからPos,Quat,Scaleの情報を取得する
	void OurGameObject::LoadXmlParam(IXMLDOMNodePtr pNode, Vec3& pos, Quat& quat, Vec3& scale)
	{
		// XmlからPosition、Quat,Scaleを取得する
		auto posStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto rotStr = XmlDocReader::GetAttribute(pNode, L"Quat");
		auto scaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");

		// wstringからVec3,Quatに変換する
		pos = MyUtil::unityVec3StrToBCVec3(posStr);
		quat = MyUtil::unityQuatStrToBCQuat(rotStr);
		scale = MyUtil::unityVec3StrToBCVec3(scaleStr);
	}

	// モデルの行列設定
	Mat4x4 OurGameObject::SetModelMatrix(const Vec3& scale, const Vec3& rotOrigin, const Vec3& rot, const Vec3& pos)
	{
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(scale),	// 大きさ
			Vec3(rotOrigin),	// 回転の原点
			Vec3(rot),	// 回転
			Vec3(pos)	// 位置
		);

		return spanMat;
	}


}