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
		// Xml‚©‚çPos,Quat,Scale‚Ìî•ñ‚ğæ“¾‚·‚é
		void LoadXmlParam(IXMLDOMNodePtr pNode, Vec3& pos, Quat& quat, Vec3& scale);
		// ƒ‚ƒfƒ‹‚Ìs—ñ‚ğİ’è‚·‚é
		Mat4x4 SetModelMatrix(const Vec3& scale, const Vec3& rotOrigin, const Vec3& rot, const Vec3& pos);
	};
}