/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Entity.hpp"
#include "Player.hpp"
#include "world/level/Level.hpp"

int Entity::entityCounter;
Random Entity::sharedRandom;

Entity::Entity(Level* pLevel)
{
	level = pLevel;
	entityId = ++entityCounter;
	setPos(0, 0, 0);
}

Entity::~Entity()
{
}

void Entity::setLevel(Level* pLvl)
{
	level = pLvl;
}

void Entity::setPos(float x, float y, float z)
{
	pos = Vec3(x, y, z);

	float halfSize = bbWidth / 2;
	float lowY = y - heightOffset + ySlideOffset;

	bb = AABB(
		x - halfSize,
		lowY,
		z - halfSize,
		x + halfSize,
		lowY + bbHeight,
		z + halfSize);
}

void Entity::remove()
{
	removed = true;
}

int Entity::move(float x, float y, float z)
{
	float x_1 = x, z_1 = z;

	if (noPhysics)
	{
		// just move it. Don't perform any kind of collision
		bb.min.x += x;
		bb.max.x += x;
		bb.min.y += y;
		bb.max.y += y;
		bb.min.z += z;
		bb.max.z += z;

		pos.x = (bb.max.x + bb.min.x) / 2;
		pos.z = (bb.max.z + bb.min.z) / 2;
		pos.y = bb.min.y + heightOffset - ySlideOffset;

		return 1300;
	}

	//@TODO: untangle the control flow

	float x1, x2, x3, x4, x5, x6;
	float x7, x8, x9, x10, x11, x12, x20;
	float x_2, z_2, x_3, z_3;
	float oldX, oldZ;
	bool b1, b2, b3, b4, b5, b6;
	AABB hitold;

	oldX = pos.x, oldZ = pos.z;

	x7 = bb.max.z;
	x8 = bb.max.y;
	x9 = bb.max.x;
	x10 = bb.min.z;
	x11 = bb.min.y;
	x12 = bb.min.x;

	x1 = bb.max.z;
	x2 = bb.max.y;
	x3 = bb.max.x;
	x4 = bb.min.z;
	x5 = bb.min.y;
	x6 = bb.min.x;

	b5 = onGround && isSneaking();
	if (b5)
	{
		if (x_1 == 0.0f)
		{
			x_2 = x_1;
			b1 = x_1 < 0.0f;
			b2 = x_1 > 0.0f;
		}
		else
		{
			x_2 = x_1;
			do
			{
				AABB aabb = bb;
				aabb.move(x_1, -1.0f, 0);

				auto cubes = level->getCubes(this, aabb);

				if (cubes->size())
					break;

				if (x_1 < 0.05f && x_1 >= -0.05f)
				{
					x_2 = 0.0f;
					x_1 = 0.0f;
					break;
				}
			
				// @BUG: See the z_1 part
				if (x_1 <= 0.0f)
					x_1 = x_1 + 0.05f;
				else
					x_1 = x_1 - 0.05f;

				x_2 = x_1;
			}
			while (x_1 != 0.0f);

			b1 = x_1 < 0.0f;
			b2 = x_1 > 0.0f;
		}

		if (z_1 == 0.0f)
		{
			z_2 = z_1;
			b3 = z_1 < 0.0f;
			b4 = z_1 > 0.0f;
		}
		else
		{
			z_2 = z_1;
			do
			{
				AABB aabb = bb;
				aabb.move(0, -1.0f, z_1);

				auto cubes = level->getCubes(this, aabb);

				if (cubes->size())
					break;

				if (z_1 < 0.05f && z_1 >= -0.05f)
				{
					z_2 = 0.0f;
					z_1 = 0.0f;
					break;
				}

				//@BUG: wouldn't this loop forever? Since if z_1 == 0.025f, it'd oscillate between -0.025f and +0.025f...
				if (z_1 <= 0.0f)
					z_1 = z_1 + 0.05f;
				else
					z_1 = z_1 - 0.05f;
			}
			while (z_1 != 0.0f);
			b3 = z_1 < 0.0f;
			b4 = z_1 > 0.0f;
		}
	}
	else
	{
		z_2 = z_1;
		b1 = x_1 < 0.0f;
		b2 = x_1 > 0.0f;
		x_2 = x_1;
		b3 = z_1 < 0.0f;
		b4 = z_1 > 0.0f;
	}

	if (b1) x6 += x_1;
	if (b2) x3 += x_1;
	if (y < 0.0f) x5 += y;
	if (y > 0.0f) x2 += y;
	if (b3) x4 += z;
	if (b4) x1 += z;

	AABB scanAABB(x6, x5, x4, x3, x2, x1);
	auto pCubes = level->getCubes(this, scanAABB);

	float newY = y;
	for (const auto &aabb : (*pCubes))
		newY = aabb.clipYCollide(bb, newY);

	bb.move(0, newY, 0);

	if (!slide && newY != y)
	{
		z_1 = 0.0f;
		x_1 = 0.0f;
		newY = 0.0f;
	}

	if (onGround)
	{
		b6 = true;
	}
	else
	{
		b6 = y < 0.0f;
		if (newY == y)
			b6 = 0;
	}

	for (const auto& aabb : (*pCubes))
		x_1 = aabb.clipXCollide(bb, x_1);

	bb.move(x_1, 0, 0);

	if (!slide && x_1 != x_2)
	{
		z_1 = 0.0f;
		x_1 = 0.0f;
		newY = 0.0f;
	}

	for (const auto& aabb : (*pCubes))
		z_1 = aabb.clipZCollide(bb, z_1);

	bb.move(0, 0, z_1);

	if (!slide && z_1 != z_2)
	{
		z_1 = 0.0f;
		x_1 = 0.0f;
		newY = 0.0f;
	}

	x20 = footSize;
	if (x20 <= 0.0f || !b6)
	{
	label_44:
		z_3 = z_1;
		x_3 = x_1;
		goto label_45;
	}

	if (ySlideOffset >= 0.05f || x_2 == x_1 && z_2 == z_1)
		goto label_44;

	// oh come on, undoing all our work??
	hitold = bb;
	bb = AABB(x12, x11, x10, x9, x8, x7);

	if (b1) x12 += x_2;
	if (b2) x9  += x_2;
	x8 += x20; //@BUG: missing if (x20 > 0) check?
	if (x20 < 0.0f) x11 += x20;
	if (b3) x10 += z_2;
	if (b4) x7  += z_2;

	{
		AABB scanAABB2(x12, x11, x10, x9, x8, x7);

		//@BUG: useless copy
		//@BUG: this doesn't actually copy anything
		*pCubes = *level->getCubes(this, scanAABB2);
	}

	for (const auto& aabb : (*pCubes))
		x20 = aabb.clipYCollide(bb, x20);

	bb.move(0, x20, 0);

	if (slide || x20 == y)
	{
		z_3 = z_2;
		x_3 = x_2;
	}
	else
	{
		x20 = 0.0f;
		z_3 = 0.0f;
		x_3 = 0.0f;
	}

	for (const auto& aabb : (*pCubes))
		x_3 = aabb.clipXCollide(bb, x_3);

	bb.move(x_3, 0, 0);

	if (!slide && x_2 != x_3)
	{
		x20 = 0.0f;
		z_3 = 0.0f;
		x_3 = 0.0f;
	}

	for (const auto& aabb : (*pCubes))
		z_3 = aabb.clipZCollide(bb, z_3);

	bb.move(0, 0, z_3);

	if (!slide && z_2 != z_3)
	{
		x20 = 0.0f;
		z_3 = 0.0f;
		x_3 = 0.0f;
	}

	// if we moved more this time than before?? no clue wtf this is about...
	if (z_1 * z_1 + x_1 * x_1 < z_3 * z_3 + x_3 * x_3)
	{
		ySlideOffset += 0.5f;
		newY = x20;
	}
	else
	{
		// don't get the rationale behind this at all...
		bb = hitold;
		z_3 = z_1;
		x_3 = x_1;
	}

label_45:
	pos.x = (bb.min.x + bb.max.x) / 2;
	pos.z = (bb.min.z + bb.max.z) / 2;
	pos.y = bb.min.y - ySlideOffset + heightOffset;

	horizontalCollision = x_2 != x_3 || z_2 != z_3;
	verticalCollision = newY != y;
	collision = horizontalCollision || verticalCollision;
	onGround = y < 0.0f && newY != y;

	checkFallDamage(newY, onGround);

	if (x_2 != x_3) vel.x = 0.0;
	if (newY != y)  vel.y = 0.0;
	if (z_2 != z_3) vel.z = 0.0;

	if (!b5)
	{
		float diffX = (pos.x - oldX), diffZ = (pos.z - oldZ);
		walkDist += Mth::sqrt(diffZ * diffZ + diffX * diffX) * 0.6f;

		int tileX = Mth::floor(pos.x);
		int tileY = Mth::floor(pos.y - 0.2f - heightOffset);
		int tileZ = Mth::floor(pos.z);

		TileID tileID = level->getTile(tileX, tileY, tileZ);

		if (tileID > 0 && walkDist > float(field_D8))
		{
			++field_D8;
			bool bPlaySound = true;

			auto sound = Tile::tiles[tileID]->soundType;
			if (level->getTile(tileX, tileY + 1, tileZ) == Tile::topSnow->id)
				sound = Tile::topSnow->soundType;
			else if (Tile::tiles[tileID]->material->isLiquid())
				bPlaySound = false;

			if (bPlaySound)
				level->playSound(this, sound->getStepSound(), sound->volume * 0.15f, sound->pitch);

			Tile::tiles[tileID]->stepOn(level, tileX, tileY, tileZ, this);
		}
	}

	// Check whether the entity is inside of any tiles.

	int minX = Mth::floor(bb.min.x), minY = Mth::floor(bb.min.y), minZ = Mth::floor(bb.min.z),
		maxX = Mth::floor(bb.max.x), maxY = Mth::floor(bb.max.y), maxZ = Mth::floor(bb.max.z);

	if (level->hasChunksAt(minX, minY, minZ, maxX, maxY, maxZ))
	{
		for (int x = minX; x <= maxX; x++)
			for (int y = minY; y <= maxY; y++)
				for (int z = minZ; z <= maxZ; z++)
				{
					TileID tileID = level->getTile(x, y, z);
					if (tileID > 0)
						Tile::tiles[tileID]->entityInside(level, x, y, z, this);
				}
	}

	ySlideOffset *= 0.4f;

	bool bIsInWater = isInWater();

	if (level->containsFireTile(bb))
	{
		burn(true);

		if (bIsInWater)
		{
		label_76:
			if (onFire > 0)
				onFire = -flameTime;

			return 1300;
		}

		onFire++;
		if (onFire == 0)
			onFire = 300;
	}
	else
	{
		if (onFire <= 0)
			onFire = -flameTime;

		if (bIsInWater)
			goto label_76;
	}

	return 1300;
}

