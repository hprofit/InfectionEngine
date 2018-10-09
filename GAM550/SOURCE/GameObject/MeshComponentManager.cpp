/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

MeshComponentManager::MeshComponentManager() {
	m_Components = (std::list<MeshComponent*>*)INFECT_MEMORY.GetComponentPool(MeshComponent::Type);
}

void MeshComponentManager::Update(float dt)
{
	for each (MC mComp in *m_Components) {
		if (!mComp->IsActive()) break;
		mComp->m_UpdatedLastFrame = false;
		if (mComp->IsDirty()) {

			//CALL ANIMATION PART HERE
			//Scene S;
			//cout << S.m_Animation.AnimationList[0].Duration << endl;

			mComp->m_IsDirty = false;
			mComp->m_UpdatedLastFrame = true;
		}
	}
}

// TODO: Super naive attempt at this
void MeshComponentManager::HandleEvent(Event * pEvent)
{
	for each (MC mComp in *m_Components) {
		if (!mComp->IsActive()) break;
		mComp->HandleEvent(pEvent);
	}
}

void MeshComponentManager::Init() {
	INFECT_MEMORY.ComponentPoolInit<MeshComponent>(MeshComponent::Type, MeshComponent::CACHESIZE);
}