/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#pragma warning(disable: 4005)
#pragma warning(disable: 4995) 


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
#include <dinput.h>
#include <Dinputd.h>
#include <XInput.h>
#include <D3Dcompiler.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

/// Linking ///
#pragma comment(lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

// DirectInput
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "XInput.lib")

#include <xinput.h>

#include <io.h>
#include <fcntl.h>

#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include <assert.h> 
#include <conio.h>
#include <chrono>
#include <filesystem>
#include <memory>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <exception>
#include <vector>
#include <thread>

#pragma region Math
#include <Math\MathDefs.h>
#include <Math\Vector2D.h>
#include <Math\Vector3D.h>
#include <Math\Matrix2x2.h>
#include <Math\Matrix3x3.h>
#include <Math\Matrix4x4.h>
#include <Math\MathFunctions.h>

#include <Physics\physics_math.h>
#include <Physics\particle.h>
#include <Physics\Quaternion.h>
#pragma endregion

#pragma region Resources
#include <Singleton.h>
#include <Color.h>
#include <UtilFunctions.h>

using json = nlohmann::json;
#include <JsonReader.h>
using namespace JsonReader;
// TODO: This should probably be moved to a file
typedef unsigned int InfectGUID;

#include <WindowProc.h>
#include <WindowSettings.h>
#include <Face.h>
#include <GameObject\GameObjectTags.h>
#include <GameObject\ComponentTypes.h>
#include <AI\FSM\AI_StateTypes.h>
#include <ThreadType.h>
#include <Command.h>
#include <ThreadCommandList.h>
#include <ThreadContainer.h>
#include <RenderThreadContainer.h>
#include <SimulationThreadContainer.h>
#include <Event.h>
#include <SubscriberTracker.h>
#include <Subscriber.h>
#include <ThreadContainer.h>

#include <RenderTarget.h>
#include <BackBufferRenderTarget.h>
#include <D3DHandler.h>
#include <Mesh.h>
#include <Plane.h>
#include <Sphere.h>
#include <Cube.h>
#include <ConstantBuffer.h>
#include <Shader.h>
#include <ShaderProgram.h>
#include <Sorting.h>
//#include <Shape.h>
#include <Physics\RigidBody.h>
#include <Physics\contacts.h>
#include <Physics\collide_coarse.h>
#include <Physics\collide_fine.h>
#pragma endregion

#pragma region Game Object
#include <GameObject\Component.h>
#include <GameObject\GameObject.h>
#pragma endregion

#pragma region Components
#include <GameObject\TransformComponent.h>

//#include <Body.h>
//
// Components - Rendering
#include <GameObject\RenderableComponent.h>
#include <GameObject\MeshComponent.h>
#include <GameObject\ParticleEmitterComponent.h>
//#include <FBOSprite.h>
//#include <Sprite.h>
//#include <Text.h>
//#include <Animation.h>
#include <GameObject\CameraComponent.h>
#include <GameObject\LightBaseComponent.h>
#include <GameObject\PointLightComponent.h>
#include <GameObject\DirectionalLightComponent.h>
#include <GameObject\RigidBodyComponent.h>
#include <GameObject\FractureComponent.h>
//#include <HeightMap.h>

//Components - Audio
//#include <Audio.h>
//#include <Microphone.h>
//
//Components - UI

// Components - AI
#include <AI\BrainComponent.h>
#include <AI\ControllerComponent.h>
#pragma endregion 

#pragma region AI
/* AI Command Library */
#include <AI\AI_Commands.h>
/* Finite-State Machine */
#include <AI\FSM\AI_State.h>
#include <AI\FSM\AI_Idle.h>
#include <AI\FSM\AI_Combat.h>
/* Behavior Tree */
#include <AI\BT\Behavior.h>
// Control Nodes
#include <AI\BT\Composite.h>
#include <AI\BT\Sequence.h>
#include <AI\BT\Selector.h>
#include <AI\BT\SimpleParallel.h>
// Decorator Nodes
#include <AI\BT\Decorator.h>
#include <AI\BT\Conditional_Loop.h>
// Action Nodes
#include <AI\BT\Action.h>
#include <AI\BT\MoveToRandom.h>
#include <AI\BT\Attack.h>

#include <AI\BT\BehaviorTree.h>
#pragma endregion

#pragma region Component Managers
#include <GameObject\ComponentManager.h>
#include <GameObject\ComponentManagerContainer.h>
#include <GameObject\TransformComponentManager.h>
#include <GameObject\CameraComponentManager.h>
#include <GameObject\MeshComponentManager.h>
#include <GameObject\ParticleEmitterComponentManager.h>
#include <GameObject\PointLightComponentManager.h>
#include <GameObject\DirectionalLightComponentManager.h>
#include <GameObject\RigidBodyComponentManager.h>
#include <GameObject\FractureComponentManager.h>
#include <AI\BrainComponentManager.h>
#include <AI\ControllerComponentManager.h>
#pragma endregion

#pragma region Globals + API
#include <JobManager.h>
#include <GUID.h>
#include <GameConfig.h>
#include <GameStateManager.h>
#include <InputManager.h>
#include <FrameRateManager.h>
#include <GameObjectManager.h>
#include <RenderManager.h>
#include <ResourceManager.h>
//#include <PhysicsManager.h>
#include <EventManager.h>
#include <LevelManager.h>
//#include <DebugManager.h>
#include <AudioManager.h>
#include <GameObject\ComponentFactory.h>
#include <AI\AI_Factory.h>
#include <MemoryManager.h>
//#include <ImguiManager.h>
//#include <LevelEditor.h>
//#include <LuaScript.h>
//#include <UIManager.h>
//#include <PlayerStatsManager.h>
//
#include <Physics\PhysicsManager.h>
#include <Physics\PhysicsTest.h>
#include <Global.h>
#include <InfectAPI.h> 
#pragma endregion 

#pragma warning(default: 4005)
#pragma warning(default: 4995)