void Entity::moveTo(float x, float y, float z, float yaw, float pitch)
{
	y += heightOffset;

	pos.x = x;
	yRot = yaw;
	posO.x = x;
	posOld.x = x;

	pos.z = z;
	xRot = pitch;
	posO.z = z;
	posOld.z = z;

	pos.y = y;
	posO.y = y;
	posOld.y = y;

	setPos(x, y, z);
}

void Entity::absMoveTo(float x, float y, float z, float yaw, float pitch)
{
	ySlideOffset = 0.0f;

	yRotO = yaw;
	yRot  = yaw;
	xRot  = pitch;
	xRotO = pitch;

	pos.x = x;
	posO.x = x;
	pos.y = y;
	posO.y = y;
	pos.z = z;
	posO.z = z;

	//@WHAT: (yaw - yaw) == 0
	if (yaw - yaw < -180.0f)
		yRotO = yaw + 360.0f;
	if (yaw - yaw >= 180.0f)
		yRotO = yRotO - 360.0f;

	setPos(this->pos.x, this->pos.y, this->pos.z);
	return setRot(yaw, pitch);
}

void Entity::moveRelative(float x, float z, float y)
{
	float d = sqrtf(x * x + z * z);
	if (d < 0.01f) return;
	if (d < 1.0f)
		d = 1.0f;

	y /= d;
	x *= y;
	z *= y;

	float yaw = yRot * float(M_PI);
	float syaw = sinf(yaw / 180.0f);
	float cyaw = cosf(yaw / 180.0f);

	vel.x += x * cyaw - z * syaw;
	vel.z += x * syaw + z * cyaw;
}

