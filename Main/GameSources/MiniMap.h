#pragma once
#include "stdafx.h"

namespace basecross
{
	class MiniMap : public GameObject
	{
		shared_ptr<Camera> m_Camera;
		shared_ptr<PostEffectRenderTarget> m_RenderTarget;
		shared_ptr<PostEffectSprite> m_Sprite;
		weak_ptr<Transform> m_Target;
		shared_ptr<Sprite> m_BackSprite;

		shared_ptr<Sprite> m_FrameSprite;
		
		Vec3 m_PosDiff;
		Vec3 m_LookAtDiff;

		template <typename T>
		void MapObjectDraw(const shared_ptr<GameObject>& obj,const float scale = 1.0f)
		{
			auto drawComp = GetDynamicComponent<SmBaseDraw>();
			auto trans = GetComponent<Transform>();

			auto objDyn = dynamic_pointer_cast<T>(obj);
			if (objDyn)
			{
				auto objTrans = objDyn->GetComponent<Transform>();
				Vec3 setPos = objTrans->GetPosition();
				setPos.y = 0.0f;
				trans->SetPosition(setPos);
				trans->SetScale(objTrans->GetScale()*scale);
				trans->SetQuaternion(objTrans->GetQuaternion());

				auto mesh = objDyn->GetMeshResourceForMinimap();
				drawComp->SetMeshResource(mesh);
				drawComp->OnDraw();
			}
		}
	public:
		MiniMap(const shared_ptr<Stage>& stage);
		virtual ~MiniMap() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;

		void MiniMapDraw();
		void SetTarget(shared_ptr<Transform> target) { m_Target = target; }

	};
}