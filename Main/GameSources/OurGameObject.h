#pragma once
#include "stdafx.h"

namespace basecross
{
	class OurGameObject : public GameObject
	{
	public:
		OurGameObject(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{

		}
		virtual ~OurGameObject() {}

		virtual void TakenIn(Vec3 takenPos);
		virtual void EndTakeIn() {}
		virtual void Stop() {}
		// Xml����Pos,Quat,Scale�̏����擾����
		void LoadXmlParam(IXMLDOMNodePtr pNode, Vec3& pos, Quat& quat, Vec3& scale);
		// ���f���̍s���ݒ肷��
		Mat4x4 SetModelMatrix(const Vec3& scale, const Vec3& rotOrigin, const Vec3& rot, const Vec3& pos);
	};
}