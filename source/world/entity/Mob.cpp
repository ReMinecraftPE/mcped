/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Mob.hpp"
#include "world/level/Level.hpp"

Mob::Mob(Level* pLevel) : Entity(pLevel)
{
	m_texture = "/mob/char.png";
	m_class = "";

	field_34 = 1;

	field_E4 = (Mth::random() + 1.0f) * 0.01f;
	setPos(pos.x, pos.y, pos.z);
	field_E0 = Mth::random() * 12398.0f;
	yRot = float(Mth::random() * M_PI);
	footSize = 0.5f;
}

Mob::~Mob()
{
}

void Mob::lerpTo(float x, float y, float z, float yaw, float pitch, int i)
{
	field_B70 = x;
	field_B74 = y + heightOffset;
	field_B78 = z;
	field_B7C = yaw;
	field_B80 = pitch;
	field_B6C = i;
}

void Mob::tick()
{
	superTick();

	if (field_B6C > 0)
	{
		float xPos = pos.x + (field_B70 - pos.x) / field_B6C;
		float yPos = pos.y + (field_B74 - pos.y) / field_B6C;
		float zPos = pos.z + (field_B78 - pos.z) / field_B6C;

		float ang = field_B7C - yRot;
		while (ang < -180.0f) ang += 360.0f;
		while (ang >= 180.0f) ang -= 360.0f;

		yRot += ang / float(field_B6C);
		xRot += (field_B80 - xRot) / float(field_B6C);

		setPos(xPos, yPos, zPos);
		setRot(yRot, xRot);

		field_B6C--;
	}

	aiStep();
	updateWalkAnim();

	//@TODO: untangle this variable mess
	float deltaX, deltaZ, dist, x1, x2, x3, x4, x5, x6, x7, field_E8_2, field_E8_new, v36;
	bool angleOOB = false;

	deltaX = pos.x - posO.x;
	deltaZ = pos.z - posO.z;
	dist = Mth::sqrt(deltaZ * deltaZ + deltaX * deltaX);
	field_E8_2 = field_E8;
	x1 = field_E8_2;

	field_B4C = field_B50;

	if (dist > 0.05f)
	{
		x2 = dist * 3.0f;
		v36 = Mth::atan2(deltaZ, deltaX);
		x3 = 1.0f;
		x1 = ((v36 * 180.0f) / float(M_PI)) - 90.0f;
		field_E8_2 = this->field_E8;
	}
	else
	{
		x3 = 0.0f;
		x2 = 0.0f;
	}

	x4 = field_F8;
	if (x4 <= 0.0f)
		x4 = yRot;
	else
		x4 = x1 = yRot;

	if (!onGround)
		x3 = 0.0f;

	field_B50 += (x3 - field_B50) * 0.3f;
	
	x5 = x1 - field_E8_2;
	while (x5 < -180.0f)
		x5 += 360.0f;
	while (x5 >= 180.0f)
		x5 -= 360.0f;

	field_E8_new = field_E8_2 + (float)(x5 * 0.3);
	this->field_E8 = field_E8_new;

	x6 = x4 - field_E8_new;

	while (x6 < -180.0f)
		x6 += 360.0f;
	
	while (x6 >= 180.0f)
		x6 -= 360.0f;

	angleOOB = x6 < -90.0f || x6 >= 90.0f;

	if (x6 < -75.0f)
	{
		x7 = x4 + 75.0f;
		x6 = -75.0f;
		field_E8 = x4 + 75.0f;
		goto LABEL_30;
	}
	if (x6 >= 75.0f)
	{
		x7 = x4 - 75.0f;
		x6 = 75.0f;
		field_E8 = x4 - 75.0f;
	LABEL_30:
		field_E8 = x7 + (x6 * 0.2f);
		goto LABEL_31;
	}

	x7 = x4 - x6;
	field_E8 = x4 - x6;
	if (x6 * x6 > 2500.0f)
		goto LABEL_30;

LABEL_31:
	if (angleOOB)
		x2 = -x2;
	
	while (x4 - yRotO < -180.0f)
		yRotO -= 360.0f;

	while (x4 - yRotO >= 180.0f)
		yRotO += 360.0f;

	while (field_E8 - field_EC < -180.0f)
		field_EC -= 360.0f;

	while (field_E8 - field_EC >= 180.0f)
		field_EC += 360.0f;
	
	while (xRot - xRotO < -180.0f)
		xRotO -= 360.0f;

	while (xRot - xRotO >= 180.0f)
		xRotO += 360.0f;

	field_B54 += x2;
}

