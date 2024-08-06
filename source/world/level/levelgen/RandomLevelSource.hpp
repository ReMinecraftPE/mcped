/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

// @NOTE: This class appears to reference a mythical "std::hashtable", but I couldn't find any traces of it.
// I doubt they used C++11 (since it came out in 2011), but this is weird...
// We'll use std::unordered_map instead.

#include <unordered_map>
#include "world/level/chunk/ChunkSource.hpp"
#include "client/common/Utils.hpp"
#include "world/level/levelgen/synth/PerlinNoise.hpp"
#include "world/level/biome/BiomeSource.hpp"
#include "world/level/levelgen/feature/Feature.hpp"
#include "world/level/levelgen/feature/LargeCaveFeature.hpp"

class RandomLevelSource : public ChunkSource
{
public:
	RandomLevelSource(Level*, TLong seed, int);
	int tick() override;
	bool shouldSave() override;
	bool hasChunk(int x, int z) override;
	LevelChunk* create(int x, int z) override;
	LevelChunk* getChunk(int x, int z) override;
	std::string gatherStats() override;
	void postProcess(ChunkSource*, int, int) override;

	float* getHeights(float*, int, int, int, int, int, int);
	void prepareHeights(int x, int z, TileID*, void*, float*);
	void buildSurfaces (int x, int z, TileID*, Biome**);
	

public:
	bool unused_field_4 = false;
	LargeCaveFeature caveFeature;
	int waterDepths[1024];
	std::unordered_map<int, LevelChunk*> chunks;
	float unused_field_19F0 = 1.0f;
	Random random;
	PerlinNoise lperlinNoise1;
	PerlinNoise lperlinNoise2;
	PerlinNoise perlinNoise1;
	PerlinNoise perlinNoise2;
	PerlinNoise perlinNoise3;
	PerlinNoise scaleNoise;
	PerlinNoise depthNoise;
	PerlinNoise forestNoise;
	Level* level = nullptr;
	float* buffer = nullptr;
	float sandBuffer[256];
	float gravelBuffer[256];
	float depthBuffer[256];
	// @TODO
	float* pnr = nullptr;
	float* ar = nullptr;
	float* br = nullptr;
	float* sr = nullptr;
	float* dr = nullptr;
	// @TODO


	static const float SNOW_CUTOFF;
	static const float SNOW_SCALE;
};

