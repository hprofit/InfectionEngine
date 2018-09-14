/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

class MeshComponent : public RenderableComponent
{
protected:	
	Scene* mp_Scene;
	//SurfaceTextureBuffer * mp_DiffuseMap;
	//SurfaceTextureBuffer * mp_NormalMap;
	//SurfaceTextureBuffer * mp_SpecMap;

public:
	static const ComponentType Type = ComponentType::C_Mesh;
	virtual ComponentType GetType() const { return Type; }

	MeshComponent(InfectGUID guid);
	~MeshComponent();
	static Component* CreateInstance(InfectGUID guid) { return new MeshComponent(guid); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	void Override();

	inline const Scene* GetScene() const { return mp_Scene; }
	inline void SetScene(Scene* scene) { mp_Scene = scene; };
};

#endif