void Mob::baseTick()
{
	field_F4 = field_F8;
	Entity::baseTick();

	if (isAlive() && isInWall())
		hurt(nullptr, 1);

	if (isAlive() && isUnderLiquid(Material::water) && !isWaterMob())
	{
		airSupply--;
		if (airSupply == -20)
		{
			airSupply = 0;

			for (int i = 0; i < 8; i++)
			{
				level->addParticle("bubble",
					pos.x + m_random.nextFloat() - m_random.nextFloat(),
					pos.y + m_random.nextFloat() - m_random.nextFloat(),
					pos.z + m_random.nextFloat() - m_random.nextFloat(),
					vel.x, vel.y, vel.z
				);
			}

			hurt(nullptr, 2);
		}
	}
	else
	{
		airSupply = airCapacity;
	}

	field_118 = field_11C;

	if (field_114 > 0) field_114--;
	if (field_104 > 0) field_104--;
	if (field_B8  > 0) field_B8--;

	if (m_health <= 0)
	{
		field_110++;
		if (field_110 > 20)
		{
			beforeRemove();
			remove();
			for (int i = 0; i < 20; i++)
			{
				level->addParticle("explode",
					pos.x + 2 * bbWidth * m_random.nextFloat() - bbWidth,
					pos.y + bbHeight * m_random.nextFloat(),
					pos.z + 2 * bbWidth * m_random.nextFloat() - bbWidth,
					0.02f * (m_random.nextFloat() * 2 - 1) * (m_random.nextFloat() * 2 - 1),
					0.02f * (m_random.nextFloat() * 2 - 1) * (m_random.nextFloat() * 2 - 1),
					0.02f * (m_random.nextFloat() * 2 - 1) * (m_random.nextFloat() * 2 - 1)
				);
			}
		}
	}

	field_B58 = field_B54;
	field_EC = field_E8;
	yRotO = yRot;
	xRotO = xRot;
}

float Mob::getHeadHeight()
{
	return 0.85f * bbHeight;
}

bool Mob::isPickable()
{
	return !removed;
}

bool Mob::isPushable()
{
	return !removed;
}

bool Mob::isShootable()
{
	return true;
}

bool Mob::isAlive()
{
	if (removed)
		return false;

	return m_health >= 0;
}

bool Mob::hurt(Entity *pAttacker, int damage)
{
	if (level->field_11)
		return false;

	field_AFC = level->field_11;

	if (m_health <= 0)
		return false;

	field_12C = 1.5f;
	if (float(field_B8) <= float(field_DC) * 0.5f)
	{
		field_100 = m_health;
		field_B8 = field_DC;
		field_B84 = damage;
		actuallyHurt(damage);
		field_108 = 10;
		field_104 = 10;
	}
	else
	{
		if (field_B84 >= damage)
			return 0;

		actuallyHurt(damage - field_B84);
		field_B84 = damage;
	}

	field_10C = 0;
	if (m_health <= 0)
		die(pAttacker);

	return true;
}

void Mob::animateHurt()
{
	field_108 = 10;
	field_10C = 0;
	field_104 = 10;
}

void Mob::setSize(float rad, float height)
{
	Entity::setSize(rad, height);
}

void Mob::outOfWorld()
{
	hurt(nullptr, 4);
}

void Mob::causeFallDamage(float flevel)
{
	int x = int(ceilf(flevel - 3));
	if (x > 0)
	{
		hurt(nullptr, 3);

		//@HUH: useless call to getTile? or could this be a return value of some sort
		//Entity::causeFallDamage returns nothing though, so....
		level->getTile(Mth::floor(pos.x), Mth::floor(pos.y - 0.2f - heightOffset), Mth::floor(pos.z));
	}
}

void Mob::knockback(Entity* pEnt, int a, float x, float z)
{
	float power = Mth::invSqrt(x * x + z * z);

	vel.x = vel.x * 0.5f - x * power * 0.4f;
	vel.y = vel.y * 0.5f + 0.4f;
	vel.z = vel.z * 0.5f - z * power * 0.4f;

	//@BUG: maybe this is meant to behave differently.
	// Unless the player is falling, the Y vel will always be 0.4.
	if (vel.y > 0.4f)
		vel.y = 0.4f;
}

bool Mob::onLadder()
{
	int tileX = Mth::floor(pos.x);
	int tileZ = Mth::floor(pos.z);
	int tileY = Mth::floor(bb.min.y);

	//@INFO: Pre Beta 1.5 stair behaviour
	return
		level->getTile(tileX, tileY + 0, tileZ) == Tile::ladder->id || 
		level->getTile(tileX, tileY + 1, tileZ) == Tile::ladder->id;
}

