/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#ifndef BT_SIMPLEPARALLEL_H
#define BT_SIMPLEPARALLEL_H

class SimpleParallel: public Composite{
protected:
public:
	SimpleParallel() {};
	~SimpleParallel() {};

	virtual void OnInitialize();
	virtual BH_Status OnUpdate();
	virtual void OnTerminate(BH_Status);
};

#endif