
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::CreateResources()
	{
		wstring dataDir;
		// Texturesディレクトリ取得
		App::GetApp()->GetDataDirectory(dataDir);
		wstring texDir = dataDir + L"Textures\\";
		dataDir += L"Textures\\";

		wstring strTexture = dataDir + L"FadeMask.png";
		App::GetApp()->RegisterTexture(L"MASK_TX", strTexture);

		strTexture = dataDir + L"StageTX.png";
		App::GetApp()->RegisterTexture(L"STAGE_TX", strTexture);

		strTexture = dataDir + L"BGSelect.png";
		App::GetApp()->RegisterTexture(L"BGSelect_TX", strTexture);

		strTexture = dataDir + L"gage_base.png";
		App::GetApp()->RegisterTexture(L"GAUGE_BASE_TX", strTexture);

		strTexture = dataDir + L"gage_copying.png";
		App::GetApp()->RegisterTexture(L"GAUGE_COPYING_TX", strTexture);

		strTexture = dataDir + L"Result_Base.png";
		App::GetApp()->RegisterTexture(L"Clear_TX", strTexture);

		strTexture = dataDir + L"Result_Next1.png";
		App::GetApp()->RegisterTexture(L"Next_TX", strTexture);

		strTexture = dataDir + L"Result_Retry1.png";
		App::GetApp()->RegisterTexture(L"Result_Retry_TX", strTexture);

		strTexture = dataDir + L"Result_Title1.png";
		App::GetApp()->RegisterTexture(L"Result_Title_TX", strTexture);

		strTexture = dataDir + L"GameOver_Title_ON.png";
		App::GetApp()->RegisterTexture(L"TitleButton_TX", strTexture);

		strTexture = dataDir + L"GameOver_Retry_ON.png";
		App::GetApp()->RegisterTexture(L"Retry_TX", strTexture);

		strTexture = dataDir + L"MiniMapFrame_Over.png";
		App::GetApp()->RegisterTexture(L"Minimap_Frame_TX", strTexture);

		strTexture = dataDir + L"MiniMapFrame_Under.png";
		App::GetApp()->RegisterTexture(L"Minimap_Back_TX", strTexture);

		strTexture = dataDir + L"AbilityGage_base.png";
		App::GetApp()->RegisterTexture(L"GAUGE_ABILITY_BASE", strTexture);

		strTexture = dataDir + L"StageLineTX.png";
		App::GetApp()->RegisterTexture(L"STAGE_LINE_TX", strTexture);

		strTexture = dataDir + L"AbilityGage_bar.png";
		App::GetApp()->RegisterTexture(L"GAUGE_ABILITY_BAR", strTexture);

		strTexture = dataDir + L"gage_success.png";
		App::GetApp()->RegisterTexture(L"GAUGE_ABILITY_BAR_SUCCESS", strTexture);

		strTexture = dataDir + L"waku_copying.png";
		App::GetApp()->RegisterTexture(L"BLUE_SUCCESS_FRAME", strTexture);

		strTexture = dataDir + L"waku_success.png";
		App::GetApp()->RegisterTexture(L"ORANGE_SUCCESS_FRAME", strTexture);

		strTexture = dataDir + L"text_copying.png";
		App::GetApp()->RegisterTexture(L"SUCCESS_GAUGE_PROGRESS", strTexture);

		strTexture = dataDir + L"text_success.png";
		App::GetApp()->RegisterTexture(L"SUCCESS_GAUGE_FINISH", strTexture);

		strTexture = dataDir + L"Result_Next1.png";
		App::GetApp()->RegisterTexture(L"Result_Next_TX", strTexture);

		strTexture = dataDir + L"Result_Retry1.png";
		App::GetApp()->RegisterTexture(L"Result_Retry_TX", strTexture);

		strTexture = dataDir + L"Result_Title1.png";
		App::GetApp()->RegisterTexture(L"Result_Title_TX", strTexture);

		strTexture = dataDir + L"StageSelect_page01.png";
		App::GetApp()->RegisterTexture(L"Stage_1", strTexture);

		strTexture = dataDir + L"StageSelect_page02.png";
		App::GetApp()->RegisterTexture(L"Stage_2", strTexture);

		strTexture = dataDir + L"StageSelect_page03.png";
		App::GetApp()->RegisterTexture(L"Stage_3", strTexture);

		strTexture = dataDir + L"StageSelect_page04.png";
		App::GetApp()->RegisterTexture(L"Stage_4", strTexture);

		strTexture = dataDir + L"StageSelect_page05.png";
		App::GetApp()->RegisterTexture(L"Stage_5", strTexture);

		strTexture = dataDir + L"StageSelect_page06.png";
		App::GetApp()->RegisterTexture(L"Stage_6", strTexture);

		strTexture = dataDir + L"StageSelect_page07.png";
		App::GetApp()->RegisterTexture(L"Stage_7", strTexture);

		strTexture = dataDir + L"StageSelect_page08.png";
		App::GetApp()->RegisterTexture(L"Stage_8", strTexture);

		strTexture = dataDir + L"RedFlame.png";
		App::GetApp()->RegisterTexture(L"REDFLAME_TX", strTexture);

		strTexture = dataDir + L"StageSelect_Arrow_L.png";
		App::GetApp()->RegisterTexture(L"L_ARROW_TX", strTexture);

		strTexture = dataDir + L"StageSelect_Arrow_R.png";
		App::GetApp()->RegisterTexture(L"R_ARROW_TX", strTexture);

		strTexture = dataDir + L"overSP.png";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);

		strTexture = dataDir + L"CircleShadow.png";
		App::GetApp()->RegisterTexture(L"CIRCLE_SHADOW_TX", strTexture);

		strTexture = dataDir + L"CopyTimeGage_Base.png";
		App::GetApp()->RegisterTexture(L"COPY_TIME_GAUGE_BASE_TX", strTexture);

		strTexture = dataDir + L"CopyTimeGage_Gage.png";
		App::GetApp()->RegisterTexture(L"COPY_TIME_GAUGE_BAR_TX", strTexture);

		strTexture = dataDir + L"TutorialSlide1.png";
		App::GetApp()->RegisterTexture(L"TUTORIAL1_SLIDE1", strTexture);

		strTexture = dataDir + L"TutorialSlide2.png";
		App::GetApp()->RegisterTexture(L"TUTORIAL1_SLIDE2", strTexture);

		strTexture = dataDir + L"TutorialSlide3.png";
		App::GetApp()->RegisterTexture(L"TUTORIAL1_SLIDE3", strTexture);

		strTexture = dataDir + L"TutorialSlide4.png";
		App::GetApp()->RegisterTexture(L"TUTORIAL2_SLIDE1", strTexture);

		strTexture = dataDir + L"TutorialSlide5.png";
		App::GetApp()->RegisterTexture(L"TUTORIAL3_SLIDE1", strTexture);

		strTexture = dataDir + L"Credit1.png";
		App::GetApp()->RegisterTexture(L"CREDIT_1_TX", strTexture);

		strTexture = dataDir + L"Credit2.png";
		App::GetApp()->RegisterTexture(L"CREDIT_2_TX", strTexture);

		//StaticModelMesh = MeshResource::CreateStaticModelMesh(dataDir, L"Enemy0523.bmf");
		//App::GetApp()->RegisterResource(L"ENEMY_MESH", StaticModelMesh);

		//App::GetApp()->RegisterTexture(L"rockMD", strTexture);

		wstring modelDir;
		App::GetApp()->GetDataDirectory(modelDir);
		modelDir += L"Models\\";

		auto playerModel = MeshResource::CreateBoneModelMesh(modelDir, L"Player0523.bmf");
		App::GetApp()->RegisterResource(L"PLAYER_MESH", playerModel);
		strTexture = modelDir + L"PlayerTX_Line0509.png";
		App::GetApp()->RegisterTexture(L"PLAYER_LINE_TEX", strTexture);

		auto pillarModel = MeshResource::CreateStaticModelMesh(modelDir, L"CR_object516.bmf",true);
		App::GetApp()->RegisterResource(L"PILLAR_MESH", pillarModel);
		strTexture = modelDir + L"object516_Light.png";
		App::GetApp()->RegisterTexture(L"PILLAR_LINE_TX", strTexture);

		auto pillarBone = MeshResource::CreateBoneModelMesh(modelDir, L"BonePillar.bmf",true);
		App::GetApp()->RegisterResource(L"PILLAR_BONE_MESH", pillarBone);

		auto enemyModel = MeshResource::CreateBoneModelMesh(modelDir, L"Enemy0523.bmf",true);
		App::GetApp()->RegisterResource(L"ENEMY_MESH", enemyModel);
		strTexture = modelDir + L"Enemy_Light.png";
		App::GetApp()->RegisterTexture(L"ENEMY_LINE_TX", strTexture);

		auto camelModel = MeshResource::CreateBoneModelMesh(modelDir, L"CamelEnemy0625.bmf", true);
		App::GetApp()->RegisterResource(L"CAMELEON_MESH", camelModel);
		strTexture = modelDir + L"camel_Light.png";
		App::GetApp()->RegisterTexture(L"CAMEL_LINE_TX", strTexture);

		auto StaticModelMesh = MeshResource::CreateStaticModelMesh(modelDir, L"StageSelect_mozi0516.bmf");
		App::GetApp()->RegisterResource(L"STAGESELECT_MESH", StaticModelMesh);

		StaticModelMesh = MeshResource::CreateStaticModelMesh(modelDir, L"StageSelect_ring0516.bmf");
		App::GetApp()->RegisterResource(L"SELECT_RING_MESH", StaticModelMesh);
		strTexture = modelDir + L"ring_Light.png";
		App::GetApp()->RegisterTexture(L"SELECT_RING_LINE_TEX", strTexture);

		auto crystalTableModel = MeshResource::CreateBoneModelMesh(modelDir, L"ObjectB.bmf",true);
		App::GetApp()->RegisterResource(L"CRYSTALTABLE_MESH", crystalTableModel);
		strTexture = modelDir + L"ObjectB_Light.png";
		App::GetApp()->RegisterTexture(L"CRYSTALTABLE_LINE_TEX", strTexture);

		auto targetModel = MeshResource::CreateBoneModelMesh(modelDir, L"Targetanime.bmf");
		App::GetApp()->RegisterResource(L"TARGET_MESH", targetModel);
		strTexture = modelDir + L"TargetTX.png";
		App::GetApp()->RegisterTexture(L"TARGET_TX", strTexture);

		auto playerEgg = MeshResource::CreateBoneModelMesh(modelDir, L"PlayerEgg.bmf");
		App::GetApp()->RegisterResource(L"EGG_MESH", playerEgg);
		strTexture = modelDir + L"PlayerEGG_Light.png";
		App::GetApp()->RegisterTexture(L"EGG_LINE_TX", strTexture);

		auto bullet = MeshResource::CreateStaticModelMesh(modelDir, L"Bullet.bmf");
		App::GetApp()->RegisterResource(L"BULLET_MESH", bullet);
		strTexture = modelDir + L"bullet.png";
		App::GetApp()->RegisterTexture(L"BULLET_TX", strTexture);

		auto goal = MeshResource::CreateBoneModelMesh(modelDir, L"Goal_Obj0-360.bmf");
		App::GetApp()->RegisterResource(L"GOAL_MESH", goal);
		strTexture = modelDir + L"goal.png";
		App::GetApp()->RegisterTexture(L"GOAL_TX", strTexture);

		//BGM
		wstring audioDir;
		App::GetApp()->GetDataDirectory(audioDir);
		audioDir += L"Audio\\";
		strTexture = audioDir + L"SE\\m-art_OK4.wav";
		App::GetApp()->RegisterWav(L"OK", strTexture);
		strTexture = audioDir + L"SE\\m-art_Select3.wav";
		App::GetApp()->RegisterWav(L"Select", strTexture);
		strTexture = audioDir + L"SE\\se_maoudamashii_system20.wav";
		App::GetApp()->RegisterWav(L"NO", strTexture);
		strTexture = audioDir + L"SE\\koukaonrab_arm-action4.wav";
		App::GetApp()->RegisterWav(L"Select2", strTexture);
		strTexture = audioDir + L"SE\\koukaonrab_data-analysis1.wav";
		App::GetApp()->RegisterWav(L"Gage", strTexture);
		wstring strMusic = audioDir + L"Stage_BG.wav";
		App::GetApp()->RegisterWav(L"GAMESTAGE_BG", strMusic);

		strMusic = audioDir + L"Title_BG.wav";
		App::GetApp()->RegisterWav(L"TITLE_BG", strMusic);

		strMusic = audioDir + L"StageClear_BG.wav";
		App::GetApp()->RegisterWav(L"STAGECLEAR_BG", strMusic);

		strMusic = audioDir + L"GameOver_BG.wav";
		App::GetApp()->RegisterWav(L"GAMEOVER_BG", strMusic);

		//SE
		strMusic = audioDir + L"GameClear_JG3.wav";
		App::GetApp()->RegisterWav(L"GAMECLEAR_JG", strMusic);

		strMusic = audioDir + L"GameOver_BG.wav";
		App::GetApp()->RegisterWav(L"GAMEOVER_BG", strMusic);

		strMusic = audioDir + L"Warning_SE.wav";
		App::GetApp()->RegisterWav(L"WARNING_SE", strMusic);

		strMusic = audioDir + L"UIOpen.wav";
		App::GetApp()->RegisterWav(L"UI_OPEN_SE", strMusic);

		strMusic = audioDir + L"UIClose.wav";
		App::GetApp()->RegisterWav(L"UI_CLOSE_SE", strMusic);

		strMusic = audioDir + L"Success.wav";
		App::GetApp()->RegisterWav(L"SUCCESS_SE", strMusic);

		strMusic = audioDir + L"EggOpen.wav";
		App::GetApp()->RegisterWav(L"EGG_OPEN_SE", strMusic);

		strMusic = audioDir + L"EggAppear.wav";
		App::GetApp()->RegisterWav(L"EGG_APPEAR_SE", strMusic);

		strMusic = audioDir + L"GameOver_JG2.wav";
		App::GetApp()->RegisterWav(L"GAME_OVER_JINGLE",strMusic);

		strMusic = audioDir + L"Warp.wav";
		App::GetApp()->RegisterWav(L"WARP_SE", strMusic);

		strMusic = audioDir + L"Inhale_SE.wav";
		App::GetApp()->RegisterWav(L"INHALE_SE", strMusic);

		strMusic = audioDir + L"Transform_Up_SE.wav";
		App::GetApp()->RegisterWav(L"TRANSFORM_UP_SE", strMusic);

		strMusic = audioDir + L"Transform_Down_SE.wav";
		App::GetApp()->RegisterWav(L"TRANSFORM_DOWN_SE", strMusic);

		strMusic = audioDir + L"Start.wav";
		App::GetApp()->RegisterWav(L"START_SE", strMusic);

		strMusic = audioDir + L"GameOver.wav";
		App::GetApp()->RegisterWav(L"GAMEOVER_SE", strMusic);
	}

	void Scene::OnCreate() 
	{
		try 
		{
			CreateResources();

			m_Transition = ObjectFactory::Create<Transition>();

			for (int i = 1; i <= 8; i++)
			{
				auto stage = ObjectFactory::Create<StageSelectStage>(i);
				m_StageSelectStages.push_back(stage);
			}

			wstring dataDir;
			App::GetApp()->GetDataDirectory(dataDir);
			m_LoadingAnim = ObjectFactory::Create<SpriteStudioAnim>(nullptr,dataDir + L"LoadingAnime\\", L"loading.ssae", L"anime_1");

			//クリアする色を設定
			Col4 Col;
			Col.set(31.0f / 255.0f,30.0f / 255.0f,71.0f / 255.0f,255.0f / 255.0f);
			SetClearColor(Col);
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToSplash");
		}
		catch (...)
		{
			throw;
		}
	}

	void Scene::OnUpdate()
	{
		if (m_IsThreadLoadingStage)
		{
			m_LoadingAnim->OnUpdate();
		}
		else
		{
			auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
			if (pad.bConnected)
			{
				if ((pad.wButtons & XINPUT_GAMEPAD_START) &&
					(pad.wButtons & XINPUT_GAMEPAD_BACK))
				{
					if (!dynamic_pointer_cast<Title>(GetActiveStage()))
					{
						PostEvent(0, App::GetApp()->GetScene<Scene>(), App::GetApp()->GetScene<Scene>(), L"ToTitle");
						return;
					}
				}
			}

			m_Transition->OnUpdate();
			if (m_Transition->GetNowFadeOut())
				return;

			SceneBase::OnUpdate();
		}
	}

	void Scene::OnDraw()
	{
			if (m_IsThreadLoadingStage)
			{
				App::GetApp()->GetDeviceResources()->ClearDefaultViews(Col4(0, 0, 0, 1));
				m_LoadingAnim->OnDraw();
			}
			else
			{
				SceneBase::OnDraw();

				m_Transition->OnDraw();
			}
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) 
	{
		auto Audioptr = App::GetApp()->GetXAudio2Manager();
		if (event->m_MsgStr == L"ToGameStage")
		{
			Audioptr->Stop(m_Audio);
			m_Audio = Audioptr->Start(L"GAMESTAGE_BG", XAUDIO2_LOOP_INFINITE, 0.5f);

			if (GetStageNum() == 0)
			{
				ResetActiveStageThread<TutorialStage1>();
				return;
			}

			//最初のアクティブステージの設定
			ResetActiveStageThread<GameStage>();
		}
		else if (event->m_MsgStr == L"ToTitle")
		{
			Audioptr->Stop(m_Audio);
			m_Audio = Audioptr->Start(L"TITLE_BG", XAUDIO2_LOOP_INFINITE, 0.6f);


			//最初のアクティブステージの設定
			ResetActiveStage<Title>();
		}
		else if (event->m_MsgStr == L"ToSelectStage")
		{
			//最初のアクティブステージの設定
			ResetActiveStageThread<SelectStage>();
		}
		else if (event->m_MsgStr == L"ToGameClear")
		{
			Audioptr->Stop(m_Audio);
			m_Audio = Audioptr->Start(L"STAGECLEAR_BG", XAUDIO2_LOOP_INFINITE, 0.6f);
			ResetActiveStage<GameClear>();
		}
		else if (event->m_MsgStr == L"ToGameOver")
		{
			Audioptr->Stop(m_Audio);
			m_Audio = Audioptr->Start(L"GAMEOVER_BG", XAUDIO2_LOOP_INFINITE, 1.5f);
			//Audioptr->Start(L"GAMEOVER_SE", 0,0.8f);
			ResetActiveStage<Over>();
		}
		else if (event->m_MsgStr == L"StopBGM")
			Audioptr->Stop(m_Audio);
		else if(event->m_MsgStr == L"StartMainBGM")
			m_Audio = Audioptr->Start(L"GAMESTAGE_BG", XAUDIO2_LOOP_INFINITE, 0.5f);
		else if (event->m_MsgStr == L"ToSplash")
		{
			ResetActiveStage<SplashStage>();
		}
	}

	void Scene::RegisterTexturePackInfo(wstring key, wstring filePath, wstring texKey)
	{
		if (m_PackdTextureInfo.find(key) != m_PackdTextureInfo.end())
		{
			throw BaseException(
				L"指定されたキーはすでに登録されています",
				L"Scene::RegisterTexturePackInfo(" + key + L"," + filePath + L"," + texKey + L")",
				L"キーをすでに登録しているか、同じ名前で登録しているか確認してください"
			);
		}
		unordered_map<wstring, TexturePackInfo> info;
		PackedTextureInfo packedInfo;
		TexturePackInfo texInfo;

		XmlDocReader* reader = new XmlDocReader(filePath);
		auto atlasNode = reader->GetSelectSingleNode(L"TextureAtlas");
		packedInfo.textureKey = texKey;
		packedInfo.width = MyUtil::wstrToInt(reader->GetAttribute(atlasNode, "width"));
		packedInfo.height = MyUtil::wstrToInt(reader->GetAttribute(atlasNode, "height"));

		auto spriteNode = reader->GetSelectNodes(L"TextureAtlas/sprite");
		texInfo.packedTextureInfo = packedInfo;
		long countNode = XmlDocReader::GetLength(spriteNode);
		for (int i = 0; i < countNode; i++)
		{
			auto node = reader->GetItem(spriteNode, i);
			texInfo.name = reader->GetAttribute(node, "n");
			texInfo.x = MyUtil::wstrToInt(reader->GetAttribute(node, "x"));
			texInfo.y = MyUtil::wstrToInt(reader->GetAttribute(node, "y"));
			texInfo.width = MyUtil::wstrToInt(reader->GetAttribute(node, "w"));
			texInfo.height = MyUtil::wstrToInt(reader->GetAttribute(node, "h"));

			info[texInfo.name] = texInfo;
		}

		m_PackdTextureInfo[key] = info;

		delete reader;
	}

}
//end basecross
