/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

class DirectionalLightComponentManager;

class DirectionalLightComponent : public LightBaseComponent
{
protected:
	FLOAT m_Far;
	FLOAT m_FoV;
	FLOAT m_AspectRatio;
	int m_BlurAmount;
	std::vector<FLOAT> m_Weights;
	std::vector<FLOAT> m_OffsetsX;	// u offset
	std::vector<FLOAT> m_OffsetsY;	// v offset

	Matrix4x4 m_ViewPerspMat;
	Matrix4x4 m_ShadowMat;

	//RenderTarget * mp_LightRenderTarget;

	void _GenerateWeights();
	void _GenerateOffsets(FLOAT width, FLOAT height);
public:
	friend DirectionalLightComponentManager;

	static const unsigned CACHESIZE = 300;
	static const ComponentType Type = ComponentType::C_DirectionalLight;
	virtual ComponentType GetType() const { return Type; }

	DirectionalLightComponent(InfectGUID guid);
	~DirectionalLightComponent() {};

	static Component* CreateInstance(InfectGUID guid) { return new DirectionalLightComponent(guid); }
	virtual void Deactivate();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);

	inline FLOAT GetFar() const { return m_Far; }
	void SetFar(FLOAT _far);

	inline FLOAT GetFoV() const { return m_FoV; }
	void SetFoV(FLOAT _fov);

	inline int BlurAmount() const { return m_BlurAmount; }

	inline std::vector<FLOAT> Weights() const { return m_Weights; }
	inline std::vector<FLOAT> OffsetsX() const { return m_OffsetsX; }
	inline std::vector<FLOAT> OffsetsY() const { return m_OffsetsY; }

	inline Matrix4x4 GetViewPerspMatrix() const { return m_ViewPerspMat; }
	inline Matrix4x4 GetShadowMatrix() const { return m_ShadowMat; }

	//inline RenderTarget* GetRenderTarget() const { return mp_LightRenderTarget; }
	//void ClearRenderTarget();
	//void BindRenderTarget();
};

#endif

