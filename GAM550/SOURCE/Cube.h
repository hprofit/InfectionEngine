/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef CUBE_MESH_H
#define CUBE_MESH_H

#define CUBE_PRIMITIVE "Cube"
#define SKYBOX_PRIMITIVE "SkyBox"

enum CubeFace {
	LEFT = 0, FRONT, RIGHT, BACK, TOP, BOTTOM
};

class Cube :
	public Scene
{
public:
	Cube();
	virtual ~Cube();
};

class Skybox :
	public Scene
{
public:
	Skybox();
	virtual ~Skybox();
};

#endif