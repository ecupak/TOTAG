#pragma once

class ParticleData;
class ParticleUpdater;
class ParticleEmitter;
class ParticleGenerator;
class ParticleRenderer;
class ParticleTrasher;


// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.
class DynamicParticleSystem
{
public:
	DynamicParticleSystem(float2& position, size_t maxParticleCount, int emitRate);
	~DynamicParticleSystem();

	void Update(float deltaTime);

	void Render(Surface* screen, int screenX, int screenY);

	bool isActive_{ true };

	float2& position_;


private:
	void Emit(float deltaTime);
	void RunUpdaters(float deltaTime);
	void Cleanup();

	ParticleData* pData_{ nullptr };
	ParticleRenderer* pRenderer_{ nullptr };
	ParticleEmitter* pEmitter_{ nullptr };
	ParticleUpdater* pUpdaters_[4];
	ParticleTrasher* pTrasher_{ nullptr };

	size_t maxParticleCount_{ 0 };
};