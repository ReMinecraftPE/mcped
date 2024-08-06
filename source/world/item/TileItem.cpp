/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "TileItem.hpp"
#include "world/level/tile/Tile.hpp"
#include "world/level/Level.hpp"

TileItem::TileItem(int id) : Item(id)
{
	id += 256;
	m_tile = id;
	m_icon = Tile::tiles[id]->getTexture(DIR_ZNEG);
}

std::string TileItem::getDescriptionId()
{
	return Tile::tiles[m_tile]->getDescriptionId();
}

std::string TileItem::getDescriptionId(ItemInstance* instance)
{
	return Tile::tiles[m_tile]->getDescriptionId();
}

bool TileItem::useOn(ItemInstance* instance, Player* player, Level* level, int x, int y, int z, int dir)
{
	if (level->getTile(x, y, z) == Tile::topSnow->id)
	{
		dir = DIR_YNEG;
	}
	else switch (dir)
	{
		case DIR_YNEG: y--; break;
		case DIR_YPOS: y++; break;
		case DIR_ZNEG: z--; break;
		case DIR_ZPOS: z++; break;
		case DIR_XNEG: x--; break;
		case DIR_XPOS: x++; break;
	}

	if (!instance->m_amount)
		return false;

	if (!level->mayPlace(m_tile, x, y, z, false))
		return false;

	Tile* pTile = Tile::tiles[m_tile];

	if (!level->setTileAndData(x, y, z, m_tile, getLevelDataForAuxValue(instance->getAuxValue())))
		return true;

	Tile::tiles[m_tile]->setPlacedOnFace(level, x, y, z, dir);
	Tile::tiles[m_tile]->setPlacedBy(level, x, y, z, player);

	level->playSound(
		float(x) + 0.5f,
		float(y) + 0.5f,
		float(z) + 0.5f,
		pTile->soundType->getStepSound(),
		(pTile->soundType->volume + 1.0f) * 0.5f,
		pTile->soundType->pitch * 0.8f
	);

	instance->m_amount--;
	return true;
}
