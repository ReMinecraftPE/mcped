/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
#pragma once

#include "Tile.hpp"

class Bush : public Tile
{
public:
	Bush(int id, int texture);

	bool canSurvive(Level*, int x, int y, int z) override;
	AABB* getAABB(Level*, int x, int y, int z) override;
	int getRenderShape() override;
	bool isCubeShaped() override;
	bool isSolidRender() override;
	bool mayPlace(Level*, int x, int y, int z) override;
	void tick(Level*, int x, int y, int z, Random*) override;
	void neighborChanged(Level*, int x, int y, int z, int dir) override;

	void checkAlive(Level*, int x, int y, int z);
};
