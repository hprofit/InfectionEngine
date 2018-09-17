/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

class MeshComponentManager;

class MeshComponent : public RenderableComponent
{
protected:	
	friend MeshComponentManager;

	Scene* mp_Scene;		// Pointer to a Scene that contains meshes for rendering
	ID3D11ShaderResourceView * mp_Diffuse;
	//SurfaceTextureBuffer * mp_NormalMap;
	//SurfaceTextureBuffer * mp_SpecMap;
	bool m_CastShadows;
	bool m_ReceiveShadows;
	bool m_IsLit;

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
	void SetScene(const std::string & sceneName);

	inline ID3D11ShaderResourceView* GetDiffuseTexture() const { return mp_Diffuse; }
	void SetDiffuseTexture(const std::string& textureName);

	inline bool CastShadows() const { return m_CastShadows; }
	void SetCastShadows(bool castShadows);

	inline bool ReceiveShadows() const { return m_ReceiveShadows; }
	void SetReceiveShadows(bool receiveShadows);

	inline bool IsLit() const { return m_IsLit; }
	void SetIsLit(bool isLit);
};

#endif

