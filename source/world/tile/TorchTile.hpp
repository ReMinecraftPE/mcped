/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
#pragma once

#include "Tile.hpp"

class TorchTile : public Tile
{
public:
	TorchTile(int ID, int texture, Material* pMtl);

	AABB* getAABB(Level*, int x, int y, int z) override;
	bool isSolidRender() override;
	bool isCubeShaped() override;
	int getRenderShape() override;
	void animateTick(Level*, int x, int y, int z, Random*) override;
	HitResult clip(Level*, int x, int y, int z, Vec3 a, Vec3 b) override;
	bool mayPlace(Level*, int, int, int) override;
	void neighborChanged(Level*, int, int, int, int) override;
	void onPlace(Level*, int, int, int) override;
	void setPlacedOnFace(Level*, int, int, int, int) override;
	void tick(Level*, int, int, int, Random*) override;

	bool checkCanSurvive(Level*, int x, int y, int z);
};