void Mob::spawnAnim()
{

}

std::string Mob::getTexture()
{
	return m_texture;
}

void Mob::playAmbientSound()
{

}

int Mob::getAmbientSoundInterval()
{
	return 80;
}

void Mob::heal(int health)
{
	if (m_health <= 0)
		return;

	m_health += health;
	if (m_health > C_MAX_MOB_HEALTH)
		m_health = C_MAX_MOB_HEALTH;

	field_B8 = field_DC / 2;
}

HitResult Mob::pick(float f1, float f2)
{
	Vec3 pos = getPos(f2);
	Vec3 view = getViewVector(f2);

	Vec3 limit = pos + view * f1;
	return level->clip(pos, limit);
}

void Mob::travel(float a2, float a3)
{
	float x1, x2, dragFactor, oldYPos = pos.y;
	if (isInWater())
	{
		moveRelative(a2, a3, 0.02f);
		move(vel.x, vel.y, vel.z);
		x1 = 0.8f;
		goto label_3;
	}
	if (isInLava())
	{
		moveRelative(a2, a3, 0.02f);
		move(vel.x, vel.y, vel.z);
		x1 = 0.5f;
	label_3:

		vel.y = vel.y * x1 - 0.02f;
		vel.x *= x1;
		vel.z *= x1;

		if (horizontalCollision)
		{
			if (isFree(vel.x, vel.y + 0.6f - pos.y + oldYPos, vel.z))
				vel.y = 0.3f;
		}

		return;
	}

	if (!onGround)
	{
		x2 = 0.02f;
	}
	else
	{
		float _x1;
		int tileX = Mth::floor(pos.x);
		int tileY = Mth::floor(bb.min.y);
		int tileZ = Mth::floor(pos.z);
		TileID tile = level->getTile(tileX, tileY - 1, tileZ);
		if (tile <= 0)
			_x1 = 0.546f;
		else
			_x1 = Tile::tiles[tile]->friction * 0.91f;

		assert(_x1 != 0.0f);

		x2 = (0.16277f / (_x1 * _x1 * _x1)) * 0.1f;
	}

	moveRelative(a2, a3, x2);

	if (!onGround)
	{
		dragFactor = 0.91f;
	}
	else
	{
		//@HUH: repeated code. Could be an inlined function?
		int tileX = Mth::floor(pos.x);
		int tileY = Mth::floor(bb.min.y);
		int tileZ = Mth::floor(pos.z);
		TileID tile = level->getTile(tileX, tileY - 1, tileZ);
		if (tile <= 0)
			dragFactor = 0.546f;
		else
			dragFactor = Tile::tiles[tile]->friction * 0.91f;
	}

	if (onLadder())
	{
		fallDistance = 0.0f;

		if (vel.y < -0.15f)
			vel.y = -0.15f;

		if (isSneaking() && vel.y < 0)
			vel.y = 0;
	}

	move(vel.x, vel.y, vel.z);

	if (horizontalCollision && onLadder())
		vel.y = 0.2f;

	vel.y = (vel.y - 0.08f) * 0.98f; // gravity?

	// drag
	vel.x *= dragFactor;
	vel.z *= dragFactor;
}

void Mob::die(Entity* pCulprit)
{
	if (pCulprit && field_B48 > 0)
		pCulprit->awardKillScore(pCulprit, field_B48);

	field_B69 = true;

	if (!level->field_11)
		dropDeathLoot();
}

bool Mob::canSee(Entity* pEnt)
{
	Vec3 v1 = pos;
	v1.y += getHeadHeight();
	Vec3 v2 = pEnt->pos;
	v2.y += pEnt->getHeadHeight();

	return level->clip(v1, v2).m_hitType == HitResult::NONE;
}

void Mob::updateWalkAnim()
{
	field_128 = field_12C;

	float diffX = pos.x - posO.x;
	float diffZ = pos.z - posO.z;

	float spd = 4.0f * Mth::sqrt(diffX * diffX + diffZ * diffZ);
	if (spd > 1.0f)
		spd = 1.0f;

	field_12C += (spd - field_12C) * 0.4f;
	field_130 += field_12C;
}

