/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef EVENT_H
#define EVENT_H

#define MAX_RESOLUTIONS 4

// Macro trick to make Event names enums from the file EventNames.h
#define REGISTER_EVENT_NAME(x) x,
typedef enum
{
	#include "EventNames.h"
	EVENT_NUM
} EventType;
#undef REGISTER_EVENT_NAME

#define REGISTER_EVENT_NAME(x) #x,
static const char* EventNameText[] =
{
#include "EventNames.h"
	"Invalid"
};
#undef REGISTER_EVENT_NAME

class GameObject;

// Abstract struct, data objects should inherit from this
struct EventData {};

struct FloatData : public EventData {
	FloatData(float value) : mValue(value) {}
	float mValue;
};

struct InputButtonData : public EventData {
	InputButtonData(bool _isPressed, bool _isTrigger, bool _isReleased) : m_isPressed(_isPressed), m_isTrigger(_isTrigger), m_isReleased(_isReleased) {}
	bool m_isPressed;
	bool m_isTrigger;
	bool m_isReleased;
};

//struct InputAxisData : public EventData {
//	InputAxisData(const Vector3D& dir) : m_dir(dir) {}
//	Vector3D m_dir;
//};

class Event
{
protected:
	double m_time;		// Time left until event is broadcast to subscribers
	EventType m_type;	// Type of event
	EventData* m_data;	// Data for event, if any
public:
	Event(EventType type, double time = 0.0);
	Event(EventType type, EventData* eventData, double time = 0.0);
	virtual ~Event();

	double Time() const { return m_time; }
	EventType Type() const { return m_type; }

	template <typename T>
	T* Data() const { return static_cast<T*>(m_data); }

	void DecrementTime(double amt) { m_time -= amt; }

	static EventType GetEventTypeFromTitle(std::string eventTitle);
};

#endif