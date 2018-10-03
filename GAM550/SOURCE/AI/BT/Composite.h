/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#ifndef BT_COMPOSITE_H
#define BT_COMPOSITE_H

class Composite: public Behavior{
protected:
	std::vector<Behavior*> m_childBehaviors;
public:
	Composite() {};
	~Composite() {};

	virtual void OnInitialize() = 0;
	virtual BH_Status OnUpdate() = 0;
	virtual void OnTerminate(BH_Status) = 0;
};

#endif