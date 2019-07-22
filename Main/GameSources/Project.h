/*!
@file Project.h
@brief コンテンツ用のヘッダをまとめる
*/

#pragma once

#define BC_CALLBACK_0(__selector__,__target__,...) std::bind(&__selector__,__target__,##__VA_ARGS__)
#define BC_CALLBACK_1(__selector__,__target__,...) std::bind(&__selector__,__target__,std::placeholders::_1,##__VA_ARGS__)
#define BC_CALLBACK_2(__selector__,__target__,...) std::bind(&__selector__,__target__,std::placeholders::_1,std::placeholders::_2,##__VA_ARGS__)
#define BC_CALLBACK_3(__selector__,__target__,...) std::bind(&__selector__,__target__,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,##__VA_ARGS__)
#define BC_CALLBACK_4(__selector__,__target__,...) std::bind(&__selector__,__target__,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,##__VA_ARGS__)

#define PI 3.141593

#define Deg2Rad(deg) (float)((deg * PI) / 180.0f)
#define Rad2Deg(rad) (float)((rad * 180.0f) / PI)

#define CLAMP(x, low, high) (x > high)? high : (x < low)? low : x

#include <unordered_map>

#include "ProjectLibs\MyUtil.h"
#include "ProjectLibs\InputHandler.h"

#include "ProjectLibs\Sprite.h"
#include "ProjectLibs\UINumber.h"
#include "ProjectLibs\BillboardSquare.h"

#include "Target.h"
#include "OurGameObject.h"
#include "EfkInterface.h"
#include "ProjectShader.h"
#include "ProjectLibs\Transition.h"
#include "ProjectLibs\PostEffectRenderTarget.h"
#include "Scene.h"
#include "ProjectCamera.h"
#include "GameStage.h"
#include "SelectStage.h"
#include "Title.h"
#include "StaticObject.h"
#include "SkySphere.h"
#include "PlayerAssimililationObject.h"
#include "AliveGameObject.h"
#include "PlayerAnimationState.h"
#include "Player.h"
#include "PlayerBehaviour.h"
#include "PlayerStateMachine.h"
#include "EnemyAnimationState.h"
#include "EnemyBehavior.h"
#include "Enemy.h"
#include "NormalEnemy.h"
#include "NormalEnemyStateMachine.h"
#include "CamereonEnemy.h"
#include "CamereonEnemyStateMachine.h"
#include "Plane.h"
#include "Wall.h"
#include "Pillar.h"
#include "MiniMap.h"
#include "Goal.h"
#include "GameClear.h"
#include "MainUI.h"
#include "Over.h"
#include "Bullet.h"
#include "StageSelectStage.h"
#include "TutorialMessage.h"
#include "TutorialStage.h"
#include "TutorialBox.h"
#include "SplashStage.h"