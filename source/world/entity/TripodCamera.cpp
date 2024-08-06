/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "TripodCamera.hpp"
#include "Player.hpp"
#include "world/level/Level.hpp"

TripodCamera::TripodCamera(Level* level, Player* player, float x, float y, float z) : Mob(level)
{
	m_owner = player;
	field_C8 = RENDER_CAMERA;

	xRotO = xRot = player->xRot;
	yRotO = yRot = player->yRot;

	field_34 = 1;

	setSize(1.0f, 1.5f);
	heightOffset = bbHeight * 0.5f - 0.25f;
	
	setPos(x, y, z);
	posO.x = x;
	posO.y = y;
	posO.z = z;
}

void TripodCamera::defineSynchedData()
{

}

float TripodCamera::getShadowHeightOffs()
{
	return 0.0f;
}

bool TripodCamera::interact(Player* player)
{
	// @BUG-ish: No check for owner?
	m_bActive = true;
	return true;
}

int TripodCamera::interactPreventDefault()
{
	return 1;
}

bool TripodCamera::isPickable()
{
	return !removed;
}

bool TripodCamera::isPushable()
{
	return false;
}

void TripodCamera::tick()
{
	posO = pos;

	vel.y -= 0.04f;
	move(vel.x, vel.y, vel.z);

	vel *= 0.98f;
	if (onGround)
	{
		vel.x *= 0.7f;
		vel.z *= 0.7f;
		vel.y *= -0.5f;
	}

	if (!m_bActive)
		return;

	field_B90--;
	if (field_B90 == 0)
	{
		remove();
		return;
	}

	if (field_B90 == 8)
	{
		level->takePicture(this, m_owner);
		level->addParticle("explode", pos.x, pos.y + 0.6f, pos.z, 0.0f, 0.0f, 0.0f);
		level->addParticle("explode", pos.x, pos.y + 0.8f, pos.z, 0.0f, 0.0f, 0.0f);
		level->addParticle("explode", pos.x, pos.y + 1.0f, pos.z, 0.0f, 0.0f, 0.0f);
		return;
	}

	if (field_B90 > 8)
	{
		level->addParticle("smoke", pos.x, pos.y + 1.0f, pos.z, 0.0f, 0.0f, 0.0f);
	}
}
