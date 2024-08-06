/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "Biome.hpp"

class Level;
class BiomeSource
{
public:
	BiomeSource(Level*);
	BiomeSource();
	virtual ~BiomeSource();

	virtual Biome* getBiome(ChunkPos&);
	virtual Biome* getBiome(int, int);
	virtual Biome** getBiomeBlock(int, int, int, int);
	virtual Biome** getBiomeBlock(Biome**, int, int, int, int);
	virtual float* getTemperatureBlock(int, int, int, int);

public:
	float* temperatures = nullptr;
	float* downfalls = nullptr;
	float* noises = nullptr;
	int unused_field_10 = 0;
	int unused_field_14 = 0;
	int unused_field_18 = 0;
	int unused_field_1C = 0;
	Biome** biomes = nullptr;
	PerlinNoise* temperatureMap = nullptr;
	PerlinNoise* downfallMap = nullptr;
	PerlinNoise* noiseMap = nullptr;
	Random random1;
	Random random2;
	Random random3;
};

