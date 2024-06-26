#pragma once

#include "LargeFeature.hpp"
#include "client/math/Random.hpp"
#include "world/level/Level.hpp"

class LargeFeature
{
public:
	LargeFeature();
	virtual ~LargeFeature();
	virtual void apply(ChunkSource*, Level*, int, int, TileID*, int);
	virtual void addFeature(Level*, int, int, int, int, TileID*, int) = 0;


public:
	int m_radius = 8;
	Random m_random;
};

