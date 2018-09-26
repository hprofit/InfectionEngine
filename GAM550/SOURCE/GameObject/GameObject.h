/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject 
{
protected:
	Component* mComponents[ComponentType::NUM_COMPONENTS];
	std::vector<ComponentType> m_activeComponents;
	InfectGUID m_GUID;
	unsigned short m_layer;
	bool m_isSetToDestroy;
	float m_destroyTimer;
	float m_destroySetTimeStamp;
	bool m_isActive;
	bool m_isRender;
	bool m_isCollisionDisabled;
	bool m_isStatic;
	bool m_isDestroy;
	GameObjectTag m_tag;

	GameObject* mp_Parent;
	std::vector<GameObject*> m_Children;

public:
	GameObject();
	GameObject(InfectGUID id);
	~GameObject();

	bool operator==(const GameObject& rhs) const;
	bool operator!=(const GameObject& rhs) const;

	virtual void HandleEvent(Event* pEvent);
	void Update(float dt);
	void LateUpdate(float dt);
	void LateInitialize();
	void Destroy();
	void DestroyIn(float time);
	void Deactivate();

	void OverrideComponents(const json& j);

	InfectGUID ID() const { return m_GUID; }
	void SetID(InfectGUID id) { m_GUID = id; }
	
	inline void SetParent(GameObject* parent) { mp_Parent = parent; }
	inline bool HasParent() const { return mp_Parent; }
	inline GameObject* Parent() const { return mp_Parent; }

	void SetActive(bool active);
	inline bool IsActive() { return m_isActive; }
	inline bool ShouldRender() { return m_isRender; }
	
	void SwitchTag(GameObjectTag t) { m_tag = t; }
	inline GameObjectTag Tag() const { return m_tag; }
	inline bool TagIs(GameObjectTag tag) const { return m_tag == tag; }

	void AddComponent(Component* pComponent);
	bool HasComponent(ComponentType type) const;

	template <typename C>
	C* GetComponent() {
		return static_cast<C*>(mComponents[C::Type]);
	}

	template <typename C>
	const C * GetComponent() const
	{
		return static_cast<C*>(mComponents[C::Type]);
	}
};

bool operator== (GameObject & lhs, GameObject & rhs)
{
	return lhs.ID() == rhs.ID();
}

#endif 

