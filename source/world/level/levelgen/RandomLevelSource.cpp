/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "RandomLevelSource.hpp"
#include "world/level/Level.hpp"
#include "world/level/tile/SandTile.hpp"

const float RandomLevelSource::SNOW_CUTOFF = 0.5f;
const float RandomLevelSource::SNOW_SCALE  = 0.3f;

float g_timeSpentInPostProcessing = 0;

RandomLevelSource::RandomLevelSource(Level* level, TLong seed, int x) :
	random(seed),
	lperlinNoise1(&random, 16),
	lperlinNoise2(&random, 16),
	perlinNoise1(&random, 8),
	perlinNoise2(&random, 4),
	perlinNoise3(&random, 4),
	scaleNoise(&random, 10),
	depthNoise(&random, 16),
	forestNoise(&random, 8),
	level(level)
{
	LogMsg("Generating world with seed: %d", seed);

	for (int a = 0; a < 32; a++)
		for (int b = 0; b < 32; b++)
			waterDepths[a * 32 + b] = 0;

	buffer = new float[1024];

	Random random = this->random;
	printf("random.get : %d\n", random.genrand_int32() >> 1);
}

// @BUG: Potential collisions.
inline int GetChunkHash(int x, int z)
{
	int v6 = z & 0x7FFF | ((x & 0x7FFF) << 16) | x & 0x80000000, v7;
	if (z >= 0)
		v7 = 0;
	else
		v7 = 0x8000;

	return v6 | v7;
}

LevelChunk* RandomLevelSource::getChunk(int x, int z)
{
	int hashCode = GetChunkHash(x, z);
	auto iter = chunks.find(hashCode);
	if (iter != chunks.end())
		return iter->second;

	// have to generate the chunk
	random.init_genrand(341872712 * x + 132899541 * z);

	TileID* pLevelData = new TileID[32768];

	LevelChunk* pChunk = new LevelChunk(level, pLevelData, x, z);
	chunks.insert({ hashCode, pChunk });

	// @NOTE: Java Edition Beta 1.2 would store the biome block in a field
	Biome** pBiomeBlock = level->getBiomeSource()->getBiomeBlock(16 * x, 16 * z, 16, 16);
	prepareHeights(x, z, pLevelData, nullptr, level->getBiomeSource()->temperatures);
	buildSurfaces(x, z, pLevelData, pBiomeBlock);
	pChunk->recalcHeightmap();

	// @NOTE: Java Edition Beta 1.6 uses the caveFeature.

	return pChunk;
}

