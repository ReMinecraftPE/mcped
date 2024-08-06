/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "LocalPlayer.hpp"
#include "client/Minecraft.hpp"

int dword_250ADC, dword_250AE0;

LocalPlayer::LocalPlayer(Minecraft* pMinecraft, Level* pLevel, User* pUser, int i) : Player(pLevel)
{
	m_pMinecraft = pMinecraft;
	m_name = pUser->field_0;

	field_BC4 = i;
	field_C38 = m_pInventory->getSelectedItemId();
}

LocalPlayer::~LocalPlayer()
{
	if (m_pKeyboardInput)
		delete m_pKeyboardInput;
}

void LocalPlayer::aiStep()
{
	m_pKeyboardInput->tick(/* this */);
	if (m_pKeyboardInput->m_bSneakButton && ySlideOffset < 0.2f)
	{
		ySlideOffset = 0.2f;
	}

	Mob::aiStep();
	Player::aiStep();
}

void LocalPlayer::animateRespawn()
{

}

void LocalPlayer::calculateFlight(float x, float y, float z)
{
	float f1 = m_pMinecraft->m_options.field_244;
	float x1 = f1 * x;
	float z1 = f1 * z;

	float y1 = 0.0f;
	if (Keyboard::_states[AKEYCODE_BUTTON_R1] == 1)
		y1 = f1 * 0.2f;
	if (Keyboard::_states[AKEYCODE_BUTTON_L1] == 1)
		y1 = f1 * -0.2f;

	field_BFC += x1;
	float f2 = m_pMinecraft->m_options.field_8 * 0.35f;
	float f3 = f2 * (field_BFC - field_C00);
	float f4 = field_C04 + 0.5f * (f3 - field_C04);
	field_C04 = f4;
	if ((f3 <= 0.0f || f3 <= f4) && (f3 >= 0.0f || f3 >= f4))
		f4 = f3;
	field_C00 += f4;
	field_BF0 = f4 * 10.0f;

	field_C08 += y1;
	float f5 = f2 * (field_C08 - field_C0C);
	float f6 = field_C10 + 0.5f * (f5 - field_C10);
	field_C10 = f6;
	if ((f5 <= 0.0f || f5 <= f6) && (f5 >= 0.0f || f5 >= f6))
		f6 = f5;
	field_C0C += f6;
	field_BF4 = f6 * 10.0f;

	field_C14 += z1;
	float f7 = f2 * (field_C14 - field_C18);
	float f8 = field_C1C + 0.5f * (f7 - field_C1C);
	field_C1C = f8;
	if ((f7 <= 0.0f || f7 <= f8) && (f7 >= 0.0f || f7 >= f8))
		f8 = f7;
	field_C18 += f8;
	field_BF8 = f8 * 10.0f;
}

void LocalPlayer::closeContainer()
{
	Player::closeContainer();
	m_pMinecraft->setScreen(nullptr);
}

bool LocalPlayer::isSneaking()
{
	return m_pKeyboardInput->m_bSneakButton;
}

int LocalPlayer::move(float x, float y, float z)
{
	int result = 0;

	LocalPlayer* pLP = m_pMinecraft->m_pLocalPlayer;
	if (Minecraft::DEADMAU5_CAMERA_CHEATS && pLP == this && m_pMinecraft->m_options.m_bFlyCheat)
	{
		//@HUH: Using m_pMinecraft->m_pLocalPlayer instead of this, even though they're the same
		pLP->noPhysics = true;

		float field_94_old = walkDist;

		pLP->calculateFlight(x, y, z);
		pLP->fallDistance = 0.0f;
		pLP->vel.y = 0.0f;

		// This looks very funny.
		result = pLP->Entity::move(field_BF0, field_BF4, field_BF8);

		pLP->onGround = true;

		walkDist = field_94_old;
	}
	else
	{
#ifndef ORIGINAL_CODE
		// @BUG: In the original Minecraft, you can't stop flying! If you do, you'll just fall through to the bottom of the world. :(
		pLP->noPhysics = false;
#endif

		// autojump stuff
		if (m_nAutoJumpFrames > 0)
		{
			m_nAutoJumpFrames--;
			m_pKeyboardInput->m_bJumpButton = true;
		}

		float posX = pos.x;
		float posY = pos.y;

		result = Entity::move(x, y, z);

		//@BUG: backing up posZ too late
		float posZ = pos.z;

		if (m_nAutoJumpFrames <= 0)
		{
			if (Mth::floor(posX * 2) == Mth::floor(pos.x * 2) &&
				Mth::floor(posY * 2) == Mth::floor(pos.y * 2) &&
				Mth::floor(posZ * 2) == Mth::floor(pos.z * 2))
				return result;

			float dist = Mth::sqrt(x * x + z * z);
			int x1 = Mth::floor(x / dist + pos.x);
			int z1 = Mth::floor(z / dist + pos.z);

			TileID tileOnTop = level->getTile(x1, int(pos.y - 1.0f), z1);

			// not standing on top of a tile?
			if (!level->isSolidTile(x1, int(pos.y - 1.0f), z1))
				return 0;

			// aren't inside of a tile right now
			if (level->isSolidTile(x1, int(pos.y), z1))
				return 0;

			// don't have anything on top of us
			if (level->isSolidTile(x1, int(pos.y + 1.0f), z1))
				return 1;

			// are we trying to walk into stairs or a slab?
			if (tileOnTop != Tile::stairs_stone->id && tileOnTop != Tile::stairs_wood->id && tileOnTop != Tile::stoneSlabHalf->id)
				// Nope, we're walking towards a full block. Trigger an auto jump.
				m_nAutoJumpFrames = 1;
		}
	}

	return result;
}

void LocalPlayer::tick()
{
	Player::tick();

	if (m_pMinecraft->isOnline())
	{
		if (fabsf(pos.x - field_C24) > 0.1f  ||
			fabsf(pos.y - field_C28) > 0.01f ||
			fabsf(pos.z - field_C2C) > 0.1f  ||
			fabsf(field_C30 - xRot) > 1.0f  ||
			fabsf(field_C34 - yRot) > 1.0f)
		{
			m_pMinecraft->m_pRakNetInstance->send(new MovePlayerPacket(entityId, pos.x, pos.y - heightOffset, pos.z, xRot, yRot));
			field_C24 = pos.x;
			field_C28 = pos.y;
			field_C2C = pos.z;
			field_C30 = xRot;
			field_C34 = yRot;
		}

		if (field_C38 != m_pInventory->getSelectedItemId())
		{
			field_C38 = m_pInventory->getSelectedItemId();
			m_pMinecraft->m_pRakNetInstance->send(new PlayerEquipmentPacket(entityId, field_C38));
		}
	}
}

void LocalPlayer::updateAi()
{
	Player::updateAi();

	field_B00 = m_pKeyboardInput->m_horzInput;
	field_B04 = m_pKeyboardInput->m_vertInput;

	field_B0C = m_pKeyboardInput->m_bJumpButton || m_nAutoJumpFrames > 0;
}
