/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

GameObject::GameObject() :
	m_isDestroy(false),
	m_isActive(false), // mem_mngr spits out active
	m_isCollisionDisabled(false),
	m_isRender(true),
	m_isSetToDestroy(false),
	m_destroyTimer(0),
	m_destroySetTimeStamp(0)
{
	std::fill_n(mComponents, int(ComponentType::NUM_COMPONENTS), nullptr);
}

GameObject::GameObject(InfectGUID id) :
	m_GUID(id), 
	m_isDestroy(false), 
	m_isActive(false), // mem_mngr spits out active
	m_isCollisionDisabled(false), 
	m_isRender(true),
	m_isSetToDestroy(false),
	m_destroyTimer(0),
	m_destroySetTimeStamp(0)
{
	std::fill_n(mComponents, int(ComponentType::NUM_COMPONENTS), nullptr);
}

GameObject::~GameObject() {
	for (int i = 0; i < ComponentType::NUM_COMPONENTS; ++i) {
		//if (mComponents[i])
		//	TETRA_MEMORY.DeleteComponent(mComponents[i]);
	}

	m_activeComponents.clear();
}

bool GameObject::operator==(const GameObject& rhs) const
{
	return rhs.m_GUID == m_GUID;
}

bool GameObject::operator!=(const GameObject & rhs) const
{
	return rhs.m_GUID != m_GUID;
}

void GameObject::Destroy() {
	m_isDestroy = true;
}

void GameObject::DestroyIn(float time) {
	m_isSetToDestroy = true;
	//m_destroySetTimeStamp = TETRA_FRAMERATE.GetElapsedTime();
	m_destroyTimer = time;
}

void GameObject::Deactivate() {
	m_isActive = true;
	m_isRender = true;
	m_destroyTimer = 0;
	m_destroySetTimeStamp = 0;
	m_isDestroy = false;
	m_isSetToDestroy = false;
	for (int i = 0; i < ComponentType::NUM_COMPONENTS; ++i) {
		if (mComponents[i]) {
			//TETRA_MEMORY.DeleteComponent(mComponents[i]);
			mComponents[i] = nullptr;
		}
	}

	m_activeComponents.clear();
}

void GameObject::OverrideComponents()
{
	//for (unsigned int i = 0; i < m_activeComponents.size(); ++i) {
	//	mComponents[(int)m_activeComponents[i]]->Override(j);
	//}
}

void GameObject::Update(float dt) {
	for (unsigned int i = 0; i < m_activeComponents.size(); ++i) {
		mComponents[(int)m_activeComponents[i]]->Update(dt);
	}

	if (m_isSetToDestroy) {
		//if (TETRA_FRAMERATE.GetElapsedTime() - m_destroySetTimeStamp > m_destroyTimer)
		//	Destroy();
	}
}

void GameObject::LateUpdate(float dt) {
	for (unsigned int i = 0; i < m_activeComponents.size(); ++i) {
		mComponents[(int)m_activeComponents[i]]->LateUpdate(dt);
	}
}

void GameObject::LateInitialize() {
	for (unsigned int i = 0; i < m_activeComponents.size(); ++i) {
		mComponents[(int)m_activeComponents[i]]->LateInitialize();
	}
}

void GameObject::AddComponent(Component* pComponent) {
	pComponent->SetParent(this);
	mComponents[pComponent->GetType()] = pComponent;
	m_activeComponents.push_back(pComponent->GetType());
}

bool GameObject::HasComponent(ComponentType type) const
{
	return mComponents[type];
}

void GameObject::HandleEvent(Event* pEvent) {
	for (unsigned int i = 0; i < m_activeComponents.size(); ++i) {
		mComponents[(int)m_activeComponents[i]]->HandleEvent(pEvent);
	}
}

void GameObject::SetActive(bool active) {
	m_isActive = active;
	m_isRender = active;
	m_isCollisionDisabled = !active;
}