void Entity::lerpTo(float x, float y, float z, float yaw, float pitch, int p)
{
	setPos(x, y, z);
	setRot(yaw, pitch);
}

void Entity::lerpMotion(float x, float y, float z)
{
	vel = Vec3(x, y, z);
}

void Entity::turn(float yaw, float pitch)
{
	float y_old = yRot;
	float p_old = xRot;

	yRot   += yaw   * 0.15f;
	xRot -= pitch * 0.15f;

	// can't rotate more than facing fully up or fully down
	if (xRot < -90.0f)
		xRot = -90.0f;
	if (xRot > 90.0f)
		xRot = 90.0f;

	yRotO += yRot   - y_old;
	xRotO += xRot - p_old;
}

void Entity::interpolateTurn(float yaw, float pitch)
{
	yRot   += yaw   * 0.15f;
	xRot -= pitch * 0.15f;

	// can't rotate more than facing fully up or fully down
	if (xRot < -90.0f)
		xRot = -90.0f;
	if (xRot > 90.0f)
		xRot = 90.0f;
}

void Entity::tick()
{
	baseTick();
}

void Entity::baseTick()
{
	//@TODO: untangle the gotos

	walkDistO = walkDist;
	posO = pos;
	tickCount++;
	yRotO = yRot;
	xRotO = xRot;
	if (isInWater())
	{
		if (!field_D4 && !field_D6)
		{
			float dist = sqrtf(vel.y * vel.y + vel.x * vel.x * 0.2f + vel.z * vel.z * 0.2f) * 0.2f;
			if (dist > 1.0f)
				dist = 1.0f;

			level->playSound(this, "random.splash", dist, 1.0f + 0.4f * (sharedRandom.nextFloat() - sharedRandom.nextFloat()));

			float f1 = floorf(bb.min.y);

			for (int i = 0; float(i) < bbWidth * 20.0f + 1.0f; i++)
			{
				level->addParticle(
					"bubble",
					pos.x + bbWidth * (sharedRandom.nextFloat() * 2.0f - 1.0f),
					f1 + 1.0f,
					pos.z + bbWidth * (sharedRandom.nextFloat() * 2.0f - 1.0f),
					vel.x,
					vel.y - 0.2f * sharedRandom.nextFloat(),
					vel.z
				);
			}

			for (int i = 0; float(i) < bbWidth * 20.0f + 1.0f; i++)
			{
				level->addParticle(
					"splash",
					pos.x + bbWidth * (sharedRandom.nextFloat() * 2.0f - 1.0f),
					f1 + 1.0f,
					pos.z + bbWidth * (sharedRandom.nextFloat() * 2.0f - 1.0f),
					vel.x,
					vel.y - 0.2f * sharedRandom.nextFloat(),
					vel.z
				);
			}
		}

		field_D4 = true;
		onFire = 0;
		fallDistance = 0;

		if (level->field_11)
			goto label_4;
	}
	else
	{
		field_D4 = false;

		if (level->field_11)
		{
		label_4:
			onFire = 0;
			goto label_5;
		}
	}

	if (onFire <= 0)
	{
	label_5:
		if (!isInLava())
			goto label_6;
		goto label_15;
	}

	if (field_D5)
	{
		onFire -= 4;
		if (onFire < 0)
			onFire = 0;
		goto label_5;
	}

	if (onFire % 20 == 0)
	{
		hurt(nullptr, true);
	}

	onFire--;

	if (isInLava())
	{
	label_15:
		lavaHurt();
	}

label_6:
	if (pos.y < -64.0f)
		outOfWorld();

	field_D6 = false;
}

