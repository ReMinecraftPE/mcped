/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
#pragma once

#include "Textures.hpp"
class Textures; // in case we are being included from Textures. We don't need a complete type

// Essentially, the way these work is by patching themselves into terrain.png with a
// call to glTexSubImage2D

class DynamicTexture
{
public:
	virtual void tick() = 0;
	virtual void bindTexture(Textures*);

	DynamicTexture(int a2);
	virtual ~DynamicTexture();

protected:
	int m_textureIndex = 0;
	int m_textureSize = 1;
	uint8_t m_pixels[1024];

	friend class Textures;
};
