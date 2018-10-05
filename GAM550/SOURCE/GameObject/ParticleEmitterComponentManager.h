/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef PARTICLE_EMITTER_COMPONENT_MANAGER_H
#define PARTICLE_EMITTER_COMPONENT_MANAGER_H

class ParticleEmitterComponentManager :
	public ComponentManager<ParticleEmitterComponent>
{
protected:
	typedef ParticleEmitterComponent* PEC;
	typedef TransformComponent* TC;

	int _FindUnusedParticle(PEC pec);
	Vector3D _GetSpawnPositionWithinShape();
	void _SpawnParticle(PEC pec, TC tc);
	void _SortParticles(PEC pec);
	void _UpdateParticles(PEC pec, float deltaTime);
	void _AllocateParticleArrays(PEC pec);
	void _DeallocateParticleArrays(PEC pec);
	void _AllocateVBOs(PEC pec);
	void _WarmSystem(PEC pec);
public:
	ParticleEmitterComponentManager();
	virtual ~ParticleEmitterComponentManager() {};


	virtual void Update(float dt);
	virtual void HandleEvent(Event* pEvent);

	void Init();
};

#endif