bool Entity::intersects(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	AABB aabb(minX, minY, minZ, maxX, maxY, maxZ);
	return aabb.intersect(bb);
}

bool Entity::isFree(float offX, float offY, float offZ)
{
	AABB aabb = bb;
	aabb.move(offX, offY, offZ);

	auto pCubes = level->getCubes(this, aabb);
	if (!pCubes)
		return false;

	return !level->containsAnyLiquid(aabb);
}

bool Entity::isFree(float offX, float offY, float offZ, float expand)
{
	AABB aabb = bb;
	aabb.move(offX, offY, offZ);
	aabb.grow(expand, expand, expand);

	auto pCubes = level->getCubes(this, aabb);
	if (!pCubes)
		return false;

	return !level->containsAnyLiquid(aabb);
}

bool Entity::isInWall()
{
	return level->isSolidTile(Mth::floor(pos.x), Mth::floor(pos.y + getHeadHeight()), Mth::floor(pos.z));
}

bool Entity::isInWater()
{
	AABB aabb = bb;
	aabb.grow(0, -0.4f, 0);
	return level->checkAndHandleWater(aabb, Material::water, this);
}

bool Entity::isInLava()
{
	AABB aabb = bb;
	aabb.grow(-0.1f, -0.4f, -0.1f);
	return level->containsMaterial(aabb, Material::lava);
}

