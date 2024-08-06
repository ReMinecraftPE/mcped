/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "FallingTile.hpp"
#include "world/level/Level.hpp"

FallingTile::FallingTile(Level* level) : Entity(level)
{
}

FallingTile::FallingTile(Level* level, float x, float y, float z, int id) : Entity(level)
{
	m_id = id;
	field_34 = 1;
	setSize(0.98f, 0.98f);
	heightOffset = bbHeight * 0.5f;
	setPos(x, y, z);
	posO.x = x;
	posO.y = y;
	posO.z = z;
	vel.x = 0.0f;
	vel.y = 0.0f;
	vel.z = 0.0f;

#if defined(ENH_ALLOW_SAND_GRAVITY)
	field_C8 = RENDER_FALLING_TILE;
#endif
}

float FallingTile::getShadowHeightOffs()
{
	return 0.0f;
}

bool FallingTile::isPickable()
{
	return !removed;
}

void FallingTile::tick()
{
	if (!m_id)
		remove();

	posO = pos;
	field_E0++;

	vel.y -= 0.04f;
	move(vel.x, vel.y, vel.z);

	vel *= 0.98f;

	int tileX = Mth::floor(pos.x);
	int tileY = Mth::floor(pos.y);
	int tileZ = Mth::floor(pos.z);

	// if we're inside one of our own tiles, clear it.
	// Assumes we started there
	if (level->getTile(tileX, tileY, tileZ) == m_id)
		level->setTile(tileX, tileY, tileZ, TILE_AIR);

	if (!onGround)
	{
		if (field_E0 > 100 && !level->field_11)
			remove();

		return;
	}

	vel.x *= 0.7f;
	vel.z *= 0.7f;
	vel.y *= -0.5f;
	remove();

	if (level->mayPlace(m_id, tileX, tileY, tileZ, true) && level->setTile(tileX, tileY, tileZ, m_id))
		return;

	// @TODO: spawn resources?
}

Level* FallingTile::getLevel()
{
	return level;
}
