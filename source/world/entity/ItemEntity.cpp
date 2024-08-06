/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "ItemEntity.hpp"
#include "world/level/Level.hpp"

ItemEntity::ItemEntity(Level* level) : Entity(level)
#ifndef ORIGINAL_CODE
, m__itemInstance(0, 0, 0)
#endif
{
	// @NOTE: not setting render type
	field_E8 = 2 * float(M_PI) * Mth::random();
	setSize(0.25f, 0.25f);
	heightOffset = bbHeight * 0.5f;
}

ItemEntity::ItemEntity(Level* level, float x, float y, float z, ItemInstance* itemInstance) :
	Entity(level)
#ifndef ORIGINAL_CODE
	, m__itemInstance(0, 0, 0)
#endif
{
	field_C8 = RENDER_ITEM;
	field_E8 = 2 * float(M_PI) * Mth::random();
	setSize(0.25f, 0.25f);
	heightOffset = bbHeight * 0.5f;
	setPos(x, y, z);

	// @BUG: Keeping a pointer to the original ItemInstance may end up storing an invalid one
#ifdef ORIGINAL_CODE
	m_pItemInstance = itemInstance;
#else
	// copy it
	m__itemInstance = *itemInstance;
#endif

	yRot = 360.0f * Mth::random();

	vel.y = 0.2f;
	vel.x = Mth::random() * 0.2f - 0.1f;
	vel.z = Mth::random() * 0.2f - 0.1f;
}

void ItemEntity::burn(int damage)
{
	hurt(nullptr, damage);
}

bool ItemEntity::hurt(Entity* pCulprit, int damage)
{
	markHurt();
	m_health -= damage;
	if (m_health < 0)
		remove();

	return false;
}

bool ItemEntity::isInWater()
{
	return level->checkAndHandleWater(bb, Material::water, this);
}

void ItemEntity::playerTouch(Player* player)
{
	// Here, this would give the item to the player, and remove the item entity.
#ifdef TEST_DROPPED_ITEMS
	if (field_E4 != 0)
		return;

	// No inventory to add an item to. Just set the amount to 0
	m_pItemInstance->m_amount = 0;

	// No "random.pop" sound.

	if (m_pItemInstance->m_amount <= 0)
		remove();
#endif
}

void ItemEntity::tick()
{
	Entity::tick();

	if (field_E4 > 0)
		field_E4--;

	posO = pos;
	vel.y -= 0.04f;

	if (level->getMaterial(Mth::floor(pos.x), Mth::floor(pos.y), Mth::floor(pos.z)) == Material::lava)
	{
		// give it a small bounce upwards
		vel.y = 0.2f;
		vel.x = 0.2f * (sharedRandom.nextFloat() - sharedRandom.nextFloat());
		vel.z = 0.2f * (sharedRandom.nextFloat() - sharedRandom.nextFloat());
	}

	checkInTile(pos.x, pos.y, pos.z);

	move(vel.x, vel.y, vel.z);

	float dragFactor = 0.98f;

	if (onGround)
	{
		dragFactor = 0.588f;
		TileID tile = level->getTile(Mth::floor(pos.x), Mth::floor(bb.min.y) - 1, Mth::floor(pos.z));
		if (tile > 0)
			dragFactor = Tile::tiles[tile]->friction * 0.98f;
	}

	vel.x *= dragFactor;
	vel.z *= dragFactor;
	vel.y *= 0.98f;

	if (onGround)
		vel.y *= -0.5f;

	field_EC++;
	field_E0++;

	//despawn after 5 minutes
	if (field_E0 >= 6000)
		remove();
}

void ItemEntity::checkInTile(float x, float y, float z)
{
	int xfl = Mth::floor(x);
	int yfl = Mth::floor(y);
	int zfl = Mth::floor(z);

	if (!Tile::solid[level->getTile(xfl, yfl, zfl)])
		return;
	
	bool solidXN = Tile::solid[level->getTile(xfl - 1, yfl, zfl)];
	bool solidXP = Tile::solid[level->getTile(xfl + 1, yfl, zfl)];
	bool solidYN = Tile::solid[level->getTile(xfl, yfl - 1, zfl)];
	bool solidYP = Tile::solid[level->getTile(xfl, yfl + 1, zfl)];
	bool solidZN = Tile::solid[level->getTile(xfl, yfl, zfl - 1)];
	bool solidZP = Tile::solid[level->getTile(xfl, yfl, zfl + 1)];

	float mindist = 9999.0f;
	int mindir = -1;

	float xdiff = x - float(xfl);
	float ydiff = y - float(yfl);
	float zdiff = z - float(zfl);
	if (!solidXN && xdiff        < mindist) mindist = xdiff,        mindir = 0;
	if (!solidXP && 1.0f - xdiff < mindist) mindist = 1.0f - xdiff, mindir = 1;
	if (!solidYN && ydiff        < mindist) mindist = ydiff,        mindir = 2;
	if (!solidYP && 1.0f - ydiff < mindist) mindist = 1.0f - ydiff, mindir = 3;
	if (!solidZN && zdiff        < mindist) mindist = zdiff,        mindir = 4;
	if (!solidZP && 1.0f - zdiff < mindist) mindist = 1.0f - zdiff, mindir = 5;

	// the -1 case will be handled accordingly
	float force = 0.1f + 0.2f * sharedRandom.nextFloat();
	switch (mindir)
	{
		case 0: vel.x = -force; break;
		case 1: vel.x =  force; break;
		case 2: vel.y = -force; break;
		case 3: vel.y =  force; break;
		case 4: vel.z = -force; break;
		case 5: vel.z =  force; break;
	}
}
