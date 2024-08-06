/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "PerlinNoise.hpp"
#include "client/common/Utils.hpp"

PerlinNoise::PerlinNoise(int levels)
{
	_seedRandom = &_defaultRandom;
	init(levels);
}

PerlinNoise::PerlinNoise(Random* random, int levels)
{
	_seedRandom = random;
	init(levels);
}

void PerlinNoise::init(int levels)
{
	this->levels = levels;
	noiseLevels = new ImprovedNoise* [levels];

	for (int i = 0; i < levels; i++)
	{
		noiseLevels[i] = new ImprovedNoise(_seedRandom);
	}
}

PerlinNoise::~PerlinNoise()
{
	for (int i = 0; i < levels; i++)
		delete[] noiseLevels[i];

	delete[] noiseLevels;
}

float PerlinNoise::getValue(float x, float y)
{
	if (levels <= 0) return 0.0f;

	float result = 0.0f, x1 = 1.0f;

	for (int i = 0; i < levels; i++)
	{
		result += noiseLevels[i]->getValue(x * x1, y * x1) / x1;
		x1 /= 2.f;
	}

	return result;
}

float PerlinNoise::getValue(float x, float y, float z)
{
	if (levels <= 0) return 0.0f;

	float result = 0.0f, x1 = 1.0f;

	for (int i = 0; i < levels; i++)
	{
		result += noiseLevels[i]->getValue(x * x1, y * x1, z * x1) / x1;
		x1 /= 2.f;
	}

	return result;
}

float* PerlinNoise::getRegion(float* a2, int a3, int a4, int a5, int a6, float a7, float a8, float a9)
{
	return getRegion(a2, float(a3), 10.0f, float(a4),  a5, 1, a6,  a7, 1.0f, a8);
}

float* PerlinNoise::getRegion(float* pMem, float a3, float a4, float a5, int a6, int a7, int a8, float a9, float a10, float a11)
{
	int amt = a6 * a7 * a8;
	if (!pMem)
		pMem = new float[amt];

	for (int i = 0; i < amt; i++)
		pMem[i] = 0;

	float x = 1.0f;
	for (int i = 0; i < levels; i++)
	{
		noiseLevels[i]->add(pMem, a3, a4, a5, a6, a7, a8, a9 * x, a10 * x, a11 * x, x);
		x /= 2;
	}

	return pMem;
}
