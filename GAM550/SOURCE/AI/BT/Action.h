 /* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef BT_ACTION_H
#define BT_ACTION_H

class Action: public Behavior{
public:
	Action() {};
	~Action() {};

	virtual void OnInitialize() = 0;
	virtual BH_Status OnUpdate() = 0;
	virtual void OnTerminate(BH_Status) = 0;
};

#endif