/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

enum class P_TextureSelection {
	CYCLE, RANDOM, SINGLE
};

class ParticleEmitterComponentManager;
class ParticleEmitterComponent : public RenderableComponent {
protected:
	friend ParticleEmitterComponentManager;

	struct Particle {
		FLOAT x, y, z;						// Position
		FLOAT rotation;						// Amount of spin the billboard has
		FLOAT speed;						// speed of the particle
		unsigned short animRow, animCol;	// animation row + col
		FLOAT vX, vY, vZ;					// velocity
		CompressedColor color;			// Color of the particle
		FLOAT scale;						// Uniform scaling of the particle
		FLOAT life;						// How much longer this particle has to live
		FLOAT animationTime;				// Current time in the animation loop for this particle
		FLOAT angleOffset;				//
		FLOAT cameraDistance;				// Distance from the currently rendering camera
		FLOAT u, v;				// Texture UV coordinates

		bool operator<(Particle& that) {
			// Sort in reverse order : far particles drawn first.
			return this->cameraDistance > that.cameraDistance;
		}
	};

	struct ParticleVertex {
		FLOAT x, y, z;
		FLOAT scale;
		FLOAT u, v;
		FLOAT r, g, b, a;

		void FillVert(
			const ParticleEmitterComponent::Particle& p, 
			float _u, float _v,
			float _xDir, float _yDir);
	};


	// Emitter Configuration Properties
	float m_loopDuration;						// Time it takes for the Emitter to complete a cycle in seconds
	bool m_looping;								// Whether or not the Emitter should loop the emission process
	bool m_prewarmed;							// Starts the Emitter as though it had completed a cycle of emission
	float m_startDelay;							// How long the emitter will wait to emit in seconds
	float m_lifeTime;							// How long a particle will live in seconds
	float m_animationSpeed;						// How long until a particle cycles to the next frame
	float m_speedMax;							// Maximum speed of a particle in the starting direction
	float m_speedMin;							// Minimum speed of a particle in the starting direction
	InterpolationItem m_velocityX;				// Velocity x offsets of a particle at the start of it's lifetime
	InterpolationItem m_velocityY;				// Velocity y offsets of a particle at the start of it's lifetime
	InterpolationItem m_velocityZ;				// Velocity z offsets of a particle at the start of it's lifetime
	InterpolationItem m_scale;					// X & Y scale of a particle at start and end, points to interpolate between
	float m_rotation;							// Z-Axis rotation of a particle at start
	CompressedColor m_colorStart;				// Color of a particle at start 
	CompressedColor m_colorEnd;					// Color of a particle at end
	float m_gravityMod;							// Scales gravity applied to particles
	int m_emissionRate;							// How many particles to emit over the duration of a single loop
	unsigned int m_maxParticles;				// Max number of particles allowed at once - emission will halt temporarily if this number is met
	float m_rotationOverTime;					// Speed at which a particle will rotate during it's lifetime
	bool m_randomEmission;						// Whether or not the Emitter should "randomly" emit new particles
	P_TextureSelection m_textureSelection;		// Whether or not particles should cycle through the available particle frames or choose one somehow
	//Shape* m_pSpawnShape;						// Area the particle can appear in, if no shape is attached, particles will spawn at transform location
	float m_angleVariation;						// How far in degrees the particle can deviate from the forward direction of the transform
	bool m_particlesFollowParent;				// Determines whether or not to have particles' positions follow the emitter over time or to exist on their own
	bool m_rotateToParentOnSpawn;				// Determines whether to rotate the particles to match the emitter's orientation at spawn or not
	float m_brightness;							// Tints the particles for added or dulled brightness

	// Emitter Run Properties
	bool m_active;								// Determines whether or not this emitter will run
	//Transform* m_pTransform;					// Parent GO is required to have a Transform for an emitter
	float m_currentTime;						// Current time (t) within this loop
	float m_emissionTime;						// How long to wait to spawn the next particle
	float m_emissionTimer;						// How long since the last particle was spawned

	// RenderParticle Configuration Properties
	ID3D11ShaderResourceView * mp_texture;		// Texture for all particles in this emitter
	unsigned int m_texWidth, m_texHeight;		// Dimensions of the texture
	float m_animRows, m_animCols;				// Number of rows & columns of sprite frames in the texture
	float m_frameHeight, m_frameWidth;			// Size of each frame in the sprite texture
	bool m_renderedOnTop;						// Will force particles associated with the parent GO to be rendered first 
	float m_directionMod;						// If the scale of the parent object gets flipped, this will appropriately flip particle velocities

	// RenderParticle Manager Properties
	std::vector<Particle> m_particles;			// Array of all existing Particles managed by this Emitter
	Scene& m_scene;
	//GLuint m_positionsScalesBuffer;				// OpenGL Buffer where positions and sizes are to be streamed
	//GLuint m_colorsBuffer;						// OpenGL Buffer where colors are to be streamed
	//GLuint m_textureCoordsBuffer;				// OpenGL Buffer where texture coords are to be streamed

	int m_vertexCount, m_indexCount;
	ParticleVertex* m_vertices;
	ID3D11Buffer * mp_vertexBuffer;
	ID3D11Buffer * mp_indexBuffer;


	std::vector<FLOAT> m_positionsScales;		// Array of positions and sizes (xyz pos, w is uniform scale)
	std::vector<unsigned char> m_colors;		// Array of Colors split up into it's components
	std::vector<FLOAT> m_textureCoords;			// Array of texture coords
	UINT m_liveParticleCount;					// Indicates how many particles are currently alive, set each Update loop
	UINT m_lastUsedParticle;					// An index into m_particles, represents the index of the last used particle
	

	void _AllocateParticleArrays();
	void _DeallocateParticleArrays();
	bool _CreateBuffers();
	void _ReleaseBuffers();
public:
	static const unsigned CACHESIZE = 300;
	static const ComponentType Type = ComponentType::C_ParticleEmitter;
	virtual ComponentType GetType() const { return Type; }

	ParticleEmitterComponent(InfectGUID guid);
	~ParticleEmitterComponent();

	static Component* CreateInstance(InfectGUID guid) { return new ParticleEmitterComponent(guid); }

	virtual void Deactivate();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);
	virtual void LateInitialize();
	virtual void HandleEvent(Event* pEvent);

	inline const Scene& GetScene() const { return m_scene; };
	inline void SetScene(Scene& scene) { m_scene = scene; }

	//inline GLuint GetTextureBuffer() const { return m_texture->bufferId; }
	//inline int GetAlphaMode() const { return m_texture->alphaMode; }
	void BindBufferDatas() const;
	//inline GLuint GetPositions() const { return m_positionsScalesBuffer; }
	//inline GLuint GetColors() const { return m_colorsBuffer; }
	//inline GLuint GetTextureCoords() const { return m_textureCoordsBuffer; }

	inline int LiveParticles() const { return m_liveParticleCount; }

	inline void ActivateParticles() { m_active = true; }
	inline void DeactivateParticles() { m_active = false; }
	void Reset();

	inline float FrameWidth() const { return m_frameWidth; }
	inline float FrameHeight() const { return m_frameHeight; }

	inline float Brightness() const { return m_brightness; }
	inline bool ShouldRenderLast() const { return m_renderedOnTop; }








	// TODO: GET RID OF THIS
	void Render() const;
};

#endif