void Mob::aiStep()
{
	if (isImmobile())
	{
		field_B0C = 0;
		field_B00 = 0.0f;
		field_B04 = 0.0f;
		field_B08 = 0.0f;
	}
	else if (!field_F0)
	{
		updateAi();
	}

	bool bIsInWater = isInWater(), bIsInLava = isInLava();
	if (field_B0C)
	{
		if (bIsInWater || bIsInLava)
			vel.y += 0.04f;
		else if (onGround)
			jumpFromGround();
	}

	field_B00 *= 0.98f;
	field_B04 *= 0.98f;
	field_B08 *= 0.9f;

	travel(field_B00, field_B04);

	AABB aabb = bb;
	aabb.grow(0.2f, 0.2f, 0.2f);

	auto pEnts = level->getEntities(this, aabb);
	for (auto pEnt : *pEnts)
	{
		if (pEnt->isPushable())
			pEnt->push(this);
	}
}

bool Mob::isWaterMob()
{
	return false;
}

void Mob::superTick()
{
	Entity::tick();
}

void Mob::lookAt(Entity* pEnt, float a3, float a4)
{
	float diffX = pEnt->pos.x - pos.x;
	float diffZ = pEnt->pos.z - pos.z;

	float x1 = atan2f(diffZ, diffX);
	float x2 = atan2f((pEnt->bb.min.y + pEnt->bb.max.y) / 2 - pos.y - getHeadHeight(), Mth::sqrt(diffX * diffX + diffZ * diffZ));

	xRot = -rotlerp(xRot, x2 * 180.0f / float(M_PI), a4);
	yRot = rotlerp(yRot, x1 * 180.0f / float(M_PI) - 90.0f, a3);
}

bool Mob::isLookingAtAnEntity()
{
	return m_pEntLookedAt != nullptr;
}

Entity* Mob::getLookingAt()
{
	return m_pEntLookedAt;
}

void Mob::beforeRemove()
{

}

bool Mob::canSpawn()
{
	return level->getCubes(this, bb)->empty();
}

float Mob::getAttackAnim(float f)
{
	float x = field_F8 - field_F4;
	
	if (x < 0.0f)
		x += 1.0f;

	return field_F4 + (x * f);
}

Vec3 Mob::getPos(float f)
{
	if (f == 1.0f)
		return pos;

	return Vec3(
		Lerp(posO.x, pos.x, f),
		Lerp(posO.y, pos.y, f),
		Lerp(posO.z, pos.z, f)
	);
}

Vec3 Mob::getLookAngle(float f)
{
	return getViewVector(1.0f);
}

Vec3 Mob::getViewVector(float f)
{
	constexpr float C_180_OVER_PI = 0.017453f;
	constexpr float C_PI = 3.1416f;
	
	if (f == 1.0)
	{
		float x1 = Mth::cos(-(yRot * C_180_OVER_PI) - C_PI);
		float x2 = Mth::sin(-(yRot * C_180_OVER_PI) - C_PI);
		float x3 = -Mth::cos(-(xRot * C_180_OVER_PI));

		return Vec3(x1 * x3, Mth::sin(-(xRot * C_180_OVER_PI)), x2 * x3);
	}

	float x1 = xRotO + (xRot - xRotO) * f;
	float x2 = -((yRotO + (yRot - yRotO) * f) * C_180_OVER_PI) - C_PI;
	float x3 = Mth::cos(x2);
	float x4 = Mth::sin(x2);
	float x5 = -(x1 * C_180_OVER_PI);
	float x6 = -Mth::cos(x5);

	return Vec3(x4 * x6, Mth::sin(x5), x3 * x6);
}

int Mob::getMaxSpawnClusterSize()
{
	return 4;
}

void Mob::actuallyHurt(int damage)
{

}

bool Mob::removeWhenFarAway()
{
	return false;
}

int Mob::getDeathLoot()
{
	return 0;
}

void Mob::dropDeathLoot()
{

}

bool Mob::isImmobile()
{
	return m_health <= 0;
}

void Mob::jumpFromGround()
{
	vel.y = 0.42f;
}

void Mob::updateAi()
{
	// TODO
}

int Mob::getMaxHeadXRot()
{
	return 10;
}

float Mob::getSoundVolume()
{
	return 1.0f;
}

std::string Mob::getAmbientSound()
{
	return "";
}

std::string Mob::getHurtSound()
{
	return "random.hurt";
}

std::string Mob::getDeathSound()
{
	return "random.hurt";
}

void Mob::defineSynchedData()
{

}

float Mob::rotlerp(float a2, float a3, float a4)
{
	float ang = a3 - a2;
	while (ang < -180.0f) ang += 360.0f;
	while (ang >= 180.0f) ang -= 360.0f;
	if (ang > a4)
		ang = a4;
	if (ang < -a4)
		ang = -a4;
	return a2 + ang;
}