bool Entity::isUnderLiquid(Material* pMtl)
{
	int tileX = Mth::floor(pos.x);
	int tileY = Mth::floor(pos.y + getHeadHeight());
	int tileZ = Mth::floor(pos.z);

	Tile* pTile = Tile::tiles[level->getTile(tileX, tileY, tileZ)];
	if (!pTile || pTile->material != pMtl)
		return false;

	int data = level->getData(tileX, tileY, tileZ);
	int level = data <= 7 ? data + 1 : 1;

	return float(tileY) < float(tileY + 1) - (float(level) / 9.0f - 0.11111f);
}

float Entity::getHeadHeight()
{
	return 0.0f;
}

float Entity::getShadowHeightOffs()
{
	return bbHeight / 2.0f;
}

float Entity::getBrightness(float f)
{
	int tileX = Mth::floor(pos.x);
	int tileY = Mth::floor(pos.y - heightOffset + (bb.max.y - bb.min.y) * 0.66f);
	int tileZ = Mth::floor(pos.z);

	int minX = Mth::floor(bb.min.x);
	int minY = Mth::floor(bb.min.y);
	int minZ = Mth::floor(bb.min.z);
	int maxX = Mth::floor(bb.max.x);
	int maxY = Mth::floor(bb.max.y);
	int maxZ = Mth::floor(bb.max.z);

	if (!level->hasChunksAt(minX, minY, minZ, maxX, maxY, maxZ))
		return 0;

	return level->getBrightness(tileX, tileY, tileZ);
}

float Entity::distanceTo(Entity* pEnt)
{
	return distanceTo(pEnt->pos.x, pEnt->pos.y, pEnt->pos.z);
}

float Entity::distanceTo(float x, float y, float z)
{
	return sqrtf(distanceToSqr(x, y, z));
}

float Entity::distanceToSqr(Entity* pEnt)
{
	return distanceToSqr(pEnt->pos.x, pEnt->pos.y, pEnt->pos.z);
}

float Entity::distanceToSqr(float x, float y, float z)
{
	float diffX = pos.x - x;
	float diffY = pos.y - y;
	float diffZ = pos.z - z;

	return diffX * diffX + diffY * diffY + diffZ * diffZ;
}

int Entity::interactPreventDefault()
{
	return 0;
}

bool Entity::interact(Player* player)
{
	return false;
}

void Entity::playerTouch(Player* player)
{

}

