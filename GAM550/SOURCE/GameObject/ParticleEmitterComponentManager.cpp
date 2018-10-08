/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

ParticleEmitterComponentManager::ParticleEmitterComponentManager() {
	m_Components = (std::list<PEC>*)INFECT_MEMORY.GetComponentPool(ParticleEmitterComponent::Type);
}

int ParticleEmitterComponentManager::_FindUnusedParticle(PEC pec)
{
	for (UINT i = pec->m_lastUsedParticle; i < pec->m_maxParticles; i++) {
		if (pec->m_particles[i].life <= 0.f) {
			pec->m_lastUsedParticle = i;
			return i;
		}
	}
	// Cycle around to the start of the array
	for (UINT i = 0; i < pec->m_lastUsedParticle; i++) {
		if (pec->m_particles[i].life <= 0.f) {
			pec->m_lastUsedParticle = i;
			return i;
		}
	}

	return -1; // All particles are taken
}

Vector3D ParticleEmitterComponentManager::_GetSpawnPositionWithinShape()
{
	//if (!m_pSpawnShape)
	//	return Vector3D();

	//switch (m_pSpawnShape->type) {
	//	case ShapeType::ST_AABB: {
	//		AABB* aabb = static_cast<AABB*>(m_pSpawnShape);
	//		float x = RandomFloat(-aabb->halfWidth, aabb->halfWidth);
	//		float y = RandomFloat(-aabb->halfHeight, aabb->halfHeight);
	//		return Vector3D(x, y, 0.f);
	//	}
	//	case ShapeType::ST_Circle: {
	//		Circle* circle = static_cast<Circle*>(m_pSpawnShape);
	//		float d = RandomFloat(0.f, circle->radius);
	//		float rot = RandomFloat(-180.f, 180.f);
	//		return Matrix4x4::Rotate(rot, ZAXIS) * Vector3D( 0.f, 1.f * d, 0.f);
	//	}
	//	default:
	return Vector3D(0,0,0,1);
	//}
}

void ParticleEmitterComponentManager::_SpawnParticle(PEC pec, TC tc)
{
	int idx = _FindUnusedParticle(pec);
	if (idx > -1) {
		pec->m_particles[idx].life = pec->m_lifeTime;
		Vector3D pos = tc->WorldPosition() + _GetSpawnPositionWithinShape();
		pec->m_particles[idx].x = pos.x;
		pec->m_particles[idx].y = pos.y;
		pec->m_particles[idx].z = pos.z;

		float variationOffset = RandomFloat(-pec->m_angleVariation, pec->m_angleVariation);
		// TODO: UPDATE THIS
		float angleOffset = (pec->m_rotateToParentOnSpawn ? tc->GetAngleZ() : 0.f) + variationOffset;

		pec->m_particles[idx].angleOffset = angleOffset;
		pec->m_particles[idx].rotation = angleOffset * DEG_TO_RAD;

		pec->m_particles[idx].speed = RandomFloat(pec->m_speedMin, pec->m_speedMax);
		pec->m_particles[idx].animationTime = 0.f;

		switch (pec->m_textureSelection) {
		case P_TextureSelection::RANDOM:
			pec->m_particles[idx].u = float(RandomInt(0, (int)(pec->m_animCols + 1))) * pec->m_frameWidth;
			pec->m_particles[idx].v = float(RandomInt(0, (int)(pec->m_animRows + 1))) * pec->m_frameHeight;
			break;
		case P_TextureSelection::CYCLE:
			pec->m_particles[idx].animRow = 0;
			pec->m_particles[idx].animCol = 0;
			pec->m_particles[idx].u = 0.f;
			pec->m_particles[idx].v = 0.f;
			break;
		case P_TextureSelection::SINGLE:
		default:
			pec->m_particles[idx].u = 0.f;
			pec->m_particles[idx].v = 0.f;
		}
	}
}

void ParticleEmitterComponentManager::_SortParticles(PEC pec)
{
	std::sort(&pec->m_particles[0], &pec->m_particles[pec->m_maxParticles-1]);
}

