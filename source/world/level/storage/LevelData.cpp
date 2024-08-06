/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "LevelData.hpp"

LevelData::LevelData()
{
}

LevelData::LevelData(TLong seed, const std::string& name, int x)
{
	m_seed = seed;
	field_20 = x;
	field_78 = name;
}

void LevelData::read(RakNet::BitStream& bs, int version)
{
	field_20 = version;
	bs.Read(m_seed);
	bs.Read(m_spawnPos.x);
	bs.Read(m_spawnPos.y);
	bs.Read(m_spawnPos.z);
	bs.Read(field_10);
	bs.Read(field_18);
	bs.Read(field_14);

	RakNet::RakString rs;
	bs.Read(rs);
	field_78 = std::string(rs.C_String());
}

void LevelData::write(RakNet::BitStream& bs)
{
	bs.Write(m_seed);
	bs.Write(m_spawnPos.x);
	bs.Write(m_spawnPos.y);
	bs.Write(m_spawnPos.z);
	bs.Write(field_10);
	bs.Write(field_18);
	bs.Write(int(getEpochTimeS()));

	RakNet::RakString rs(field_78.c_str());
	bs.Write(rs);
}

void PlayerData::loadPlayer(Player* player)
{
	player->setPos(0.0f, 0.0f, 0.0f);

	player->pos    = m_pos;
	player->posO   = m_pos;
	player->posOld = m_pos;
	player->vel.x = Mth::abs(m_vel.x) > 10.0f ? 0.0f : m_vel.x;
	player->vel.y = Mth::abs(m_vel.y) > 10.0f ? 0.0f : m_vel.y;
	player->vel.z = Mth::abs(m_vel.z) > 10.0f ? 0.0f : m_vel.z;

	player->xRot = player->xRotO = m_pitch;
	player->yRot = player->yRotO = m_yaw;
	player->fallDistance = m_distanceFallen;
	player->onFire    = field_24;
	player->airSupply = field_26;
	player->onGround  = field_28;

	// @NOTE: Why are we updating m_pos, field_3C and field_98 above if we do this?
	player->setPos(m_pos.x, m_pos.y, m_pos.z);

	for (int i = 0; i < C_MAX_HOTBAR_ITEMS; i++)
		player->m_pInventory->setSelectionSlotItemId(i, m_hotbar[i]);
}

void PlayerData::savePlayer(Player* player)
{
	m_pos = player->pos;
	m_vel = player->vel;
	m_pitch = player->xRot;
	m_yaw   = player->yRot;
	m_distanceFallen = player->fallDistance;
	field_24 = player->onFire;
	field_26 = player->airSupply;
	field_28 = player->onGround;

	for (int i = 0; i < C_MAX_HOTBAR_ITEMS; i++)
		m_hotbar[i] = player->m_pInventory->getSelectionSlotItemId(i);
}
