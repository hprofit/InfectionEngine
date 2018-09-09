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
	//SurfaceTextureBuffer * m_pTexture;
	//SurfaceTextureBuffer * m_pNormal;
	//SurfaceTextureBuffer * m_pBump;

public:
	MeshComponent();
	~MeshComponent();
	static Component* CreateInstance() { return new MeshComponent(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize();
	void Override();

	inline const Scene* GetScene() const { return mp_Scene; }
	inline void SetScene(Scene* scene) { mp_Scene = scene; };

	//inline const GLuint GetTextureBuffer() const { return m_pTexture ? m_pTexture->bufferId : 0; }
	//inline const SurfaceTextureBuffer * GetTexture() const { return m_pTexture; }

	//inline const GLuint GetNormalTextureBuffer() const { return m_pNormal ? m_pNormal->bufferId : 0; }
	//inline const SurfaceTextureBuffer * GetNormalTexture() const { return m_pNormal; }

	//inline const GLuint GetBumpTextureBuffer() const { return m_pBump ? m_pBump->bufferId : 0; }
	//inline const SurfaceTextureBuffer * GetBumpTexture() const { return m_pBump; }
};

#endif

