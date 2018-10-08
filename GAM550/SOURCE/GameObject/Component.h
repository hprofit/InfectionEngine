/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component : 
	public Subscriber
{
protected:
	InfectGUID m_GUID;
	GameObject *mp_Parent;
	bool m_IsDirty;
	bool m_UpdatedLastFrame;
	bool m_IsActive;
public:
	static const ComponentType Type = ComponentType::NUM_COMPONENTS;
	static const unsigned CACHESIZE = 300;

	Component() :
		m_GUID(0),
		mp_Parent(nullptr),
		m_IsDirty(false),
		m_UpdatedLastFrame(false),
		m_IsActive(false)
	{};

	Component(InfectGUID guid) :
		m_GUID(guid),
		mp_Parent(nullptr),
		m_IsDirty(false),
		m_UpdatedLastFrame(false),
		m_IsActive(false)
	{};


	virtual ~Component() {};
	virtual void LateInitialize() {};
	virtual void Serialize(const json& j) = 0;
	virtual void Deactivate() {};
	virtual void Override(const json& j) {};
	virtual void HandleEvent(Event* pEvent) {}

	virtual ComponentType GetType() const { return Type; }
	inline GameObject* Parent() { return mp_Parent; }
	inline void SetParent(GameObject* parent) { 
		mp_Parent = parent; 
		m_IsDirty = true;
	}
	inline InfectGUID ID() const { return m_GUID; }
	inline void SetID(InfectGUID guid) { m_GUID = guid; }
	inline bool IsDirty() const { return m_IsDirty; }
	inline void SetDirty(bool flag) { m_IsDirty = flag; }
	inline bool UpdatedLastFrame() const { return m_UpdatedLastFrame; }
	inline void SetActive(bool flag) { m_IsActive = flag; }
	inline bool IsActive() const { return m_IsActive; }
	inline bool operator==(const Component& comp) const { return m_GUID == comp.m_GUID; }
};
bool operator== (Component & lhs, Component & rhs)
{
	return lhs.ID() == rhs.ID();
}
#endif
