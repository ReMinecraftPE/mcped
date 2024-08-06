/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Player.hpp"
#include "world/level/Level.hpp"

Player::Player(Level* pLevel) : Mob(pLevel)
{
	field_C8 = 2;

	m_pInventory = new Inventory(this);

	heightOffset = 1.62f;

	Pos pos = level->getSharedSpawnPos();

	moveTo(float(pos.x) + 0.5f, float(pos.y) + 1.0f, float(pos.z) + 0.5f, 0.0f, 0.0f);

	m_health = 20;

	m_class = "humanoid";
	m_texture = "mob/char.png";

	flameTime = 20;
	field_B5C = 180.0f;
}

Player::~Player()
{
	delete m_pInventory;
}

void Player::remove()
{
	Entity::remove();
}

void Player::tick()
{
	Mob::tick();
}

bool Player::isInWall()
{
	return Entity::isInWall();
}

float Player::getHeadHeight()
{
	return 0.12f; //@HUH: what?
}

bool Player::isShootable()
{
	return true;
}

bool Player::isPlayer()
{
	return true;
}

bool Player::isCreativeModeAllowed()
{
	return true;
}

bool Player::hurt(Entity* pEnt, int damage)
{
	//@HUH
	return false;
}

void Player::awardKillScore(Entity* pKilled, int score)
{
	m_score += score;
}

void Player::resetPos()
{
	heightOffset = 1.62f;
	setSize(0.6f, 1.8f);

	Entity::resetPos();

	m_health = 20;
	field_110 = 0;
}

void Player::die(Entity* pCulprit)
{
	Mob::die(pCulprit);
	setSize(0.2f, 0.2f);
	setPos(pos.x, pos.y, pos.z); // update hitbox
	vel.y = 0.1f;

	if (pCulprit)
	{
		vel.x = -0.1f * Mth::cos(float((field_10C + yRot) * M_PI / 180.0));
		vel.z = -0.1f * Mth::cos(float((field_10C + yRot) * M_PI / 180.0));
	}
	else
	{
		vel.x = 0;
		vel.z = 0;
	}
}

void Player::aiStep()
{
	field_B9C = field_BA0;
	float velLen = Mth::sqrt(vel.x * vel.x + vel.z * vel.z);
	float velYAtan = Mth::atan(vel.y * -0.2f), x1 = 0.0f;

	if (velLen > 0.1f)
		velLen = 0.1f;

	if (!onGround)
	{
		if (m_health > 0)
		{
			velLen = 0;
			x1 = velYAtan * 15.0f;
		}
		else
		{
			velLen = x1 = 0.0f;
		}
	}

	if (m_health <= 0)
		velLen = 0.0f;

	field_BA0 += (velLen - field_BA0) * 0.4f;
	field_11C += (x1 - field_11C) * 0.8f;

	if (m_health <= 0)
		return;

	AABB scanAABB = bb;
	scanAABB.grow(1, 1, 1);

	auto pEnts = level->getEntities(this, scanAABB);

	for (auto pEnt : *pEnts)
	{
		if (pEnt->removed)
			continue;

		touch(pEnt);
	}
}

bool Player::isImmobile()
{
	return m_health <= 0;
}

void Player::updateAi()
{
	if (field_BA4)
	{
		field_BA8++;
		if (field_BA8 > 7)
		{
			field_BA8 = 0;
			field_BA4 = false;
		}
	}
	else
	{
		field_BA8 = 0;
	}

	field_F8 = field_BA8 * 0.125f;
}

void Player::defineSynchedData()
{
	Mob::defineSynchedData();
}

void Player::animateRespawn()
{
	
}

int Player::addResource(int x)
{
	return 0;
}

void Player::animateRespawn(Player*, Level*)
{

}

void Player::attack(Entity* pEnt)
{
	
}

bool Player::canDestroy(Tile* pTile)
{
	return true;
}

void Player::closeContainer()
{

}

void Player::displayClientMessage(const std::string& msg)
{

}

void Player::drop(ItemInstance* pItemInstance)
{
	drop(pItemInstance, false);
}

void Player::drop(ItemInstance* pItemInstance, bool b)
{
	if (!pItemInstance)
		return;

	ItemEntity* pItemEntity = new ItemEntity(level, pos.x, pos.y - 0.3f + getHeadHeight(), pos.z, pItemInstance);
	pItemEntity->field_E4 = 40;

	if (b)
	{
		float throwPower = 0.5f * m_random.nextFloat();
		float throwAngle = m_random.nextFloat();

		pItemEntity->vel.x = -(throwPower * Mth::sin(2 * float(M_PI) * throwAngle));
		pItemEntity->vel.z =  (throwPower * Mth::cos(2 * float(M_PI) * throwAngle));
		pItemEntity->vel.y = 0.2f;
	}
	else
	{
		pItemEntity->vel.x = -(Mth::sin(yRot / 180.0f * float(M_PI)) * Mth::cos(xRot / 180.0f * float(M_PI))) * 0.3f;
		pItemEntity->vel.z =  (Mth::cos(yRot / 180.0f * float(M_PI)) * Mth::cos(xRot / 180.0f * float(M_PI))) * 0.3f;
		pItemEntity->vel.y = 0.1f - Mth::sin(xRot / 180.0f * float(M_PI)) * 0.3f;

		float f1 = m_random.nextFloat();
		float f2 = m_random.nextFloat();

		pItemEntity->vel.x += 0.02f * f2 * Mth::cos(2 * float(M_PI) * f1);
		pItemEntity->vel.y += 0.1f * (m_random.nextFloat() - m_random.nextFloat());
		pItemEntity->vel.z += 0.02f * f2 * Mth::sin(2 * float(M_PI) * f1);
	}

	reallyDrop(pItemEntity);
}

void Player::drop()
{

}

float Player::getDestroySpeed()
{
	return 0.5f;
}

int Player::getInventorySlot(int x)
{
	return 0;
}

Pos Player::getRespawnPosition()
{
	return m_respawnPos;
}

int Player::getScore()
{
	return m_score;
}

void Player::prepareCustomTextures()
{

}

void Player::reallyDrop(ItemEntity* pEnt)
{
	level->addEntity(pEnt);
}

void Player::respawn()
{

}

void Player::rideTick()
{

}

void Player::setDefaultHeadHeight()
{
	heightOffset = 1.62f;
}

void Player::setRespawnPos(Pos* pos)
{
	if (!pos)
	{
		m_bHaveRespawnPos = false;
		return;
	}

	m_bHaveRespawnPos = true;
	m_respawnPos.x = pos->x;
	m_respawnPos.y = pos->y;
	// @BUG: no m_respawnPos.z = pos->z ??
}

void Player::startCrafting(int x, int y, int z)
{

}

void Player::swing()
{
	field_BA8 = -1;
	field_BA4 = true;
}

void Player::take(Entity* pEnt, int x)
{

}

void Player::touch(Entity* pEnt)
{
	pEnt->playerTouch(this);
}

void Player::interact(Entity* pEnt)
{
	pEnt->interact(this);
}