void ParticleEmitterComponentManager::_UpdateParticles(PEC pec, TC tc, float deltaTime)
{
	const Vector3D Gravity = Vector3D(0, -98.f * pec->m_gravityMod, 0);
	// TODO: FIX THIS
	const Vector3D cameraPos = INFECT_GOM.GetCamera()->GetComponent<TransformComponent>()->WorldPosition();
	pec->m_liveParticleCount = 0;
	// Reset the vertex array
	memset(pec->m_vertices, 0, (sizeof(ParticleEmitterComponent::ParticleVertex) * pec->m_vertexCount));
	float t = 0.f;
	for (int i = 0; i < pec->m_maxParticles; i++) {

		ParticleEmitterComponent::Particle& p = pec->m_particles[i];

		if (p.life > 0.0f) {

			// Decrease life
			p.life -= deltaTime;
			if (p.life > 0.0f) {
				t = 1.f - (p.life / pec->m_lifeTime);
				Vector3D velocityOffset = Matrix4x4::Rotate(p.angleOffset, ZAXIS) *
					Vector3D(
						BezierInterpolation(pec->m_velocityX.points, t) * pec->m_velocityX.amplitude,
						BezierInterpolation(pec->m_velocityY.points, t) * pec->m_velocityY.amplitude,
						BezierInterpolation(pec->m_velocityZ.points, t) * pec->m_velocityZ.amplitude,
						0.f
					);
				Vector3D newVelocity = (velocityOffset * (p.speed * pec->m_directionMod)) + Gravity;
				Vector3D newPos = Vector3D(p.x, p.y, p.z, 1) + (newVelocity * deltaTime);

				if (pec->m_particlesFollowParent) {
					Vector3D movement = tc->GetMovement();
					newPos += movement;
				}
				// put the updated values into the particle
				newPos.FillValues(p.x, p.y, p.z);
				newVelocity.FillValues(p.vX, p.vY, p.vZ);

				p.scale = BezierInterpolation(pec->m_scale.points, t) * pec->m_scale.amplitude;

				// TODO: decide on a better way to get a camera
				p.cameraDistance = Vector3D::SquareDistance(newPos, cameraPos);

				p.color = Lerp(pec->m_colorStart, pec->m_colorEnd, t);

				if (pec->m_textureSelection == P_TextureSelection::CYCLE) {
					p.animationTime += deltaTime;
					if (p.animationTime >= pec->m_animationSpeed) {
						p.animationTime = 0.f;
						++p.animCol;
						if (p.animCol >= pec->m_animCols) {
							p.animCol = 0;
							++p.animRow;

							if (p.animRow >= pec->m_animRows)
								p.animRow = 0;
						}
					}

					p.u = float(p.animCol) * pec->m_frameWidth;
					p.v = float(p.animRow) * pec->m_frameHeight;
				}
			}
			else {
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.cameraDistance = -1.0f;
			}
			++pec->m_liveParticleCount;
		}
	}


	//pec->m_vertices[pec->m_liveParticleCount].FillVert(p);

	//// Fill the GPU buffer
	////m_positionsScales[4 * m_liveParticleCount + 0] = p.m_pos_rot_animRow.x;	// Location
	////m_positionsScales[4 * m_liveParticleCount + 1] = p.m_pos_rot_animRow.y;	// Location
	////m_positionsScales[4 * m_liveParticleCount + 2] = p.m_pos_rot_animRow.z;	// Rotation

	////m_positionsScales[4 * m_liveParticleCount + 3] = p.m_scale;

	////m_colors[4 * m_liveParticleCount + 0] = p.m_color.r;
	////m_colors[4 * m_liveParticleCount + 1] = p.m_color.g;
	////m_colors[4 * m_liveParticleCount + 2] = p.m_color.b;
	////m_colors[4 * m_liveParticleCount + 3] = p.m_color.a;

	////m_textureCoords[2 * m_liveParticleCount + 0] = p.m_texCoords[0];
	////m_textureCoords[2 * m_liveParticleCount + 1] = p.m_texCoords[1];
}

