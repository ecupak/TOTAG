#pragma once

class ParticleData;
class ParticleGenerator;


// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.
class ParticleEmitter
{
public:
	ParticleEmitter(float2& position, int emitRate);
	~ParticleEmitter();

	void Emit(float deltaTime, ParticleData* pData);

	void ChangeColor(const uint color);

private:
	float2& position_;
	ParticleGenerator* generators_[6];
	int emitRate_{ 0 };
	float emissionBuildup_{ 0.0f };
	uint color_{ 0x0000FF };
};

