/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#pragma warning(disable: 4005)
#pragma warning(disable: 4995) 



//#include <Math\MathLibs.h>
//#include <glew.h>
//#include <GL\gl.h>
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>
//#include <External\Imgui\imgui.h>
//#include <External\Imgui\imgui_impl_sdl_gl3.h>
#include <External\json.hpp>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <dinput.h>
//#include <d3dx10math.h>

/// Linking ///
#pragma comment(lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// DirectInput
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <xinput.h>

#include <io.h>
#include <fcntl.h>

#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include <vector>
#include <assert.h> 
#include <conio.h>
#include <chrono>
#include <filesystem>
#include <memory>
#include <stdlib.h>
#include <float.h>
//#include <time.h>
#include <math.h>
#include <exception>

#pragma region Math
#include <Math\MathDefs.h>
#include <Math\Vector2D.h>
#include <Math\Vector3D.h>
#include <Math\Matrix2x2.h>
#include <Math\Matrix3x3.h>
#include <Math\Matrix4x4.h>
#include <Math\MathFunctions.h>

#include <Physics/physics_math.h>
#include <Physics/particle.h>
#include <Physics/Quaternion.h>
#pragma endregion

#pragma region Resources
#include <Singleton.h>
using json = nlohmann::json;
#include <JsonReader.h>
using namespace JsonReader;
// TODO: This should probably be moved to a file
typedef unsigned int InfectGUID;

//#include <Color.h>
//#include <Debug.h>
#include <WindowProc.h>
#include <WindowSettings.h>
#include <Face.h>
#include <GameObject\GameObjectTags.h>
#include <GameObject\ComponentTypes.h>
#include <Event.h>
#include <SubscriberTracker.h>
#include <Subscriber.h>
#include <D3DHandler.h>
//#include <SurfaceTextureBuffer.h>
//#include <FontInfo.h>
//#include <FrameBufferObject.h>
#include <Mesh.h>
//#include <Plane.h>
//#include <DebugLineMesh.h>
//#include <Shader.h>
//#include <ShaderProgram.h>
//#include <ImageRenderer.h>
#include <Sorting.h>
//#include <Shape.h>
#include <Physics/RigidBody.h>

#pragma endregion

#pragma region Game Object
#include <GameObject\Component.h>
#include <GameObject\GameObject.h>
#pragma endregion

#pragma region AI
//#include <AI_State.h>
//#include <AI_Idle.h>
//#include <AI_IdleForDuration.h>
//#include <AI_Engage.h>
//#include <AI_Attack.h>
//#include <AI_RangeAttack.h>
//#include <AI_Retreat.h>
//#include <AI_MoveToRandomPointInZone.h>
//#include <AI_WalkTowardPlayerIdle.h>
//#include <AI_WalkTowardPlayerAttack.h>
//#include <AI_WalkTowardPlayerEngage.h>
//#include <AI_DashingAttack.h>
//#include <AI_DashingEngage.h>
//#include <AI_DashingStunned.h>
//#include <AI_HitNRunAttack.h>
//#include <AI_HitNRunEngage.h>
//#include <AI_HitNRunRetreat.h>
//#include <AI_BomberAttack.h>
//#include <AI_BomberRetreat.h>
//#include <AI_Reaction.h>
//#include <AI_EnemyBatReaction.h>
//#include <AI_RatBossIdle.h>
//#include <AI_RatBossReaction.h>
//#include <AI_RatBossEngage.h>
//#include <AI_RatBossAttack.h>
//#include <AI_RatBossRetreat.h>
//#include <AI_RockBossIdle.h>
//#include <AI_RockBossReaction.h>
//#include <AI_RockBossEngage.h>
//#include <AI_RockBossAttack.h>
//#include <AI_StateFactory.h>
#pragma endregion

#pragma region Components
#include <GameObject\TransformComponent.h>

//#include <Body.h>
//
// Components - Rendering
#include <GameObject\RenderableComponent.h>
#include <GameObject\MeshComponent.h>
//#include <FBOSprite.h>
//#include <Sprite.h>
//#include <ParticleEmitter.h>
//#include <ParticleEmitterResizer.h>
//#include <Text.h>
//#include <Animation.h>
#include <GameObject\CameraComponent.h>
#include <GameObject\LightBaseComponent.h>
#include <GameObject\PointLightComponent.h>
//#include <LetterBox.h>
//#include <HeightMap.h>
//
//#include <Stamina.h>
//
//// Components - Agent
//#include <Agent.h>
//#include <Controller.h>
//#include <NPCController.h>
//
//// Components - Combat
//#include <Weapon.h>
//
////Components - Audio
//#include <Audio.h>
//#include <Microphone.h>
//
////Components - UI
//#include <FollowCursor.h>
//#include <Button.h>
//#include <EggCounter.h>
//#include <UICounter.h>
//#include <PauseMenu.h>
//#include <Canvas.h>
//#include <Slider.h>
//#include <Endpoints.h>
//#include <PowerUpButton.h>
//#include <SelectableButton.h>
//#include <ListSelector.h>
//
//#include<HillNodes.h>
//#include <CutScene.h>
//#include <CamFollow.h>
//#include <ScreenWrap.h>
//#include <SizeToScreen.h>
//#include <FollowCamera.h>
//#include <SnapToCamera.h>
//#include <Health.h>
//#include <FlashOnTakeDamage.h>
//#include <Projectile.h>
//
//#include <DestroyOnHealthZero.h>
//#include <DealDamageOnCollision.h>
//#include <ProjectileSpawner.h>
//#include <ScaleByHPStamina.h>
//#include <Collectible.h>
//#include <WaveMovement.h>
//#include <StraightLineMovement.h>
//#include <SpawnOnHealthZero.h>
//#include <Shop.h>
//#include <NotEnoughGoldFeathers.h>
//
//#include <LevelStatus.h>
//#include <Microphone.h>
//#include <ScriptedAnimation.h>
//#include <WinMessage.h>
//#include <Gate.h>
//#include <GateTriggerBox.h>
//#include <Spawner.h>
//#include <TimeToLive.h>
//#include <ApplyTinyOnDifficultyUp.h>
//#include <SplashScreen.h>
//#include <InGameCredits.h>
//#include <Spinner.h>

#pragma endregion 

#pragma region Component Managers
#include <GameObject/ComponentManager.h>
#include <GameObject/ComponentManagerContainer.h>
#include <GameObject/TransformComponentManager.h>
#include <GameObject/CameraComponentManager.h>
#include <GameObject/MeshComponentManager.h>
#include <GameObject/PointLightComponentManager.h>
#pragma endregion

#pragma region Globals + API
#include <GUID.h>
#include <GameConfig.h>
#include <GameStateManager.h>
#include <InputManager.h>
#include <FrameRateManager.h>
//#include <PostProcessing.h>
#include <GameObjectManager.h>
#include <RenderManager.h>
#include <ResourceManager.h>
//#include <PhysicsManager.h>
#include <EventManager.h>
//#include <LevelManager.h>
//#include <DebugManager.h>
#include <AudioManager.h>
//#include <MemoryManager.h>
//#include <ImguiManager.h>
//#include <LevelEditor.h>
#include <GameObject\ComponentFactory.h>
//#include <LuaScript.h>
//#include <UIManager.h>
//#include <PlayerStatsManager.h>
//
#include <Global.h>
#include <InfectAPI.h>
#pragma endregion 

#pragma warning(default: 4005)
#pragma warning(default: 4995)