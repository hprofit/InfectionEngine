/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/
#pragma once

#ifndef COLLISIONTRIGGER_C_MANAGER_H
#define COLLISIONTRIGGER_C_MANAGER_H

class CollisionTriggerComponentManager :
  public ComponentManager<CollisionTriggerComponent>
{
protected:
  typedef CollisionTriggerComponent* RBC;

public:
  CollisionTriggerComponentManager();
  virtual ~CollisionTriggerComponentManager() {};


  virtual void Update(float dt);
  virtual void HandleEvent(Event* pEvent);

  void Init();
};

#endif