float* RandomLevelSource::getHeights(float* fptr, int a3, int a4, int a5, int a6, int a7, int a8)
{
	if (fptr == nullptr)
	{
		fptr = new float[a6 * a7 * a8];
	}

	float* bsf4 = level->getBiomeSource()->temperatures;
	float* bsf8 = level->getBiomeSource()->downfalls;

	constexpr float C_MAGIC_1 = 684.412f;

	sr = scaleNoise.getRegion(sr, a3, a5, a6, a8, 1.121f, 1.121f, 0.5f);
	dr = depthNoise.getRegion(dr, a3, a5, a6, a8, 200.0f, 200.0f, 0.5f);
	pnr = perlinNoise1.getRegion(pnr, float(a3), float(a4), float(a5), a6, a7, a8, 8.5552f,   4.2776f,   8.5552f);
	ar = lperlinNoise1.getRegion(ar, float(a3), float(a4), float(a5), a6, a7, a8, C_MAGIC_1, C_MAGIC_1, C_MAGIC_1);
	br = lperlinNoise2.getRegion(br, float(a3), float(a4), float(a5), a6, a7, a8, C_MAGIC_1, C_MAGIC_1, C_MAGIC_1);

	int k1 = 0;
	int l1 = 0;
	int i2 = 16 / a6;
	for (int j2 = 0; j2 < a6; j2++)
	{
		int k2 = j2 * i2 + i2 / 2;
		for (int l2 = 0; l2 < a8; l2++)
		{
			int i3 = l2 * i2 + i2 / 2;
			float d2 = bsf4[k2 * 16 + i3];
			float d3 = bsf8[k2 * 16 + i3] * d2;
			float d4 = 1.0f - d3;
			d4 *= d4;
			d4 *= d4;
			d4 = 1.0f - d4;
			float d5 = (sr[l1] + 256.0f) / 512.0f;
			d5 *= d4;
			if (d5 > 1.0f)
				d5 = 1.0f;
			float d6 = dr[l1] / 8000.0f;
			if (d6 < 0.0f)
			{
				d6 = -d6 * 0.3f;
			}
			d6 = d6 * 3.0f - 2.0f;
			if (d6 < 0.0f)
			{
				d6 /= 2.0f;
				if (d6 < -1.0f)
					d6 = -1.0f;
				d6 /= 1.4f;
				d6 /= 2.0f;
				d5 = 0.0f;
			}
			else
			{
				if (d6 > 1.0f)
					d6 = 1.0f;
				d6 /= 8.0f;
			}
			if (d5 < 0.0f)
				d5 = 0.0f;
			d5 += 0.5f;
			d6 = (d6 * (float)a7) / 16.0f;
			float d7 = (float)a7 / 2.0f + d6 * 4.0f;
			l1++;
			for (int j3 = 0; j3 < a7; j3++)
			{
				float d8 = 0.0f;
				float d9 = (((float)j3 - d7) * 12.0f) / d5;
				if (d9 < 0.0f)
				{
					d9 *= 4.0f;
				}
				float d10 = ar[k1] / 512.0f;
				float d11 = br[k1] / 512.0f;
				float d12 = (pnr[k1] / 10.0f + 1.0f) / 2.0f;
				if (d12 < 0.0f)
					d8 = d10;
				else if (d12 > 1.0f)
					d8 = d11;
				else
					d8 = d10 + (d11 - d10) * d12;
				d8 -= d9;
				if (j3 > a7 - 4)
				{
					float d13 = (float)(j3 - (a7 - 4)) / 3.0f;
					d8 = d8 * (1.0f - d13) + -10.0f * d13;
				}
				fptr[k1] = d8;
				k1++;
			}
		}
	}

	return fptr;
}

void RandomLevelSource::prepareHeights(int x, int z, TileID* tiles, void* huh, float* fptr)
{
	buffer = getHeights(buffer, x * 4, 0, z * 4, 5, 17, 5);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 16; k++)
			{
				float v24 = buffer[85 * i + 17 * j + k];
				float v23 = buffer[85 * i + 17 + 17 * j + k];
				float v22 = buffer[85 * i + 85 + 17 * j + k];
				float v21 = buffer[85 * i + 102 + 17 * j + k];
				float v20 = (buffer[85 * i + 1 + 17 * j + k] - v24) * 0.125f;
				float v19 = (buffer[85 * i + 18 + 17 * j + k] - v23) * 0.125f;
				float v18 = (buffer[85 * i + 86 + 17 * j + k] - v22) * 0.125f;
				float v17 = (buffer[85 * i + 103 + 17 * j + k] - v21) * 0.125f;
				for (int l = 0; l < 8; l++)
				{
					float v15 = v24;
					float v14 = v23;
					for (int m = 0; m < 4; m++)
					{
						int v12 = ((m + 4 * i) << 11) | (j << 9) | (l + 8 * k);
						float v11 = v15;
						for (int n = 0; n < 4; n++)
						{
							TileID tile = TILE_AIR;
							if (8 * k + l < 64)
							{
								if (fptr[64 * i + 16 * m + 4 * j + n] < 0.5f && 8 * k + l == 63)
									tile = Tile::ice->id;
								else
									tile = Tile::calmWater->id;
							}
							if (v11 > 0.0f)
								tile = Tile::rock->id;
							
							tiles[v12] = tile;
							v12 += 128;
							v11 = v11 + (v14 - v15) * 0.25f;
						}
						v15 = v15 + (v22 - v24) * 0.25f;
						v14 = v14 + (v21 - v23) * 0.25f;
					}
					v24 = v24 + v20;
					v23 = v23 + v19;
					v22 = v22 + v18;
					v21 = v21 + v17;
				}
			}
		}
	}
}

