#pragma once
#include "stdafx.h"

namespace basecross
{
	class PlayerAssimililationObject : public GameObject 
	{
		
	public:
		PlayerAssimililationObject(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr){}
		virtual ~PlayerAssimililationObject() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void ResetDiffuse();
		void ChangeAlpha(float alpha);
		void ChangeMeshResource(wstring key);
	};
}