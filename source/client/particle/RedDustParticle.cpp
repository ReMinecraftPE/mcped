/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Particle.hpp"

RedDustParticle::RedDustParticle(Level* level, float x, float y, float z, float vx, float vy, float vz) :
	Particle(level, x, y, z, 0.0f, 0.0f, 0.0f)
{
	vel.x = vx + vel.x * 0.1f;
	vel.y = vy + vel.y * 0.1f;
	vel.z = vz + vel.z * 0.1f;

	if (vx == 0.0f)
		vx = 1.0f;

	float f = Mth::random() * 0.4f + 0.6f;
	field_F8  = f * vx * (Mth::random() * 0.2f + 0.8f);
	field_FC  = f * vy * (Mth::random() * 0.2f + 0.8f);
	field_100 = f * vz * (Mth::random() * 0.2f + 0.8f);

	field_104 = field_F0 = field_F0 * 0.75f;

	noPhysics = false;
	field_EC = int(8.0f / (0.2f + 0.8f * Mth::random()));
}

void RedDustParticle::render(Tesselator& t, float f, float a, float b, float c, float d, float e)
{
	float mult = 32.0f * (float(field_E8 + f) / float(field_EC));
	if (mult < 0.0f)
		mult = 0.0f;
	if (mult > 1.0f)
		mult = 1.0f;

	field_F0 = field_104 * mult;
	Particle::render(t, f, a, b, c, d, e);
}

void RedDustParticle::tick()
{
	posO = pos;

	field_E8++;
	if (field_E8 > field_EC)
		remove();

	vel.y += 0.004f;
	field_DC = -8 * field_E8 / field_EC + 7;

	move(vel.x, vel.y, vel.z);

	if (pos.y == posO.y)
	{
		vel.x *= 1.1f;
		vel.z *= 1.1f;
	}

	vel *= 0.96f;

	if (onGround)
	{
		vel.x *= 0.7f;
		vel.z *= 0.7f;
	}
}
