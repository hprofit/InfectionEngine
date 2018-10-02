/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#define DIFFUSE_TEXTURED	1
#define NORMAL_MAPPED		2
#define SPECULAR_MAPPED		4

class MeshComponentManager;

enum TextureType {
	DiffuseTexture = 0,
	NormalMap,
	SpecularMap,

	NUM_TEXTURE_TYPES
};

class MeshComponent : public RenderableComponent
{
protected:	
	friend MeshComponentManager;

	Scene* mp_Scene;		// Pointer to a Scene that contains meshes for rendering
	ID3D11ShaderResourceView * m_Textures[TextureType::NUM_TEXTURE_TYPES];
	bool m_CastShadows;
	bool m_ReceiveShadows;
	bool m_IsLit;
	unsigned int m_TextureFlags;

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

	inline ID3D11ShaderResourceView* GetDiffuseTexture() const { return m_Textures[TextureType::DiffuseTexture]; }
	void SetDiffuseTexture(const std::string& textureName);

	inline ID3D11ShaderResourceView* GetNormalTexture() const { return m_Textures[TextureType::NormalMap]; }
	void SetNormalTexture(const std::string& textureName);

	inline ID3D11ShaderResourceView* GetSpecularTexture() const { return m_Textures[TextureType::SpecularMap]; }
	void SetSpecularTexture(const std::string& textureName);

	inline ID3D11ShaderResourceView*const* GetTextures() const { return &(m_Textures[0]); }

	inline bool CastShadows() const { return m_CastShadows; }
	void SetCastShadows(bool castShadows);

	inline bool ReceiveShadows() const { return m_ReceiveShadows; }
	void SetReceiveShadows(bool receiveShadows);

	inline bool IsLit() const { return m_IsLit; }
	void SetIsLit(bool isLit);

	inline unsigned int TexturedFlags() const { return m_TextureFlags; }
};

#endif

