/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/
#pragma once

#ifndef COLLISION_COMPONENT_MANAGER_H
#define COLLISION_COMPONENT_MANAGER_H

class CollisionComponentManager :
  public ComponentManager<CollisionComponent>
{
protected:
  typedef CollisionComponent* RBC;

public:
  CollisionComponentManager();
  virtual ~CollisionComponentManager() {};


  virtual void Update(float dt);
  virtual void HandleEvent(Event* pEvent);

  void Init();
};

#endif
