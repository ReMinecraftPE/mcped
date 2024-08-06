/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "BiomeSource.hpp"
#include "world/level/Level.hpp"

BiomeSource::BiomeSource()
{
	biomes = new Biome*[256];
}

BiomeSource::BiomeSource(Level* pLevel) :
	random1(pLevel->getSeed() * 9871),
	random2(pLevel->getSeed() * 39811),
	random3(pLevel->getSeed() * 543321)
{
	// @NOTE: Minecraft Java would use PerlinSimplexNoise here
	temperatureMap = new PerlinNoise(&random1, 4);
	downfallMap = new PerlinNoise(&random2, 4);
	noiseMap = new PerlinNoise(&random3, 2);

	biomes = new Biome*[256];
	temperatures = new float[256];
}

Biome* BiomeSource::getBiome(ChunkPos& pos)
{
	// @BUG: Shifting right by 4 instead of left.  In Java Edition, a shift left by 4 is performed instead.
	return getBiome(pos.x >> 4, pos.z >> 4);
}

Biome* BiomeSource::getBiome(int a, int b)
{
	return *getBiomeBlock(a, b, 1, 1);
}

Biome** BiomeSource::getBiomeBlock(int a, int b, int c, int d)
{
	return getBiomeBlock(biomes, a, b, c, d);
}

Biome** BiomeSource::getBiomeBlock(Biome** pUnusedBiomes, int a, int b, int c, int d)
{
	temperatures = temperatureMap->getRegion(temperatures, a, b, c, c, 0.025f, 0.025f, 0.25f);
	downfalls = downfallMap->getRegion(downfalls, a, b, c, c, 0.05f, 0.05f, 0.3333f);
	noises = noiseMap->getRegion(noises, a, b, c, c, 0.25f, 0.25f, 0.588f);

	int index = 0;

	for (int i = 0; i < c; i++)
	{
		for (int j = 0; j < d; j++)
		{
			float d = noises[index] * 1.1f + 0.5f;
			float d1 = 0.01f;
			float d2 = 1.0f - d1;
			float d3 = (temperatures[index] * 0.15f + 0.7f) * d2 + d * d1;
			d1 = 0.002f;
			d2 = 1.0f - d1;
			float d4 = (downfalls[index] * 0.15f + 0.5f) * d2 + d * d1;
			d3 = 1.0f - (1.0f - d3) * (1.0f - d3);

			if (d3 < 0.0f) d3 = 0.0f;
			if (d4 < 0.0f) d4 = 0.0f;
			if (d3 > 1.0f) d3 = 1.0f;
			if (d4 > 1.0f) d4 = 1.0f;
			
			temperatures[index] = d3;
			downfalls[index] = d4;
			biomes[index++] = Biome::getBiome(d3, d4);
		}
	}

	return biomes;
}

float* BiomeSource::getTemperatureBlock(int a, int b, int c, int d)
{
	temperatures = temperatureMap->getRegion(temperatures, a, b, c, d, 0.025f, 0.025f, 0.25f);
	noises = noiseMap->getRegion(noises, a, b, c, d, 0.25f, 0.25f, 0.588f);

	int index = 0;

	for (int i = 0; i < c; i++)
	{
		for (int j = 0; j < d; j++)
		{
			float d = noises[index] * 1.1f + 0.5f;
			float d1 = 0.01f;
			float d2 = 1.0f - d1;
			float d3 = (temperatures[index] * 0.15f + 0.7f) * d2 + d * d1;
			d3 = 1.0f - (1.0f - d3) * (1.0f - d3);
			if (d3 < 0.0f)
				d3 = 0.0f;
			if (d3 > 1.0f)
				d3 = 1.0f;

			temperatures[index++] = d3;
		}
	}

	return temperatures;
}

BiomeSource::~BiomeSource()
{
	if (temperatureMap)
		delete temperatureMap;

	if (downfallMap)
		delete downfallMap;

	if (noiseMap)
		delete noiseMap;

	if (temperatures)
		delete[] temperatures;

	if (downfalls)
		delete[] downfalls;

	if (noises)
		delete[] noises;

	if (biomes)
		delete[] biomes;
}
