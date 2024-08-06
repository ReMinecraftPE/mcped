/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Particle.hpp"
#include "world/level/Level.hpp"

BubbleParticle::BubbleParticle(Level* level, float x, float y, float z, float vx, float vy, float vz) :
	Particle(level, x, y, z, vx, vy, vz)
{
	field_F8 = field_FC = field_100 = 1.0f;
	field_DC = PTI_BUBBLE;
	setSize(0.02f, 0.02f);

	field_F0 *= 0.2f + 0.6f * sharedRandom.nextFloat();
	vel.x = vx * 0.2f + 0.02f * (2.0f * Mth::random() - 1.0f);
	vel.y = vy * 0.2f + 0.02f * (2.0f * Mth::random() - 1.0f);
	vel.z = vz * 0.2f + 0.02f * (2.0f * Mth::random() - 1.0f);
	field_EC = int(8.0f / (Mth::random() * 0.8f + 0.2f));
}

void BubbleParticle::tick()
{
	posO = pos;

	vel.y += 0.002f;
	move(vel.x, vel.y, vel.z);

	vel *= 0.85f;

	if (level->getMaterial(Mth::floor(pos.x), Mth::floor(pos.y), Mth::floor(pos.z)) != Material::water)
		remove();

	field_EC--;
	if (field_EC <= 0)
		remove();
}
