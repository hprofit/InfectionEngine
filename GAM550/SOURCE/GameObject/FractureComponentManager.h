/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/
#pragma once

#ifndef FRACTRUE_COMPONENT_MANAGER_H
#define FRACTRUE_COMPONENT_MANAGER_H

class FractureComponentManager :
  public ComponentManager<FractureComponent>
{
protected:
  typedef FractureComponent* FC;

public:
  FractureComponentManager();
  virtual ~FractureComponentManager() {};


  virtual void Update(float dt);
  virtual void HandleEvent(Event* pEvent);

  void Init();
};

#endif
