/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/
#pragma once

#ifndef RIGIDBODY_COMPONENT_MANAGER_H
#define RIGIDBODY_COMPONENT_MANAGER_H

class RigidBodyComponentManager :
  public ComponentManager<RigidBodyComponent>
{
protected:
  typedef RigidBodyComponent* RBC;

public:
  RigidBodyComponentManager();
  virtual ~RigidBodyComponentManager() {};


  virtual void Update(float dt);
  virtual void HandleEvent(Event* pEvent);

  void Init();
};

#endif
