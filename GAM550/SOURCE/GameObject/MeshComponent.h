/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
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
	ID3D11ShaderResourceView * mp_NormalMap;
	ID3D11ShaderResourceView * mp_SpecMap;
	bool m_CastShadows;
	bool m_ReceiveShadows;
	bool m_IsLit;

public:
	static const unsigned CACHESIZE = 300;
	static const ComponentType Type = ComponentType::C_Mesh;
	virtual ComponentType GetType() const { return Type; }

	MeshComponent(InfectGUID guid);
	~MeshComponent();
	static Component* CreateInstance(InfectGUID guid) { return new MeshComponent(guid); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);

	inline const Scene* GetScene() const { return mp_Scene; }
	void SetScene(const std::string & sceneName);

	inline ID3D11ShaderResourceView* GetDiffuseTexture() const { return mp_Diffuse; }
	void SetDiffuseTexture(const std::string& textureName);

	inline ID3D11ShaderResourceView* GetNormalTexture() const { return mp_NormalMap; }
	void SetNormalTexture(const std::string& textureName);

	inline ID3D11ShaderResourceView* GetSpecularTexture() const { return mp_SpecMap; }
	void SetSpecularTexture(const std::string& textureName);

	inline bool CastShadows() const { return m_CastShadows; }
	void SetCastShadows(bool castShadows);

	inline bool ReceiveShadows() const { return m_ReceiveShadows; }
	void SetReceiveShadows(bool receiveShadows);

	inline bool IsLit() const { return m_IsLit; }
	void SetIsLit(bool isLit);

	inline bool IsTextured() const { 
		return mp_Diffuse != nullptr; 
	}
};

#endif