bool ParticleEmitterComponentManager::_UpdateParticleBuffers(PEC pec)
{
	UINT index, i;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ParticleEmitterComponent::ParticleVertex* verticesPtr;

	// Initialize vertex array to zeros at first.
	memset(pec->m_vertices, 0, (sizeof(ParticleEmitterComponent::ParticleVertex) * pec->m_vertexCount));

	// Now build the vertex array from the particle list array.  Each particle is a quad made out of two triangles.
	index = 0;

	for (i = 0; i < pec->m_liveParticleCount; i++)
	{
		// Bottom left.
		pec->m_vertices[index].FillVert(pec->m_particles[i], 0.f, 1.f, -1, -1);
		index++;

		// Top left.
		pec->m_vertices[index].FillVert(pec->m_particles[i], 0.f, 0.f, -1, 1);
		index++;

		// Bottom right.
		pec->m_vertices[index].FillVert(pec->m_particles[i], 1.f, 1.f, 1, -1);
		index++;

		// Bottom right.
		pec->m_vertices[index].FillVert(pec->m_particles[i], 1.f, 1.f, 1, -1);
		index++;

		// Top left.
		pec->m_vertices[index].FillVert(pec->m_particles[i], 0.f, 0.f, -1, 1);
		index++;

		// Top right.
		pec->m_vertices[index].FillVert(pec->m_particles[i], 1.f, 0.f, 1, 1);
		index++;
	}

	// Lock the vertex buffer.
	if (FAILED(INFECT_RENDERER.DeviceContext()->Map(pec->mp_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (ParticleEmitterComponent::ParticleVertex*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)pec->m_vertices, (sizeof(ParticleEmitterComponent::ParticleVertex) * pec->m_vertexCount));

	// Unlock the vertex buffer.
	INFECT_RENDERER.DeviceContext()->Unmap(pec->mp_vertexBuffer, 0);

	return true;
}

void ParticleEmitterComponentManager::_WarmSystem(PEC pec)
{
	const float step = 0.1f;
	TC tc = pec->Parent()->GetComponent<TransformComponent>();
	for (float timer = 0.f; timer < pec->m_loopDuration; timer += step) {
		pec->m_emissionTimer += step;

		while (pec->m_emissionTimer > pec->m_emissionTime) {
			_SpawnParticle(pec, tc);
			pec->m_emissionTimer -= pec->m_emissionTime;
		}

		_UpdateParticles(pec, tc, step);
	}
}

void ParticleEmitterComponentManager::Update(float dt)
{
	for each (PEC pec in *m_Components) {
		if (!pec->IsActive()) break;

		pec->m_UpdatedLastFrame = false;
		if (pec->IsDirty()) {
			TC tc = pec->Parent()->GetComponent<TransformComponent>();
			if (pec->m_startDelay > 0.f) {
				pec->m_startDelay -= dt;
				return;
			}

			if (pec->m_currentTime < pec->m_loopDuration) {
				pec->m_currentTime += dt;
				pec->m_emissionTimer += dt;

				if (pec->m_active) {
					while (pec->m_emissionTimer > pec->m_emissionTime) {
						_SpawnParticle(pec, tc);
						pec->m_emissionTimer -= pec->m_emissionTime;
					}
				}

				if (pec->m_looping && pec->m_currentTime >= pec->m_loopDuration)
					pec->m_currentTime = 0.f;
			}

			_UpdateParticles(pec, tc, dt);
			_SortParticles(pec);
			_UpdateParticleBuffers(pec);


			pec->m_IsDirty = true;
			pec->m_UpdatedLastFrame = true;
		}
	}
}

// TODO: Super naive attempt at this
void ParticleEmitterComponentManager::HandleEvent(Event * pEvent)
{
	for each (PEC tComp in *m_Components) {
		if (!tComp->IsActive()) break;
		tComp->HandleEvent(pEvent);
	}
}

void ParticleEmitterComponentManager::Init() {
	INFECT_MEMORY.ComponentPoolInit<ParticleEmitterComponent>(ParticleEmitterComponent::Type, ParticleEmitterComponent::CACHESIZE);
}
