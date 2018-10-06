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
#define COLOR_OVERRIDE		8
#define TINT_COLOR			16

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
	CompressedColor m_Color;
	Color m_Specular;
	bool m_CastShadows;			// Convert these to flags
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

	// Returns this GameObject's scene
	inline const Scene* GetScene() const { return mp_Scene; }
	// Sets this GameObject's scene
	void SetScene(const std::string & sceneName);

	// Returns the Diffuse color texture for this GameObject
	inline ID3D11ShaderResourceView* GetDiffuseTexture() const { return m_Textures[TextureType::DiffuseTexture]; }
	// Sets the Diffuse color texture for this GameObject, the values in this texture will override the vertex
	// colors of the mesh and m_Color member of this GameObject
	void SetDiffuseTexture(const std::string& textureName);

	// Returns the Normal Map texture for this GameObject
	inline ID3D11ShaderResourceView* GetNormalTexture() const { return m_Textures[TextureType::NormalMap]; }
	// Sets the Normal Map texture for this GameObject, the values in this texture will move the normals of
	// the GameObject
	void SetNormalTexture(const std::string& textureName);

	// Returns the Specular Map texture for this GameObject
	inline ID3D11ShaderResourceView* GetSpecularTexture() const { return m_Textures[TextureType::SpecularMap]; }
	// Sets the Specular Map texture for this GameObject, the values in the texture will override the color 
	// portion of this GameObject's m_Specular member
	void SetSpecularTexture(const std::string& textureName);

	// Returns all textures associated with this GameObject's mesh, see m_Textures for more details
	inline ID3D11ShaderResourceView*const* GetTextures() const { return &(m_Textures[0]); }

	inline bool CastShadows() const { return m_CastShadows; }
	void SetCastShadows(bool castShadows);

	inline bool ReceiveShadows() const { return m_ReceiveShadows; }
	void SetReceiveShadows(bool receiveShadows);

	// Returns true if this GameObject is to be lit normally or false if it's full bright
	inline bool IsLit() const { return m_IsLit; }
	// Toggles whether or not this GameObject is full bright or lit normally
	void SetIsLit(bool isLit);

	// Returns this GameObject's flags regarding the values set, see m_TextureFlags for more details
	inline unsigned int TexturedFlags() const { return m_TextureFlags; }

	// Returns the override color for this GameObject's mesh
	// If none was set, the color will be clear
	inline CompressedColor GetOverrideColor() const { return m_Color; }
	// Sets the override color for this GameObject's mesh
	// If the mesh doesn't have a diffuse texture, this color will be the color of the mesh
	void SetOverrideColor(const CompressedColor & color);

	// Returns the specular values for this GameObject's mesh
	// r, g, b are all specular color values, a is the specular coefficient
	inline Color GetSpecularValues() const { return m_Specular; }

	// Sets the specular values for this GameObject's mesh
	// r, g, b are all specular color values, a is the specular coefficient
	void SetSpecularValues(const Color& specular);
};

#endif

