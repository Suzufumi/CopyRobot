#include "stdafx.h"
#include "Project.h"

namespace basecross
{

	//----------------------------------------------------------------------
	/// Default�A�j���[�V����
	//----------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NormalEnemyDefaultAnim)
	
	void NormalEnemyDefaultAnim::Enter(const shared_ptr<NormalEnemy>& Obj)
	{
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"Default");
	}

	void NormalEnemyDefaultAnim::Execute(const shared_ptr<NormalEnemy>& Obj)
	{

	}

	void NormalEnemyDefaultAnim::Exit(const shared_ptr<NormalEnemy>& Obj)
	{

	}

	//----------------------------------------------------------------------
	/// Chase�A�j���[�V����
	//----------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NormalEnemyChaseAnim)

	void NormalEnemyChaseAnim::Enter(const shared_ptr<NormalEnemy>& Obj)
	{
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"Chase");
	}
	void NormalEnemyChaseAnim::Execute(const shared_ptr<NormalEnemy>& Obj)
	{

	}
	void NormalEnemyChaseAnim::Exit(const shared_ptr<NormalEnemy>& Obj)
	{

	}

	//----------------------------------------------------------------------
	/// Release�A�j���[�V����
	//----------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NormalEnemyReleaseAnim)

	void NormalEnemyReleaseAnim::Enter(const shared_ptr<NormalEnemy>& Obj)
	{
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"Release");
	}

	void NormalEnemyReleaseAnim::Execute(const shared_ptr<NormalEnemy>& Obj)
	{

	}

	void NormalEnemyReleaseAnim::Exit(const shared_ptr<NormalEnemy>& Obj)
	{

	}

	//----------------------------------------------------------------------
	/// Default�A�j���[�V����(CamelEnemy)
	//----------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CamelEnemyDefaultAnim)

	void CamelEnemyDefaultAnim::Enter(const shared_ptr<CamereonEnemy>& Obj)
	{
		Obj->GetComponent<PNTBoneModelDraw>()->ChangeCurrentAnimation(L"CamelDefault");
	}

	void CamelEnemyDefaultAnim::Execute(const shared_ptr<CamereonEnemy>& Obj)
	{

	}

	void CamelEnemyDefaultAnim::Exit(const shared_ptr<CamereonEnemy>& Obj)
	{

	}

	//----------------------------------------------------------------------
	/// Stay�A�j���[�V����(CamelEnemy)
	//----------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CamelEnemyStayAnim)

	void CamelEnemyStayAnim::Enter(const shared_ptr<CamereonEnemy>& Obj)
	{
		Obj->GetComponent<PNTBoneModelDraw>()->ChangeCurrentAnimation(L"CamelStay");
	}

	void CamelEnemyStayAnim::Execute(const shared_ptr<CamereonEnemy>& Obj)
	{

	}

	void CamelEnemyStayAnim::Exit(const shared_ptr<CamereonEnemy>& Obj)
	{

	}

	//----------------------------------------------------------------------
	/// Find�A�j���[�V����(CamelEnemy)
	//----------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CamelEnemyFindAnim)

	void CamelEnemyFindAnim::Enter(const shared_ptr<CamereonEnemy>& Obj)
	{
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"CamelFind");
	}

	void CamelEnemyFindAnim::Execute(const shared_ptr<CamereonEnemy>& Obj)
	{

	}

	void CamelEnemyFindAnim::Exit(const shared_ptr<CamereonEnemy>& Obj)
	{
	}


}