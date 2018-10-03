/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

#define INFECT_EVENTS Singleton<EventManager>::GetInstance()
#define INFECT_GUID Singleton<GUID_Generator>::GetInstance()
#define INFECT_CMC Singleton<ComponentManagerContainer>::GetInstance()
#define INFECT_GAME_CONFIG Singleton<GameConfig>::GetInstance()
#define INFECT_GOM Singleton<GameObjectManager>::GetInstance()
#define INFECT_RENDERER Singleton<RenderManager>::GetInstance()
#define INFECT_RESOURCES Singleton<ResourceManager>::GetInstance()
#define INFECT_AUDIOMANAGER Singleton<AudioManager>::GetInstance()
#define INFECT_GAME_STATE Singleton<GameStateManager>::GetInstance()
#define INFECT_FRAMERATE Singleton<FrameRateManager>::GetInstance()
#define INFECT_INPUT Singleton<InputManager>::GetInstance()
#define INFECT_MEMORY Singleton<MemoryManager>::GetInstance()
#define INFECT_THREAD_JOBS Singleton<JobManager>::GetInstance()
#define INFECT_LEVEL Singleton<LevelManager>::GetInstance()
#define INFECT_PHYSICS Singleton<PhysicsManager>::GetInstance()

#endif