void RandomLevelSource::buildSurfaces(int x, int z, TileID* tiles, Biome** biomes)
{
	perlinNoise2.getRegion(sandBuffer, float(x) * 16.0f, float(z) * 16.0f, 0.0f,    16, 16, 1, 1.0f / 32.0f, 1.0f / 32.0f, 1.0f);
	perlinNoise2.getRegion(gravelBuffer, float(x) * 16.0f, 109.01f, float(z) * 16.0f, 16, 1, 16, 1.0f / 32.0f, 1.0f,         1.0f / 32.0f);
	perlinNoise3.getRegion(depthBuffer, float(x) * 16.0f, float(z) * 16.0f, 0.0f,    16, 16, 1, 1.0f / 16.0f, 1.0f / 16.0f, 1.0f / 16.0f);

	// @NOTE: Again, extracted from Java Beta 1.6. Probably accurate
	constexpr int byte0 = 64;
	constexpr float d = 0.03125f;
	for (int k = 0; k < 16; k++)
	{
		for (int l = 0; l < 16; l++)
		{
			Biome* pBiome = biomes[k + l * 16];
			bool flag = sandBuffer[k + l * 16] + random.nextFloat() * 0.2f > 0.0f;
			bool flag1 = gravelBuffer[k + l * 16] + random.nextFloat() * 0.2f > 3.0f;
			int i1 = (int)(depthBuffer[k + l * 16] / 3.0f + 3.0f + random.nextFloat() * 0.25f);
			int j1 = -1;

			TileID topMaterial = pBiome->topMaterial;
			TileID material = pBiome->material;

			for (int k1 = 127; k1 >= 0; k1--)
			{
				int l1 = (l * 16 + k) * 128 + k1;
				if (k1 <= int(0 + random.nextInt(5)))
				{
					tiles[l1] = Tile::unbreakable->id;
					continue;
				}

				TileID byte3 = tiles[l1];
				if (byte3 == 0)
				{
					j1 = -1;
					continue;
				}

				if (byte3 != Tile::rock->id)
					continue;

				if (j1 == -1)
				{
					if (i1 <= 0)
					{
						topMaterial = 0;
						material = Tile::rock->id;
					}
					else if (k1 >= byte0 - 4 && k1 <= byte0 + 1)
					{
						topMaterial = pBiome->topMaterial;
						material = pBiome->material;
						if (flag1) {
							topMaterial = 0;
							material = Tile::gravel->id;
						}
						if (flag) {
							topMaterial = Tile::sand->id;
							material = Tile::sand->id;
						}
					}

					if (k1 < byte0 && topMaterial == 0)
						topMaterial = Tile::calmWater->id;

					j1 = i1;

					if (k1 >= byte0 - 1)
						tiles[l1] = topMaterial;
					else
						tiles[l1] = material;

					continue;
				}

				if (j1 <= 0)
					continue;

				j1--;
				tiles[l1] = material;

				if (j1 == 0 && material == Tile::sand->id)
				{
					j1 = random.nextInt(4);
					material = Tile::sandStone->id;
				}
			}
		}
	}
}

