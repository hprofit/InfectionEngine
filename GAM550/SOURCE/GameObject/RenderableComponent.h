/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RENDERABLE_COMPONENT_H
#define RENDERABLE_COMPONENT_H

class RenderableComponent : public Component
{
protected:

public:
	RenderableComponent(InfectGUID guid) : Component(guid) {};
	~RenderableComponent() {};
};

#endif