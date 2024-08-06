/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "PrimedTnt.hpp"
#include "world/level/Level.hpp"

PrimedTnt::PrimedTnt(Level* level) : Entity(level)
{
	m_fuseTimer = 0;
	field_C8 = RENDER_TNT;
	field_34 = 1;
	setSize(0.98f, 0.98f);
	heightOffset = bbHeight * 0.5f;
}

PrimedTnt::PrimedTnt(Level* level, float x, float y, float z) : PrimedTnt(level)
{
	setPos(x, y, z);

	float fAng = 0.017453f * 2.0f * float(M_PI) * Mth::random();

	vel.x = -0.02f * Mth::sin(fAng);
	vel.z = -0.02f * Mth::cos(fAng);
	vel.y = 0.2f;

	posO = pos;

	m_fuseTimer = 80; // 4 secs
}

void PrimedTnt::explode()
{
	// @NOTE: Not passing `this` as pointer to entity
	level->explode(nullptr, pos.x, pos.y, pos.z, 2.5f);
}

float PrimedTnt::getShadowHeightOffs()
{
	return 0.0f;
}

bool PrimedTnt::isPickable()
{
	return !removed;
}

void PrimedTnt::tick()
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

	m_fuseTimer--;
	if (m_fuseTimer <= 0)
	{
		remove();
		explode();
	}
	else
	{
		level->addParticle("smoke", pos.x, pos.y + 0.5f, pos.z, 0.0f, 0.0f, 0.0f);
	}
}
