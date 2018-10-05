/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

#define SHAPE_STR "SHAPE"

ParticleEmitterComponent::ParticleEmitterComponent(InfectGUID guid) :
	RenderableComponent(guid),
	m_active(true),
	m_currentTime(0.f),
	m_emissionTime(0.f),
	m_emissionTimer(0.f),
	m_liveParticleCount(0),
	mp_texture(nullptr),
	m_scene(*INFECT_RESOURCES.GetScene(QUAD_PRIMITIVE)),
	//m_pSpawnShape(nullptr),
	m_directionMod(1.f),
	m_brightness(1.f)
{
	//TETRA_EVENTS.Subscribe(EVENT_OnLevelInitialized, this);
}

ParticleEmitterComponent::~ParticleEmitterComponent() 
{
	//_DeallocateParticleArrays();
	//if (m_pSpawnShape)
	//	delete m_pSpawnShape;
}

#pragma region Component Methods

void ParticleEmitterComponent::LateInitialize()
{
}

void ParticleEmitterComponent::Deactivate()
{
	mp_Parent = nullptr;
	mp_texture = nullptr;

	//_DeallocateParticleArrays();
}

void ParticleEmitterComponent::Serialize(const json & j)
{
	m_active = ValueExists(j, "active") ? ParseBool(j, "active") : m_active;
	m_loopDuration = ParseFloat(j, "loopDuration");
	m_looping = ParseBool(j, "looping");
	m_prewarmed = ParseBool(j, "prewarmed");
	m_startDelay = ParseFloat(j, "startDelay");
	m_lifeTime = ParseFloat(j, "lifeTime");
	m_animationSpeed = ParseFloat(j, "animationSpeed");
	m_speedMax = ParseFloat(j, "speedMax");
	m_speedMin = ParseFloat(j, "speedMin");

	SerializeInterpolationItem(m_velocityX, j, "velocityX");
	SerializeInterpolationItem(m_velocityY, j, "velocityY");
	SerializeInterpolationItem(m_velocityZ, j, "velocityZ");
	SerializeInterpolationItem(m_scale, j, "scale");

	m_rotation = ParseFloat(j, "rotation");

	m_colorStart = ParseCompColor(j, "color", "start");
	m_colorEnd = ParseCompColor(j, "color", "end");

	m_gravityMod = ParseFloat(j, "gravity");
	m_emissionRate = ParseInt(j, "emissionRate");
	m_maxParticles = ParseInt(j, "max");
	m_rotationOverTime = ParseFloat(j, "rotationOverTime");

	std::string textureName = ParseString(j, "particleTexture");
	mp_texture = INFECT_RESOURCES.GetTexture(textureName);
	m_animRows = ParseFloat(j, "rows");
	m_animCols = ParseFloat(j, "cols");
	if (ValueExists(j, "frameHeight"))
		m_frameHeight = ParseFloat(j, "frameHeight");
	if (ValueExists(j, "frameWidth"))
		m_frameWidth = ParseFloat(j, "frameWidth");


	ID3D11Resource* res;
	mp_texture->GetResource(&res);
	ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(res);
	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);
	m_texWidth = textureDesc.Width;
	m_texHeight = textureDesc.Height;

	if (m_animRows != 0.f && m_frameHeight == 0.f)	
		m_frameHeight = (m_texHeight / m_animRows) / m_texHeight;
	else										
		m_frameHeight = 1.f;
	if (m_animCols != 0.f && m_frameWidth == 0.f)	
		m_frameWidth = (m_texWidth / m_animCols) / m_texWidth;
	else										
		m_frameWidth = 1.f;

	std::string textureSelection = ParseString(j, "textureSelection");
	if (textureSelection == "sequence")			m_textureSelection = P_TextureSelection::CYCLE;
	else if (textureSelection == "random")		m_textureSelection = P_TextureSelection::RANDOM;
	else										m_textureSelection = P_TextureSelection::SINGLE;


	//if (ValueExists(j, SHAPE_STR)) {
	//	std::string shape = ParseString(j[SHAPE_STR], "type");

	//	if (!m_pSpawnShape) {
	//		if (shape == "AABB")
	//			m_pSpawnShape = new AABB(ParseFloat(j[SHAPE_STR], "width"), ParseFloat(j[SHAPE_STR], "height"));
	//		else if (shape == "circle")
	//			m_pSpawnShape = new Circle(ParseFloat(j[SHAPE_STR], "radius"));
	//		// Particle Emitter does not support polygonal spawn shapes currently
	//	}
	//}

	m_angleVariation = ParseFloat(j, "angleVariation");

	m_emissionTime = m_loopDuration / float(m_emissionRate);

	std::string shaderProgramName = ValueExists(j, "shader") ? JsonReader::ParseStringUnsafe(j, "shader") : "particle";
	//m_shaderProgramID = TETRA_RENDERER.GetShaderIDByName(shaderProgramName);
	m_active = ValueExists(j, "isActive") ? j["isActive"] : true;


	m_particlesFollowParent = ParseBool(j, "followParentPosition");
	m_rotateToParentOnSpawn = ParseBool(j, "matchParentRotation");

	m_renderedOnTop = ParseBool(j, "renderOnTop");

	m_brightness = ValueExists(j, "brightness") ? j["brightness"] : m_brightness;

	//_AllocateParticleArrays();
	//_AllocateVBOs();
	m_liveParticleCount = 0;
}

void ParticleEmitterComponent::Override(const json & j)
{
}

void ParticleEmitterComponent::HandleEvent(Event * p_event)
{
	//switch (p_event->Type()) {
	//	case EventType::EVENT_FlipScaleX: {
	//		m_directionMod *= -1;
	//		break;
	//	}
	//	case EventType::EVENT_OnLevelInitialized: {
	//		if (m_prewarmed)
	//			_WarmSystem();
	//		break;
	//	}
	//}
}

void ParticleEmitterComponent::BindBufferDatas() const
{
	//TETRA_RENDERER.BindBufferData(m_positionsScalesBuffer, m_positionsScales, m_liveParticleCount * sizeof(GLfloat) * 4);
	//TETRA_RENDERER.BindBufferData(m_colorsBuffer, m_colors, m_liveParticleCount * sizeof(GLubyte) * 4);
	//TETRA_RENDERER.BindBufferData(m_textureCoordsBuffer, m_textureCoords, m_liveParticleCount * sizeof(GLfloat) * 2);
}

void ParticleEmitterComponent::Reset()
{
	for (int i = 0; i<m_lastUsedParticle; i++) {
		m_particles[i].life = -1.f;
	}
}

#undef SHAPE_STR

#pragma endregion
