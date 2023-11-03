#pragma once


// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.
class ParticleData
{
public:
	ParticleData(size_t maxCount);
	~ParticleData();


	// Disallow copying or assignment.
	ParticleData(const ParticleData&) = delete;
	ParticleData& operator=(const ParticleData&) = delete;


	// Methods.
	void Kill(size_t id);
	void Wake(size_t id);


	// Properties.
	float4* positions_{ nullptr };
	float2* origins_{ nullptr };
	float2* radians_{ nullptr };
	uint* colors_{ nullptr };
	float* lifetimes_{ nullptr };
	bool* isAlives_{ nullptr };


	// Particle tracking (size_t always positive, holds largest value possible on platform).
	size_t count_{ 0 };
	size_t countAlive_{ 0 };


private:
	void Generate(size_t maxSize);
	void SwapData(size_t replace, size_t replacement);
};

