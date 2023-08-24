/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "EntityRenderer.hpp"
#include "world/item/ItemInstance.hpp"
#include "client/renderer/TileRenderer.hpp"

class ItemRenderer : public EntityRenderer
{
public:
	ItemRenderer();

	void render(Entity*, float, float, float, float, float) override;
	void blitRect(Tesselator&, int, int, int, int, int);

	static void blit(int, int, int, int, int, int);
	static void renderGuiItem(Font*, Textures*, ItemInstance*, int, int, bool);

public:
	Random m_random;
	static TileRenderer* tileRenderer;
};

