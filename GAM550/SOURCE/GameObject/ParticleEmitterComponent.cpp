/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

#define SHAPE_STR "SHAPE"

void ParticleEmitterComponent::ParticleVertex::FillVert(const ParticleEmitterComponent::Particle & p, float _u, float _v, float _xDir, float _yDir)
{
	x = p.x + (p.scale * _xDir);
	y = p.y + (p.scale * _yDir);
	z = p.z;

	scale = p.scale;
	
	u = _u;//p.u;
	v = _v;// p.v;
	
	r = FLOAT(p.color.r) / 255.0f;
	g = FLOAT(p.color.g) / 255.0f;
	b = FLOAT(p.color.b) / 255.0f;
	a = FLOAT(p.color.a) / 255.0f;
}

void ParticleEmitterComponent::_AllocateParticleArrays()
{
	_DeallocateParticleArrays();
	m_particles.resize(m_maxParticles);
	std::fill(m_particles.begin(), m_particles.end(), ParticleEmitterComponent::Particle());
	m_positionsScales.resize(m_maxParticles * 4);
	m_colors.resize(m_maxParticles * 4);
	m_textureCoords.resize(m_maxParticles * 2);

	for (int i = 0; i < m_maxParticles; ++i) {
		m_particles[i].life = -1.f;
		m_particles[i].angleOffset = 0.f;

		m_particles[i].u = m_frameWidth / m_texWidth;
		m_particles[i].v = m_frameHeight / m_texHeight;

		int i4 = i * 4;
		m_positionsScales[i4] = 0.f;
		m_positionsScales[i4 + 1] = 0.f;
		m_positionsScales[i4 + 2] = 0.f;
		m_positionsScales[i4 + 3] = 0.f;

		m_colors[i4] = 0;
		m_colors[i4 + 1] = 0;
		m_colors[i4 + 2] = 0;
		m_colors[i4 + 3] = 0;

		m_textureCoords[i * 2 + 0] = 0.f;
		m_textureCoords[i * 2 + 1] = 0.f;
	}
}

void ParticleEmitterComponent::_DeallocateParticleArrays()
{
	m_particles.clear();
	m_positionsScales.clear();
	m_colors.clear();
	m_textureCoords.clear();
}

bool ParticleEmitterComponent::_CreateBuffers()
{
	unsigned long* indices;
	int i;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the maximum number of vertices in the vertex array.
	m_vertexCount = m_maxParticles * 6;

	// Set the maximum number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array for the particles that will be rendered.
	m_vertices = new ParticleVertex[m_vertexCount];
	if (!m_vertices)	return false;

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)	return false;

	// Initialize vertex array to zeros at first.
	memset(m_vertices, 0, (sizeof(ParticleVertex) * m_vertexCount));

	// Initialize the index array.
	for (i = 0; i<m_indexCount; i++)
	{
		indices[i] = i;
	}

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(ParticleVertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	if (FAILED(INFECT_RENDERER.Device()->CreateBuffer(&vertexBufferDesc, &vertexData, &mp_vertexBuffer)))
		return false;

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	if (FAILED(INFECT_RENDERER.Device()->CreateBuffer(&indexBufferDesc, &indexData, &mp_indexBuffer)))
		return false;

	// Release the index array since it is no longer needed.
	delete[] indices;
	indices = 0;

	return true;
}

void ParticleEmitterComponent::_ReleaseBuffers()
{
	// Release the index buffer.
	if (mp_indexBuffer)
	{
		mp_indexBuffer->Release();
		mp_indexBuffer = nullptr;
	}

	// Release the vertex buffer.
	if (mp_vertexBuffer)
	{
		mp_vertexBuffer->Release();
		mp_vertexBuffer = nullptr;
	}
}


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
}

ParticleEmitterComponent::~ParticleEmitterComponent() 
{
	_DeallocateParticleArrays();
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

	_DeallocateParticleArrays();
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

	//std::string shaderProgramName = ValueExists(j, "shader") ? JsonReader::ParseStringUnsafe(j, "shader") : "particle";
	//m_shaderProgramID = TETRA_RENDERER.GetShaderIDByName(shaderProgramName);
	m_active = ValueExists(j, "isActive") ? j["isActive"] : true;


	m_particlesFollowParent = ParseBool(j, "followParentPosition");
	m_rotateToParentOnSpawn = ParseBool(j, "matchParentRotation");

	m_renderedOnTop = ParseBool(j, "renderOnTop");

	m_brightness = ValueExists(j, "brightness") ? j["brightness"] : m_brightness;

	_AllocateParticleArrays();
	_CreateBuffers();
	m_liveParticleCount = 0;
}

void ParticleEmitterComponent::Override(const json & j) {}

void ParticleEmitterComponent::HandleEvent(Event * p_event) {}

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
	m_IsDirty = true;
}

void ParticleEmitterComponent::Render() const
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(ParticleEmitterComponent::ParticleVertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	INFECT_RENDERER.DeviceContext()->IASetVertexBuffers(0, 1, &mp_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	INFECT_RENDERER.DeviceContext()->IASetIndexBuffer(mp_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer.
	INFECT_RENDERER.DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

#undef SHAPE_STR

#pragma endregion
