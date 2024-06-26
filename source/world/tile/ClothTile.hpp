/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
#pragma once

#include "Tile.hpp"

class ClothTile : public Tile
{
public:
	ClothTile(int id, int type);
	int getTexture(int dir) override;
	int getTexture(int dir, int data) override;
	int getSpawnResourcesAuxValue(int val) override;

	uint8_t field_6C;
};
