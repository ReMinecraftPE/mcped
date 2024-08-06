/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Feature.hpp"
#include "world/level/Level.hpp"

SpringFeature::SpringFeature(int id)
{
	this->id = id;
}

bool SpringFeature::place(Level* level, Random* random, int x, int y, int z)
{
    if (level->getTile(x, y + 1, z) != Tile::rock->id)
        return false;

    if (level->getTile(x, y - 1, z) != Tile::rock->id)
        return false;

    if (level->getTile(x, y, z) && level->getTile(x, y, z) != Tile::rock->id)
        return false;

    int nRockTiles = 0;
    int nEmptyTiles = 0;

    if (level->getTile(x - 1, y, z) == Tile::rock->id) nRockTiles++;
    if (level->getTile(x + 1, y, z) == Tile::rock->id) nRockTiles++;
    if (level->getTile(x, y, z - 1) == Tile::rock->id) nRockTiles++;
    if (level->getTile(x, y, z + 1) == Tile::rock->id) nRockTiles++;

    if (level->isEmptyTile(x - 1, y, z)) nEmptyTiles++;
    if (level->isEmptyTile(x + 1, y, z)) nEmptyTiles++;
    if (level->isEmptyTile(x, y, z - 1)) nEmptyTiles++;
    if (level->isEmptyTile(x, y, z + 1)) nEmptyTiles++;

    if (nEmptyTiles != 1) return true;
    if (nRockTiles  != 3) return true;

    level->setTile(x, y, z, id);

    level->field_10 = true;
    Tile::tiles[id]->tick(level, x, y, z, random);
    level->field_10 = false;
    return true;
}