void RandomLevelSource::postProcess(ChunkSource* src, int x, int z)
{
	//return;
	level->field_12 = true;
	SandTile::instaFall = true;
	
	int x16 = x * 16, z16 = z * 16;

	Biome* pBiome = level->getBiomeSource()->getBiome(x16 + 16, z16 + 16);
	TLong seed = level->getSeed();
	int xo, yo, zo;

	random.setSeed(seed);
	TLong x1 = 1 + 2 * (random.nextInt() / 2);
	TLong x2 = 1 + 2 * (random.nextInt() / 2);
	random.setSeed((TLong(x) * x1 + TLong(z) * x2) ^ seed);

	// @NOTE: I can't put the random calls _in_ the argument list - args are evaluated right to left I believe

	for (int i = 0; i < 10; i++)
	{
		xo = random.nextInt(16);
		yo = random.nextInt(128);
		zo = random.nextInt(16);
		ClayFeature(Tile::clay->id, 32).place(level, &random, x16 + xo, yo, z16 + zo);
	}
	
	// Start of ore generation

	for (int i = 0; i < 20; i++)
	{
		xo = random.nextInt(16);
		yo = random.nextInt(128);
		zo = random.nextInt(16);
		OreFeature(Tile::dirt->id, 32).place(level, &random, x16 + xo, yo, z16 + zo);
	}
	for (int i = 0; i < 10; i++)
	{
		xo = random.nextInt(16);
		yo = random.nextInt(128);
		zo = random.nextInt(16);
		OreFeature(Tile::gravel->id, 32).place(level, &random, x16 + xo, yo, z16 + zo);
	}
	for (int i = 0; i < 20; i++)
	{
		xo = random.nextInt(16);
		yo = random.nextInt(128);
		zo = random.nextInt(16);
		OreFeature(Tile::coalOre->id, 16).place(level, &random, x16 + xo, yo, z16 + zo);
	}
	for (int i = 0; i < 20; i++)
	{
		xo = random.nextInt(16);
		yo = random.nextInt(64);
		zo = random.nextInt(16);
		OreFeature(Tile::ironOre->id, 8).place(level, &random, x16 + xo, yo, z16 + zo);
	}
	for (int i = 0; i < 2; i++)
	{
		xo = random.nextInt(16);
		yo = random.nextInt(32);
		zo = random.nextInt(16);
		OreFeature(Tile::goldOre->id, 8).place(level, &random, x16 + xo, yo, z16 + zo);
	}
	for (int i = 0; i < 8; i++)
	{
		xo = random.nextInt(16);
		yo = random.nextInt(16);
		zo = random.nextInt(16);
		OreFeature(Tile::redStoneOre->id, 7).place(level, &random, x16 + xo, yo, z16 + zo);
	}

	xo = random.nextInt(16);
	yo = random.nextInt(16);
	zo = random.nextInt(16);
	OreFeature(Tile::emeraldOre->id, 7).place(level, &random, x16 + xo, yo, z16 + zo);

	xo = random.nextInt(16);
	yo = random.nextInt(16) + random.nextInt(16);
	zo = random.nextInt(16);
	OreFeature(Tile::lapisOre->id,   6).place(level, &random, x16 + xo, yo, z16 + zo);

	// End of ore generation
	// Start of tree generation

	float t1 = forestNoise.getValue(float(x16) / 2.0f, float(z16) / 2.0f);
	float t2 = random.nextFloat();
	int t3 = int((4.0f + t2 * 4.0f + t1 * 0.125f) / 3.0f);

	int treeCount = 0;
	if (random.nextInt(10) == 0)
		treeCount++;

	if (pBiome == Biome::forest)
		treeCount += 2 + t3;

	if (pBiome == Biome::rainForest)
		treeCount += t3 + 2;

	if (pBiome == Biome::seasonalForest)
		treeCount += t3 + 1;

	if (pBiome == Biome::taiga)
		treeCount += t3 + 1;

	if (pBiome == Biome::desert)
		treeCount -= 20;

	if (pBiome == Biome::tundra)
		treeCount -= 20;

	if (pBiome == Biome::plains)
		treeCount -= 20;

	for (int i = 0; i < treeCount; i++)
	{
		int rngX = random.nextInt(16) + x16 + 8;
		int rngZ = random.nextInt(16) + z16 + 8;
		int height = level->getHeightmap(rngX, rngZ);

		Feature* pTreeFeature = pBiome->getTreeFeature(&random);
		if (pTreeFeature)
		{
			pTreeFeature->init(1.0f, 1.0f, 1.0f);
			pTreeFeature->place(level, &random, rngX, height, rngZ);
			delete pTreeFeature;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		int xo = random.nextInt(16);
		int yo = random.nextInt(128);
		int zo = random.nextInt(16);
		FlowerFeature(Tile::flower->id).place(level, &random, x16 + 8 + xo, yo, z16 + 8 + zo);
	}

	if (random.nextInt(2) == 0)
	{
		int xo = random.nextInt(16);
		int yo = random.nextInt(128);
		int zo = random.nextInt(16);
		FlowerFeature(Tile::rose->id).place(level, &random, x16 + 8 + xo, yo, z16 + 8 + zo);
	}
	if (random.nextInt(4) == 0)
	{
		int xo = random.nextInt(16);
		int yo = random.nextInt(128);
		int zo = random.nextInt(16);
		FlowerFeature(Tile::mushroom1->id).place(level, &random, x16 + 8 + xo, yo, z16 + 8 + zo);
	}
	if (random.nextInt(8) == 0)
	{
		int xo = random.nextInt(16);
		int yo = random.nextInt(128);
		int zo = random.nextInt(16);
		FlowerFeature(Tile::mushroom2->id).place(level, &random, x16 + 8 + xo, yo, z16 + 8 + zo);
	}

	for (int i = 0; i < 10; i++)
	{
		int xo = random.nextInt(16);
		int yo = random.nextInt(128);
		int zo = random.nextInt(16);
		ReedsFeature().place(level, &random, x16 + 8 + xo, yo, z16 + 8 + zo);
	}

	for (int i = 0; i < 50; i++)
	{
		int xo = random.nextInt(16);
		int yo = random.nextInt(random.nextInt(120) + 8);
		int zo = random.nextInt(16);
		SpringFeature(Tile::water->id).place(level, &random, x16 + 8 + xo, yo, z16 + 8 + zo);
	}

	for (int i = 0; i < 20; i++)
	{
		int xo = random.nextInt(16);
		int yo = random.nextInt(random.nextInt(random.nextInt(112) + 8) + 8);
		int zo = random.nextInt(16);
		SpringFeature(Tile::lava->id).place(level, &random, x16 + 8 + xo, yo, z16 + 8 + zo);
	}

	float* tempBlock = level->getBiomeSource()->getTemperatureBlock(x16 + 8, z16 + 8, 16, 16);
	for (int j19 = x16 + 8; j19 < x16 + 8 + 16; j19++)
	{
		for (int j22 = z16 + 8; j22 < z16 + 8 + 16; j22++)
		{
			int i24 = j19 - (x16 + 8);
			int j25 = j22 - (z16 + 8);

			int tsb = level->getTopSolidBlock(j19, j22);
			
			if (SNOW_CUTOFF > (tempBlock[i24 * 16 + j25] - SNOW_SCALE * (float(tsb - 64) / 64.0f)))
			{
				if (tsb >= 0 && tsb < C_MAX_Y && level->isEmptyTile(j19, tsb, j22))
				{
					if (level->getMaterial(j19, tsb - 1, j22)->blocksMotion() &&
						level->getMaterial(j19, tsb - 1, j22) != Material::ice)
					{
						level->setTile(j19, tsb, j22, Tile::topSnow->id);
					}
				}
			}
		}
	}

	SandTile::instaFall = false;
	level->field_12 = false;
}

int RandomLevelSource::tick()
{
	return 0;
}

bool RandomLevelSource::shouldSave()
{
	return true;
}

bool RandomLevelSource::hasChunk(int x, int z)
{
	return true;
}

LevelChunk* RandomLevelSource::create(int x, int z)
{
	return getChunk(x, z);
}

std::string RandomLevelSource::gatherStats()
{
	return "RandomLevelSource";
}
