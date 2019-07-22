#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	IMPLEMENT_SINGLETON_INSTANCE(PlayerDefaultAnim)

	void PlayerDefaultAnim::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"Default");
	}

	void PlayerDefaultAnim::Execute(const shared_ptr<Player>& Obj)
	{

	}

	void PlayerDefaultAnim::Exit(const shared_ptr<Player>& Obj)
	{

	}

	IMPLEMENT_SINGLETON_INSTANCE(PlayerCopyOpenAnim)

	void PlayerCopyOpenAnim::Enter(const shared_ptr<Player>& Obj)
	{
		App::GetApp()->GetXAudio2Manager()->Start(L"EGG_OPEN_SE", 0);
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"CopyOpen");
	}

	void PlayerCopyOpenAnim::Execute(const shared_ptr<Player>& Obj)
	{

	}

	void PlayerCopyOpenAnim::Exit(const shared_ptr<Player>& Obj)
	{

	}

	IMPLEMENT_SINGLETON_INSTANCE(PlayerCopyCloseAnim)

	void PlayerCopyCloseAnim::Enter(const shared_ptr<Player>& Obj)
	{
		auto drawComp = Obj->GetDynamicComponent<SmBaseDraw>();
		
		float animTime = drawComp->GetCurrentAnimationTime();
		if (animTime >= 59.0f / 60.0f)
		{
			App::GetApp()->GetXAudio2Manager()->Start(L"EGG_OPEN_SE", 0);
			drawComp->ChangeCurrentAnimation(L"CopyClose");
		}
	}

	void PlayerCopyCloseAnim::Execute(const shared_ptr<Player>& Obj)
	{
		auto drawComp = Obj->GetDynamicComponent<SmBaseDraw>();

		if (drawComp->GetCurrentAnimation() != L"CopyClose")
		{
			float animTime = drawComp->GetCurrentAnimationTime();
			if (animTime >= 59.0f / 60.0f)
			{
				App::GetApp()->GetXAudio2Manager()->Start(L"EGG_OPEN_SE", 0);
				drawComp->ChangeCurrentAnimation(L"CopyClose");
			}
		}
		else
		{
			float animTime = drawComp->GetCurrentAnimationTime();
			if (animTime >= 59.0f / 60.0f)
			{
				Obj->GetAnimationStateMachine()->ChangeState(PlayerDefaultAnim::Instance());
			}
		}
	}

	void PlayerCopyCloseAnim::Exit(const shared_ptr<Player>& Obj)
	{

	}

	IMPLEMENT_SINGLETON_INSTANCE(PlayerGoalAnim)

		void PlayerGoalAnim::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"Goal");
	}

	void PlayerGoalAnim::Execute(const shared_ptr<Player>& Obj)
	{

	}

	void PlayerGoalAnim::Exit(const shared_ptr<Player>& Obj)
	{

	}
}