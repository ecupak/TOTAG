#pragma once

class ParticleData;
class ParticleUpdater;
class ParticleEmitter;
class ParticleGenerator;
class ParticleRenderer;
class ParticleTrasher;

struct ParticleRewindData;

enum class Event;


// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.
class ParticleSystem : public Observer
{
public:
	ParticleSystem::ParticleSystem();
	ParticleSystem(float2 position, size_t maxParticleCount, int emitRate);
	~ParticleSystem();

	void Initialize(float2 position, size_t maxParticleCount, int emitRate);

	void Update(float deltaTime);
	
	void Render(Surface* screen, int screenX, int screenY);

	void OnNotify(const ObjectClass objectClass, const Event event) override;
	
	// Manage rewind data.
	void StoreRewindData(ParticleRewindData& rewindData);
	void Rewind(ParticleRewindData& rewindData);
	void Resume(ParticleRewindData& rewindData);


	float2 position_{ 0.0f, 0.0f };

	bool isActive_{ true };


protected:
	void Emit(float deltaTime);
	void RunUpdaters(float deltaTime);
	void Cleanup();

	ParticleData* pData_{ nullptr };
	ParticleRenderer* pRenderer_{ nullptr };
	ParticleEmitter* pEmitter_{ nullptr };
	ParticleUpdater* pUpdaters_[3];
	ParticleTrasher* pTrasher_{ nullptr };

	size_t maxParticleCount_{ 0 };

	uint color_{ 0x0000FF };
};