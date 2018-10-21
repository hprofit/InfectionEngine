/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

void DirectionalLightComponent::_GenerateWeights()
{
	//int size = m_BlurAmount + 1;
	//// m_Weights will always be a multiple of 4 in size
	//m_Weights.resize(size + (4 - size % 4));
	//int i;
	//for (i = -m_BlurAmount; i <= 0; ++i)
	//{
	//	m_Weights[i + m_BlurAmount] = GetGaussianWeight(FLOAT(m_BlurAmount), i);
	//}

    int size = m_BlurAmount * 2 + 1;
    m_Weights.resize(size + (4 - size % 4));
    int i;
    for (i = -m_BlurAmount; i <= m_BlurAmount; ++i) 
        m_Weights[i + m_BlurAmount] = GetGaussianWeight(FLOAT(m_BlurAmount), i);

    float total = 0;
    for (i = -m_BlurAmount; i <= m_BlurAmount; ++i)
    {
    	//int offset = i <= 0 ? i + m_BlurAmount : m_BlurAmount - i;
        int offset = i + m_BlurAmount;
    	total += m_Weights[offset];
    }

    for (i = -m_BlurAmount; i <= m_BlurAmount; ++i)
    {
    	m_Weights[i + m_BlurAmount] = m_Weights[i + m_BlurAmount] / total;
    }

    // Check that the weights sum to 1
    total = 0;
    for (i = -m_BlurAmount; i <= m_BlurAmount; ++i)
    {
    	//int offset = i <= 0 ? i + m_BlurAmount : m_BlurAmount - i;
        int offset = i + m_BlurAmount;
    	total += m_Weights[offset];
    }
}

void DirectionalLightComponent::_GenerateOffsets(FLOAT width, FLOAT height)
{
    int size = m_BlurAmount + 1;
	m_OffsetsX.resize(size + (4 - size % 4));
	m_OffsetsY.resize(size + (4 - size % 4));

	FLOAT pixelWidth = 1.f / width;
	FLOAT pixelHeight = 1.f / height;

	for (int i = -m_BlurAmount; i <= 0; ++i)
	{
		int offset = i + m_BlurAmount;
		m_OffsetsX[offset] = -i * pixelWidth;
		m_OffsetsY[offset] = -i * pixelHeight;
	}
}

DirectionalLightComponent::DirectionalLightComponent(InfectGUID guid) :
	LightBaseComponent(guid, true),
	m_Far(1000000.f), 
	m_FoV(90.f),
	m_ViewPerspMat(Matrix4x4())
{
}

void DirectionalLightComponent::Deactivate()
{
	mp_Parent = nullptr;
}

void DirectionalLightComponent::Serialize(const json & j)
{
	LightBaseComponent::Serialize(j);
	m_FoV = ParseFloat(j, "fov");
	if (ValueExists(j, "far"))
		m_Far = ParseFloat(j, "far");

	UINT height, width;
	height = ParseInt(j, "height");
	width = ParseInt(j, "width");

	m_AspectRatio = FLOAT(width) / FLOAT(height);

	m_BlurAmount = ParseInt(j, "blur");
	_GenerateWeights();
	_GenerateOffsets(width, height);

	m_IsDirty = true;
}

void DirectionalLightComponent::Override(const json & j)
{
	LightBaseComponent::Serialize(j);
	if (ValueExists(j, "fov"))
		m_FoV = ParseFloat(j, "fov");
	if (ValueExists(j, "far"))
		m_Far = ParseFloat(j, "far");

	if (ValueExists(j, "height") || ValueExists(j, "width")) {

		UINT height, width;
		height = ParseInt(j, "height");
		width = ParseInt(j, "width");

		m_AspectRatio = FLOAT(width) / FLOAT(height);
	}

	m_IsDirty = true;
}

void DirectionalLightComponent::SetFar(FLOAT _far)
{
	m_Far = _far;
	m_IsDirty = true;
}

void DirectionalLightComponent::SetFoV(FLOAT _fov)
{
	m_FoV = _fov;
	m_IsDirty = true;
}