/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/
#pragma once
#ifndef BEHAVIOR_H
#define BEHAVIOR_H

enum BH_Status {
	BH_INVALID,
	BH_SUCCESS,
	BH_FAILURE,
	BH_RUNNING,
	BH_SUSPENDED,

	BH_NUM_STATUS
};

//functor that handles child's return status
typedef std::function<void(BH_Status)> BH_Observer;

// Base Behavior Nodes
class Behavior: public Subscriber {
public:
	Behavior() {}
	~Behavior() {}

	BH_Status Tick() {
		if (m_Status == BH_INVALID) {
			OnInitialize();
		}
		m_Status = OnUpdate();
		if (m_Status != BH_RUNNING) {
			OnTerminate(m_Status);
		}
		return m_Status;
	}

	virtual void OnInitialize() = 0;
	virtual BH_Status OnUpdate() = 0;
	virtual void OnTerminate(BH_Status) = 0;

	void SetStatus(BH_Status status) { m_Status = status; }
	const BH_Status Status() { return m_Status; }

	BH_Observer m_Observer;
protected:
	BH_Status m_Status;

};

#endif