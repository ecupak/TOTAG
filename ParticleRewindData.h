#pragma once

struct ParticleRewindData
{
	ParticleRewindData();
	~ParticleRewindData();


	int countAlive_{ 0 };

	uint color_{ 0 };

	float4* positions_{ nullptr };

	float2* radians_{ nullptr };

	float* lifetimes_{ nullptr };
	
	bool* isAlives_{ nullptr };
};