void Entity::push(Entity* bud)
{
	float diffX = bud->pos.x - pos.x;
	float diffZ = bud->pos.z - pos.z;
	float maxDiff = Mth::absMax(diffX, diffZ);

	if (maxDiff < 0.01f) return;
	
	float x1 = sqrtf(maxDiff);
	float x2 = 1.0f / x1;
	if (x2 > 1.0f)
		x2 = 1.0f;
	float x3 = 1.0f - this->pushthrough;
	float x4 = x3 * diffX / x1 * x2 * 0.05f;

	push(-x4, 0.0f, -x4);
	bud->push(x4, 0.0f, x4);
}

void Entity::push(float x, float y, float z)
{
	vel.x += x;
	vel.y += y;
	vel.z += z;
}

bool Entity::isPickable()
{
	return false;
}

bool Entity::isPushable()
{
	return false;
}

bool Entity::isShootable()
{
	return false;
}

bool Entity::isSneaking()
{
	return false;
}

bool Entity::isAlive()
{
	return removed;
}

bool Entity::isOnFire()
{
	return onFire > 0;
}

bool Entity::isPlayer()
{
	return false;
}

bool Entity::isCreativeModeAllowed()
{
	return false;
}

bool Entity::shouldRender(Vec3& camPos)
{
	return shouldRenderAtSqrDistance(distanceToSqr(camPos.x, camPos.y, camPos.z));
}

bool Entity::shouldRenderAtSqrDistance(float distSqr)
{
	float maxDist = (viewScale * 64.0f) * (((bb.max.z - bb.min.z) + (bb.max.x - bb.min.x) + (bb.max.y - bb.min.y)) / 3.0f);

	return maxDist * maxDist > distSqr;
}

bool Entity::hurt(Entity* pAttacker, int damage)
{
	markHurt();
	return 0;
}

void Entity::animateHurt()
{

}

float Entity::getPickRadius()
{
	return 0.1f;
}


void Entity::awardKillScore(Entity* pKilled, int score)
{
	
}

void Entity::setEquippedSlot(int a, int b, int c)
{

}

void Entity::setRot(float yaw, float pitch)
{
	yRot   = yaw;
	xRot = pitch;
}

void Entity::setSize(float rad, float height)
{
	bbWidth = rad;
	bbHeight = height;
}

void Entity::setPos(EntityPos* pPos)
{
	if (pPos->m_bHasPos)
		setPos(pPos->pos.x, pPos->pos.y, pPos->pos.z);
	else
		setPos(pos.x, pos.y, pos.z);

	if (pPos->m_bHasRot)
		setRot(pPos->m_yaw, pPos->m_pitch);
	else
		setRot(yRot, xRot);
}

void Entity::resetPos()
{
	if (!level)
		// No level?  Fine
		return;

	if (pos.y <= 0.0f)
		// It's in the void.  It's probably not inside of any tiles.
		return;

	// Push the entity up while it's inside solid tiles.
	while (true)
	{
		setPos(pos.x, pos.y, pos.z);

		auto pCubes = level->getCubes(this, bb);

		// if we aren't inside any tiles, great!
		if (!pCubes->size())
			break;

		pos.y += 1.0f;
	}

	vel.x = 0.0f;
	vel.y = 0.0f;
	vel.z = 0.0f;
	xRot = 0.0f;
}

void Entity::outOfWorld()
{
	remove();
}

void Entity::checkFallDamage(float fDeltaY, bool bHitGround)
{
	if (bHitGround)
	{
		if (fallDistance > 0.0f)
		{
			causeFallDamage(fallDistance);
			fallDistance = 0.0f;
		}
	}
	else if (fDeltaY < 0.0f)
	{
		fallDistance -= fDeltaY;
	}
}

void Entity::causeFallDamage(float f)
{
	// stub
}

void Entity::markHurt()
{
	hurtMarked = true;
}

void Entity::burn(int x)
{
	if (!field_D5)
		hurt(nullptr, 4);
}

void Entity::lavaHurt()
{
	if (!field_D5)
	{
		hurt(nullptr, 4);
		onFire = 600;
	}
}

int Entity::hashCode()
{
	return entityId;
}

bool Entity::operator==(const Entity& other) const
{
	return entityId == other.entityId;
}
