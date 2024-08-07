/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Particle.hpp"

float Particle::xOff, Particle::yOff, Particle::zOff;

Particle::Particle(Level* level, float x, float y, float z, float vx, float vy, float vz) : Entity(level)
{
	setSize(0.2f, 0.2f);
	heightOffset = 0.5f * bbHeight;
	
	setPos(x, y, z);
	vel.x = vx + 0.4f * (2.0f * Mth::random() - 1.0f);
	vel.y = vy + 0.4f * (2.0f * Mth::random() - 1.0f);
	vel.z = vz + 0.4f * (2.0f * Mth::random() - 1.0f);

	float mult = 0.4f * 0.15f * (Mth::random() + Mth::random() + 1.0f) / Mth::sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
	vel.x *= mult;
	vel.y = vel.y * mult + 0.1f;
	vel.z *= mult;

	field_E0 = 3.0f * sharedRandom.nextFloat();
	field_E4 = 3.0f * sharedRandom.nextFloat();
	field_F0 = 2.0f * (0.5f + 0.5f * sharedRandom.nextFloat());
	field_EC = int(4.0f / (0.1f + 0.9f * sharedRandom.nextFloat()));
}

int Particle::getParticleTexture()
{
	return PT_PARTICLES;
}

Particle* Particle::scale(float f)
{
	setSize(0.2f * f, 0.2f * f);
	field_F0 *= f;
	return this;
}

Particle* Particle::setPower(float f)
{
	vel.x *= f;
	vel.y = (vel.y - 0.1f) * f + 0.1f;
	vel.z *= f;
	return this;
}

void Particle::render(Tesselator& t, float f, float a4, float a5, float a6, float a7, float a8)
{
	constexpr float C_MAGIC_1 = 0.062438f; // @BUG: Slightly bigger than 1/16.0f

	int texture = field_DC;
	int texX = texture % 16;
	if (texture < 0)
		texture += 15;

	float texU_1 = float(texX) / 16.0f;
	float texV_1 = float(texture >> 4) / 16.0f;

	float posX = Lerp(posO.x, pos.x, f) - xOff;
	float posY = Lerp(posO.y, pos.y, f) - yOff;
	float posZ = Lerp(posO.z, pos.z, f) - zOff;
	float fBright = getBrightness(f);

	float sizeX = a4 * field_F0 * 0.1f;
	float sizeY = a5 * field_F0 * 0.1f;
	float sizeZ = a6 * field_F0 * 0.1f;
	float siz2X = a7 * field_F0 * 0.1f;
	float siz2Z = a8 * field_F0 * 0.1f;

	t.color(field_F8 * fBright, field_FC * fBright, field_100 * fBright);
	t.vertexUV(posX - sizeX - siz2X, posY - sizeY, posZ - sizeZ - siz2Z, texU_1 + C_MAGIC_1, texV_1 + C_MAGIC_1);
	t.vertexUV(posX - sizeX + siz2X, posY + sizeY, posZ - sizeZ + siz2Z, texU_1 + C_MAGIC_1, texV_1);
	t.vertexUV(posX + sizeX + siz2X, posY + sizeY, posZ + sizeZ + siz2Z, texU_1,             texV_1);
	t.vertexUV(posX + sizeX - siz2X, posY - sizeY, posZ + sizeZ - siz2Z, texU_1,             texV_1 + C_MAGIC_1);
}

void Particle::tick()
{
	posO = pos;
	field_E8++;
	if (field_E8 >= field_EC)
		remove();
	
	vel.y -= field_F4 * 0.04f;
	move(vel.x, vel.y, vel.z);

	vel.x *= 0.98f;
	vel.y *= 0.98f;
	vel.z *= 0.98f;

	if (onGround)
	{
		vel.x *= 0.7f;
		vel.z *= 0.7f;
	}
}
